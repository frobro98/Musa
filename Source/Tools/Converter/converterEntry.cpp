/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "Common/Common.h"


#include "File/FileCore/File.h"
#include "Model/ModelFileHeader.h"
#include "Model/GeometryPrimitives.h"
#include "Model/MeshUtilities.hpp"
#include "Texture/TextureChunk.h"

#include "File/Path.hpp"
#include "Texture/ImageFormats.h"
#include "Processing/PrimitiveGeneration.h"
#include "Archiver/AnimationHeader.h"
#include "Archiver/ChunkHeader.h"
#include "DirectoryLocations.h"

#include "Math/MathEngine.h"
#include "Containers/DynamicArray.hpp"

static bool gVerbose = true;

struct VertexWithFace
{
	Vertex v{};
	uint32 faceIndex = 0;
	int32 weightIndex = -1;

	bool operator<(const VertexWithFace& vert)
	{
		return v < vert.v;
	}
};

struct TexturePath
{
	std::string textureFilePath;
};

constexpr const uint32 ModelNameLength = 32;

struct ModelData
{
	DynamicArray<Vertex> vertices;
	DynamicArray<Face> faces;
	DynamicArray<VertexBoneWeights> weights;
	DynamicArray<TexturePath> textures;
	std::string directoryPath;
	std::string modelName;
};

std::vector<TexturePath> GetTextureData(FbxGeometry* pGeometry);

namespace
{
struct BoneData
{
	std::vector<Vector4> translationKeys;
	std::vector<Quat> rotationKeys;
	std::vector<Vector4> scaleKeys;
	std::vector<uint32> timeMSPerKey;
	uint32 totalAnimTimeMS;
	std::string name;
};

struct PoseData
{
	Matrix invPoseMat{ IDENTITY };
	uint32 index = 0;
	std::string boneName = "";
};

struct AnimationDescription
{
	std::string name;
	std::vector<BoneData> bones;
};

struct Hierarchy
{
	std::string name;
	int index;
	int parentIndex;
};

struct Frame
{
	std::vector<FrameData> bonesPerFrame;
	uint32 frameTime;
};

struct AnimationClip
{
	std::string name;
	std::vector<Frame> frames;
	uint32 totalTime;
};

struct SkinData
{
	std::vector<std::pair<int, float>> bonesWeights;
};

struct HierarchyTableElement
{
	std::vector<uint32> boneParentHierarchy;
};

struct HierarchyTableDescription
{
	std::vector<HierarchyTableElement> tableElements;
	uint32 maxElementLength = std::numeric_limits<uint32>::min();
};
}

// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

void ProcessMesh(FbxNode* node, const std::vector<Hierarchy>& bones, std::vector<VertexWithFace>& vertList, std::vector<SkinData>& skinData, ModelData& modelData);
void ReduceAndSaveModelData(const std::vector<VertexWithFace>& verts, const std::vector<SkinData>& weights, ModelData& modelData);
void PackModelData(const std::string& fileName);

static std::vector<PoseData> posesCorrect;

static CompressionFormat compressionFormat = CompressionFormat::Invalid;

static void RecurseHierarchy(FbxNode* node, int parentIndex, int& index, std::vector<Hierarchy>& boneHierarchy, uint32 depth = 0)
{
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr == nullptr)
	{
		printf("Attribute of node %s is null... \n\n", node->GetName());
	}
	else
	{
		if (attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			Hierarchy boneHier;
			boneHier.index = static_cast<int>(boneHierarchy.size());
			boneHier.parentIndex = parentIndex;
			boneHier.name = node->GetName();
			boneHierarchy.push_back(boneHier);

			std::stringstream stream;
			for (uint32 i = 0; i < depth; ++i)
			{
				stream << "     ";
			}

			stream << node->GetName() << " Index: " << index << " Parent's Index: " << parentIndex;
			printf("%s\n", stream.str().c_str());

			int currentIndex = index;
	
			for (int i = 0; i < node->GetChildCount(); ++i)
			{
				RecurseHierarchy(node->GetChild(i), currentIndex, ++index, boneHierarchy, depth + 1);
			}
		}
		else
		{
			for (int i = 0; i < node->GetChildCount(); ++i)
			{
				RecurseHierarchy(node->GetChild(i), parentIndex, index, boneHierarchy, depth + 1);
			}
		}
	}
}

std::vector<Hierarchy> ComposeHierarchy(FbxScene* scene)
{
	std::vector<Hierarchy> boneHierarchy;
	int index = 0;
	RecurseHierarchy(scene->GetRootNode()->GetChild(0), -1, index, boneHierarchy);

	return boneHierarchy;
}

HierarchyTableDescription ConstructTable(const std::vector<Hierarchy>& hierarchy)
{
	HierarchyTableDescription table;
	for (const auto& bone : hierarchy)
	{
		HierarchyTableElement boneElement;
		int32 parentIndex = bone.parentIndex;
		boneElement.boneParentHierarchy.push_back(bone.index);
		while (parentIndex >= 0)
		{
			boneElement.boneParentHierarchy.push_back(parentIndex);
			parentIndex = hierarchy[parentIndex].parentIndex;
		}
		//std::reverse(boneElement.boneParentHierarchy.begin(), boneElement.boneParentHierarchy.end());

		table.tableElements.push_back(boneElement);
		table.maxElementLength = std::max(table.maxElementLength, boneElement.boneParentHierarchy.size());
	}

	Assert(table.tableElements.size() == hierarchy.size());
	return table;
}

void SaveHierarchy(const std::string& fileName, const std::vector<Hierarchy>& hierarchy, const std::vector<PoseData>& poseData, const HierarchyTableDescription& hierarchyTable)
{
	std::string file = fileName + ".skel";
	File::Handle skeletonFile;
	File::Result result = File::Open(skeletonFile, file.c_str(), File::Mode::WRITE);
	Assert(result == File::Result::SUCCESS);

	SkeletonHeader header;
	uint32 hierarchyOffset, tableOffset, poseDataOffset;
	header.boneCount = hierarchy.size();
	result = File::Write(skeletonFile, &header, sizeof(SkeletonHeader));
	Assert(result == File::Result::SUCCESS);

	File::Tell(skeletonFile, hierarchyOffset);
	header.boneHierarchyOffset = hierarchyOffset;

	for (const Hierarchy& hier : hierarchy)
	{
		SingleBoneData data;
		memset(&data, 0, sizeof(SingleBoneData));
		hier.name.copy(data.boneName, BoneNameLength);
		data.parentIndex = hier.parentIndex;

		result = File::Write(skeletonFile, &data, sizeof(SingleBoneData));
		Assert(result == File::Result::SUCCESS);
	}

	File::Tell(skeletonFile, tableOffset);
	header.boneTableOffset = tableOffset;
	result = File::Write(skeletonFile, &hierarchyTable.maxElementLength, sizeof(uint32));
	Assert(result == File::Result::SUCCESS);

	for (const auto& elem : hierarchyTable.tableElements)
	{
		uint32 numParents = static_cast<uint32>(elem.boneParentHierarchy.size());
		result = File::Write(skeletonFile, &numParents, sizeof(uint32));
		Assert(result == File::Result::SUCCESS);

		result = File::Write(skeletonFile, elem.boneParentHierarchy.data(), sizeof(uint32) * numParents);
		Assert(result == File::Result::SUCCESS);
	}

	
	// TODO - Determine if this pose data should be required if there's a skeleton
	if (poseData.size() > 0)
	{
		File::Tell(skeletonFile, poseDataOffset);
		header.bonePoseOffset = poseDataOffset;

		for (const PoseData& pose : poseData)
		{
			BonePoseData data;
			data.inversePoseMatrix = pose.invPoseMat;

			result = File::Write(skeletonFile, &data, sizeof(BonePoseData));
			Assert(result == File::Result::SUCCESS);
		}
	}

	File::Seek(skeletonFile, File::Location::BEGIN, 0);
	result = File::Write(skeletonFile, &header, sizeof(SkeletonHeader));
	Assert(result == File::Result::SUCCESS);

	result = File::Close(skeletonFile);
	Assert(result == File::Result::SUCCESS);
}

void SaveOutAnimation(uint32 skeletonHash, const std::vector<AnimationClip>& clips, Time::Duration frameRate)
{
	for (const auto& clip : clips)
	{
		printf("\n\nSAVING OUT THE DATA\\n\n");
		std::string file = clip.name + ".anim";
		File::Handle animationFile;
		File::Result result = File::Open(animationFile, file.c_str(), File::Mode::WRITE);
		Assert(result == File::Result::SUCCESS);

		AnimationHeader header;
		clip.name.copy(header.animationName, AnimationNameLength);
		// TODO - figure out how to get this hash
		header.referenceSkeleton = skeletonHash;
		header.keyFrameCount = clip.frames.size();
		header.bonesPerFrame = clip.frames[0].bonesPerFrame.size();
		header.totalAnimationTime = clip.totalTime;
		header.frameRate = frameRate;

		result = File::Write(animationFile, &header, sizeof(AnimationHeader));
		Assert(result == File::Result::SUCCESS);

		for (const Frame& keyFrame : clip.frames)
		{
			KeyFrame frame;
			frame.boneCount = static_cast<uint32>(keyFrame.bonesPerFrame.size());
			frame.frameTime = Time(Time::ONE_MILLISECOND) * keyFrame.frameTime;
			frame.boneFrameData = nullptr;

			result = File::Write(animationFile, &frame, sizeof(KeyFrame));
			Assert(result == File::Result::SUCCESS);

			for (const FrameData& data : keyFrame.bonesPerFrame)
			{
				printf("Translation: x(%f) y(%f) z(%f)\nRotation: x(%f) y(%f) z(%f) w(%f)\nScale x(%f) y(%f) z(%f)\n\n",
					data.translation.x, data.translation.y, data.translation.z,
					data.rotation.x, data.rotation.y, data.rotation.z, data.rotation.w,
					data.scale.x, data.scale.y, data.scale.z
				);

				result = File::Write(animationFile, &data, sizeof(FrameData));
				Assert(result == File::Result::SUCCESS);
			}
		}

		result = File::Close(animationFile);
		Assert(result == File::Result::SUCCESS);
	}
}

uint32 GetSkeletonHash(const std::string& fileName)
{
	File::Handle fHandle;
	File::Result result = File::Open(fHandle, fileName.c_str(), File::Mode::READ);
	Assert(result == File::Result::SUCCESS);

	ChunkHeader header;
	result = File::Read(fHandle, &header, sizeof(ChunkHeader));
	Assert(result == File::Result::SUCCESS);

	result = File::Close(fHandle);
	Assert(result == File::Result::SUCCESS);

	return header.hashNum;
}

Time::Duration ConvertFbxFrameRate(FbxTime::EMode frameRate)
{
	switch (frameRate)
	{
	case fbxsdk::FbxTime::eFrames60:
		return Time::NTSC_FRAME;
	case fbxsdk::FbxTime::eFrames50:
		return Time::PAL_FRAME;
	case fbxsdk::FbxTime::eFrames30:
		return Time::NTSC_30_FRAME;
	case fbxsdk::FbxTime::eFrames24:
		return Time::NTSC_24_FRAME;
// 	case fbxsdk::FbxTime::ePAL: ? 
// 		break;
	default:
		printf("%d\n", frameRate);
		Assert(false);
	}

	return Time::DWORD;
}

constexpr const uint32 SystemCallLength = 512;

void PackAndMoveAnimationData(const std::string& fileName)
{
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "liu \"%s_anim.pak\" -v v0.1 -n \"%s_anim\"",
		fileName.c_str(), fileName.c_str());
	system(systemCall);

	system("del *.chnk");

	// TODO - find the actual move location within file system. Make this more intelligent
	std::string moveLocation = "../Assets/";
	moveLocation += "Models/";

	sprintf_s(systemCall, "move /y %s_anim.pak %s%s_anim.pak > nul",
		fileName.c_str(), moveLocation.c_str(), fileName.c_str());

	system(systemCall);
}

void ChunkHierarchy(const std::string& fileName)
{
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "vodka \"%s.skel\" \"%s_skel.chnk\" -t SKEL_TYPE -n \"%s_Skel\"",
		fileName.c_str(), fileName.c_str(), fileName.c_str());

	system(systemCall);
	system("del *.skel");
}

void ChunkAnimationData(const std::vector<AnimationClip>& clips)
{
	char systemCall[SystemCallLength];

	for (const auto& clip : clips)
	{
		sprintf_s(systemCall, "vodka \"%s.anim\" \"%s.chnk\" -t ANIM_TYPE -n \"%s_anim\"",
			clip.name.c_str(), clip.name.c_str(), clip.name.c_str());

		system(systemCall);
	}

	system("del *.anim");
}


//////////////////////////////////////////////////////////////////////////
// Skinning!

static Matrix FbxMatToMusaMat(const FbxAMatrix& fbxMat)
{
	Matrix mat;
	mat.Set(ROW_0, Vector4{ static_cast<float>(fbxMat[0][0]), static_cast<float>(fbxMat[0][1]), static_cast<float>(fbxMat[0][2]), static_cast<float>(fbxMat[0][3]) });
	mat.Set(ROW_1, Vector4{ static_cast<float>(fbxMat[1][0]), static_cast<float>(fbxMat[1][1]), static_cast<float>(fbxMat[1][2]), static_cast<float>(fbxMat[1][3]) });
	mat.Set(ROW_2, Vector4{ static_cast<float>(fbxMat[2][0]), static_cast<float>(fbxMat[2][1]), static_cast<float>(fbxMat[2][2]), static_cast<float>(fbxMat[2][3]) });
	mat.Set(ROW_3, Vector4{ static_cast<float>(fbxMat[3][0]), static_cast<float>(fbxMat[3][1]), static_cast<float>(fbxMat[3][2]), static_cast<float>(fbxMat[3][3]) });

	return mat;
}

std::vector<SkinData> ProcessSkinningData(const std::vector<Hierarchy>& bones, const FbxMesh* geometry)
{
	int32 controlPointCount = geometry->GetControlPointsCount();
	uint32 skinCount = geometry->GetDeformerCount(FbxDeformer::eSkin);
	Assert(skinCount == 1);

	posesCorrect.resize(bones.size());

	std::vector<SkinData> skinData(controlPointCount);

	for (uint32 i = 0; i < skinCount; ++i)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(geometry->GetDeformer(i, FbxDeformer::eSkin));
		uint32 clusterCount = skin->GetClusterCount();
		for (uint32 j = 0; j < clusterCount; ++j)
		{
			FbxCluster* cluster = skin->GetCluster(j);
			auto compareFunc = [&](const Hierarchy& bone)
			{
				return cluster->GetLink()->GetName() == bone.name;
			};

			auto boneIter = std::find_if(bones.cbegin(), bones.cend(), compareFunc);
			Assert(boneIter != bones.end());

			uint32 affectedVertCount = cluster->GetControlPointIndicesCount();
			int32* affectedVertIndices = cluster->GetControlPointIndices();
			double* boneWeights = cluster->GetControlPointWeights();

			printf("Bone[%d]: Weights: \n", boneIter->index);
			for (uint32 k = 0; k < affectedVertCount; ++k)
			{
				uint32 vertIndex = affectedVertIndices[k];
				Hierarchy bone = *boneIter;
				std::pair<int, float> boneWeight(bone.index, static_cast<float>(boneWeights[k]));
				skinData[vertIndex].bonesWeights.push_back(boneWeight);
				printf("\tVert #%u(%f) \n", vertIndex, boneWeights[k]);
			}
			printf("\n");

			// TODO - Move this into a function!!

			FbxAMatrix clusterTransform, linkTransform;
			clusterTransform = cluster->GetTransformMatrix(clusterTransform);
			linkTransform = cluster->GetTransformLinkMatrix(linkTransform).Inverse();

			Matrix clusterMat = FbxMatToMusaMat(clusterTransform);
			Matrix linkMat = FbxMatToMusaMat(linkTransform);

			Matrix poseInv = clusterMat * linkMat;

			PoseData pose = {};
			pose.boneName = boneIter->name;
			pose.index = boneIter->index;
			pose.invPoseMat = poseInv;

			posesCorrect[pose.index] = pose;
		}
	}

	printf("\n");

	for (auto& data : skinData)
	{
		std::sort(data.bonesWeights.begin(), data.bonesWeights.end(),
		[](const std::pair<int, float>& left, const std::pair<int, float>& right)
		{
			return left.second > right.second;
		});
	}

	for (uint32 i = 0; i < skinData.size(); ++i)
	{
		SkinData& data = skinData[i];
		for (uint32 j = 0; j < data.bonesWeights.size(); ++j)
		{
			printf("Vertex %u: Weight %f on Bone #%u\n", 
				i, data.bonesWeights[j].second, data.bonesWeights[j].first);
		}
		printf("\n\n");
	}

	printf("---POSE DATA---\n");
	for (uint32 i = 0; i < posesCorrect.size(); ++i)
	{
		printf("Pose[%u]: %f, %f, %f, %f,\n", i, posesCorrect[i].invPoseMat[m0], posesCorrect[i].invPoseMat[m1], posesCorrect[i].invPoseMat[m2], posesCorrect[i].invPoseMat[m3]);
		printf("          %f, %f, %f, %f,\n", posesCorrect[i].invPoseMat[m4], posesCorrect[i].invPoseMat[m5], posesCorrect[i].invPoseMat[m6], posesCorrect[i].invPoseMat[m7]);
		printf("          %f, %f, %f, %f,\n", posesCorrect[i].invPoseMat[m8], posesCorrect[i].invPoseMat[m9], posesCorrect[i].invPoseMat[m10], posesCorrect[i].invPoseMat[m11]);
		printf("          %f, %f, %f, %f,\n\n", posesCorrect[i].invPoseMat[m12], posesCorrect[i].invPoseMat[m13], posesCorrect[i].invPoseMat[m14], posesCorrect[i].invPoseMat[m15]);
	}

	return skinData;
}


//////////////////////////////////////////////////////////////////////////


void CompressAnimations(std::vector<AnimationClip>& animations, float errorAngle);
void ProcessAnimation(FbxScene* scene, std::vector<AnimationDescription>& animations);
AnimationDescription ProcessAnimationStack(FbxAnimStack* stack, FbxNode* rootNode);
AnimationDescription ProcessAnimationLayer(FbxNode* rootNode, FbxAnimLayer* layer, uint32 frameCount);
void ExtractBoneData(FbxNode* boneRootNode, FbxAnimLayer* layer, std::vector<BoneData>& bones, uint32 frameCount);


int main(int argc, char** argv)
{
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;
	bool compress = false;
	float compressionDegreeError = 0.f;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

	ModelData modelData;
	FbxString lFilePath("");
	for( int i = 1, c = argc; i < c; ++i )
	{
		FbxString arg(argv[i]);
		if( arg == "-test" ) gVerbose = false;
		else if( lFilePath.IsEmpty() ) lFilePath = argv[i];
		else if (arg == "-compress")
		{
			compress = true;
			compressionDegreeError = strtof(argv[i + 1], nullptr);
		}
		else if (arg == "-t")
		{
			TexturePath t;
			FbxString s(argv[i+1]);
			t.textureFilePath = s;
			modelData.textures.Add(t);
			i++;
			printf("Pushed Texture %s\n", (const char*)s);
		}
		else if (arg == "-c")
		{
			if (i + 1 >= argc)
			{
				printf("Didn't pass in a texture compression scheme with the '-c' argument!\n");
				return -1;
			}

			char* args = argv[i + 1];
			if (strcmp(args, "bc7") == 0)
			{
				compressionFormat = CompressionFormat::BC7;
			}
			else if (strcmp(args, "bc1") == 0)
			{
				compressionFormat = CompressionFormat::BC1;
			}
			else if (strcmp(args, "bc3") == 0)
			{
				compressionFormat = CompressionFormat::BC3;
			}
			++i;
		}
	}

	if( lFilePath.IsEmpty() )
	{
        lResult = false;
        FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

	std::string filePath = lFilePath;

	std::vector<VertexWithFace> vertList;
	std::vector<SkinData> skinData;
	size_t endOfPathIndex = filePath.find_last_of('/');
	if (endOfPathIndex == std::string::npos)
	{
		endOfPathIndex = filePath.find_last_of('\\');
	}
	Assert(endOfPathIndex != std::string::npos);
	endOfPathIndex += 1;
	modelData.directoryPath = filePath.substr(0, endOfPathIndex);
	size_t nameLen = filePath.find_last_of('.') - endOfPathIndex;
	std::string fileName = filePath.substr(endOfPathIndex, nameLen);;
	modelData.modelName = fileName;

	if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
    }
	else
	{

		FbxGeometryConverter lGeomConverter(lSdkManager);
		lGeomConverter.Triangulate(lScene, true);

		FbxAxisSystem OpenGL(FbxAxisSystem::eOpenGL);
		OpenGL.ConvertScene(lScene);

		FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

//		if (gVerbose) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

		FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

		//if (gVerbose) DisplayHierarchy(lScene);

		//if( gVerbose ) DisplayContent(lScene);

		std::vector<Hierarchy> bones = ComposeHierarchy(lScene);
		HierarchyTableDescription table = ConstructTable(bones);

		FbxNode* node = lScene->GetRootNode();
		
		if (node)
		{
			// TODO - Clean this shit up!!
			for (int i = 0; i < node->GetChildCount(); i++)
			{
				FbxNode* child = node->GetChild(i);
				ProcessMesh(child, bones, vertList, skinData, modelData);
			}
		}

		// TODO - Convert this and save it out for the engine to absorb
		// Set up time mode for entire fbx file
		FbxTime::EMode fs = lScene->GetGlobalSettings().GetTimeMode();
		FbxTime::SetGlobalTimeMode(fs);

		//if (gVerbose) DisplayPose(lScene);

		//if (gVerbose) DisplayAnimation(lScene);

		if (bones.size() > 0)
		{
			Time::Duration frameRate = ConvertFbxFrameRate(fs);
			SaveHierarchy(fileName, bones, posesCorrect, table);
			ChunkHierarchy(fileName);

			// TODO - Create the hash without opening the chunk file...
			const uint32 skeletonHash = GetSkeletonHash(fileName + "_skel.chnk");

			// TODO - Save out each animation...
			std::vector<AnimationDescription> animations;
			ProcessAnimation(lScene, animations);

			printf("\n\n MOVING FROM BONE TO FRAME RELATIONSHIP \n\n");
			std::vector<AnimationClip> clips;
			for (const auto& animation : animations)
			{
				AnimationClip clip;
				clip.name = animation.name;
				printf("Writing out %s\n", clip.name.c_str());
				uint32 frameCount = animation.bones[0].rotationKeys.size();
				clip.frames.reserve(frameCount);
				for (uint32 i = 0; i < frameCount; ++i)
				{
					Frame frame;
					for (uint32 j = 0; j < animation.bones.size(); ++j)
					{
						FrameData bone;

						Assert(animation.bones[j].translationKeys.size() > 0);
						Assert(animation.bones[j].rotationKeys.size() > 0);
						Assert(animation.bones[j].scaleKeys.size() > 0);
						Assert(animation.bones[j].timeMSPerKey.size() > 0);

						{
							frame.frameTime = animation.bones[j].timeMSPerKey[i];
							bone.translation = animation.bones[j].translationKeys[i];
							bone.rotation = animation.bones[j].rotationKeys[i];
							bone.scale = animation.bones[j].scaleKeys[i];
							clip.totalTime = animation.bones[j].totalAnimTimeMS;

							printf("Translation: x(%f) y(%f) z(%f)\nRotation: x(%f) y(%f) z(%f) w(%f)\nScale x(%f) y(%f) z(%f)\n\n",
								bone.translation.x, bone.translation.y, bone.translation.z,
								bone.rotation.x, bone.rotation.y, bone.rotation.z, bone.rotation.w,
								bone.scale.x, bone.scale.y, bone.scale.z
							);
						}
						frame.bonesPerFrame.push_back(std::move(bone));
					}

					clip.frames.push_back(std::move(frame));
				}

				clips.push_back(clip);
			}

			if (compress)
			{
				CompressAnimations(clips, compressionDegreeError);
			}

			SaveOutAnimation(skeletonHash, clips, frameRate);
			ChunkAnimationData(clips);

			PackAndMoveAnimationData(fileName);
		}

		// After all of this processing is done...
		// Do the reduction and saving out to a file
		ReduceAndSaveModelData(vertList, skinData, modelData);
		PackModelData(fileName);
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Compression
//////////////////////////////////////////////////////////////////////////


static float CalculateErrorAngle(const FrameData& testBoneData, const FrameData& centerBone)
{
	float numerator = testBoneData.translation.Dot(centerBone.translation);
	float denominator = testBoneData.translation.Magnitude() * centerBone.translation.Magnitude();
	float resultAngle = Math::RadiansToDegrees(Math::Acos(numerator / denominator));
	return resultAngle;
}

static bool IsFrameWithinTolerence(const Frame& newFrame, const Frame& actualFrame, float error, float& cumulativeError)
{
	Assert(newFrame.bonesPerFrame.size() == actualFrame.bonesPerFrame.size());
	if (error >= CalculateErrorAngle(newFrame.bonesPerFrame[0], actualFrame.bonesPerFrame[0]))
	{
		const FrameData& testCenter = newFrame.bonesPerFrame[0];
		const FrameData& actualCenter = actualFrame.bonesPerFrame[0];
		// Loop through and calculate the error angles for each bone using the root
		for (uint32 i = 1; i < actualFrame.bonesPerFrame.size(); ++i)
		{
			float testAngle = CalculateErrorAngle(newFrame.bonesPerFrame[i], testCenter);
			float actualAngle = CalculateErrorAngle(actualFrame.bonesPerFrame[i], actualCenter);
			float diffAngle = Math::Abs(testAngle - actualAngle);
			cumulativeError += diffAngle;
			if (cumulativeError > error)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

static Frame InterpolateFrame(const Frame& srcFrame, const Frame& tarFrame, uint32 frameTime)
{
	Assert(srcFrame.bonesPerFrame.size() == tarFrame.bonesPerFrame.size());
	Frame resultingFrame;
	resultingFrame.frameTime = frameTime;
	float deltaTime = static_cast<float>(frameTime - srcFrame.frameTime) / static_cast<float>(tarFrame.frameTime - srcFrame.frameTime);

	uint32 boneCount = srcFrame.bonesPerFrame.size();
	for (uint32 i = 0; i < boneCount; ++i)
	{
		FrameData boneData;
		Math::Lerp(boneData.translation, srcFrame.bonesPerFrame[i].translation, tarFrame.bonesPerFrame[i].translation, deltaTime);
		Math::Slerp(boneData.rotation, srcFrame.bonesPerFrame[i].rotation, tarFrame.bonesPerFrame[i].rotation, deltaTime);
		Math::Lerp(boneData.scale, srcFrame.bonesPerFrame[i].scale, tarFrame.bonesPerFrame[i].scale, deltaTime);
		resultingFrame.bonesPerFrame.push_back(boneData);
	}

	return resultingFrame;
}

static bool IsFrameRangeWithinTolerence(const AnimationClip& animation, uint32 startIndex, uint32 endIndex, float error)
{
	float cumulative = 0.f;
	for (uint32 i = startIndex; i < endIndex; ++i)
	{
		Frame newFrame = InterpolateFrame(animation.frames[startIndex - 1], animation.frames[endIndex], animation.frames[i].frameTime);
		if (!IsFrameWithinTolerence(newFrame, animation.frames[i], error, cumulative))
		{
			return false;
		}
	}

	return true;
}

static AnimationClip ProcessCompression(const AnimationClip& animation, float error)
{
	AnimationClip newClip;
	newClip.name = animation.name;
	newClip.totalTime = animation.totalTime;

	for (uint32 i = 0; i < animation.frames.size(); ++i)
	{
		newClip.frames.push_back(animation.frames[i]);
		// Test each frame against the actual frame to get their validity
		for (uint32 j = i + 2; j < animation.frames.size(); ++j)
		{
			if (!IsFrameRangeWithinTolerence(animation, i+1, j, error))
			{
				// i gets advanced in outer for loop. Needs to be one less than the value it needs to be
				i = j - 2;
				break;
			}
		}
	}

	return newClip;
}

void CompressAnimations(std::vector<AnimationClip>& animations, float error)
{
	for (auto& animation : animations)
	{
		animation = ProcessCompression(animation, error);
	}
}


//////////////////////////////////////////////////////////////////////////
// Process Animations
//////////////////////////////////////////////////////////////////////////

void ProcessAnimation(FbxScene* scene, std::vector<AnimationDescription>& animations)
{
	for (int32 i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); ++i)
	{
		FbxAnimStack* stack = scene->GetSrcObject<FbxAnimStack>(i);
		scene->SetCurrentAnimationStack(stack);
		printf("Animation Name: %s\n", stack->GetName());
		AnimationDescription description = ProcessAnimationStack(stack, scene->GetRootNode());
		description.name = stack->GetName();

		if (!description.bones.empty())
		{
			animations.push_back(std::move(description));
		}
	}
}

static uint32 FindMostKeyFrames(FbxNode* node, FbxAnimLayer* layer, uint32 maxFrames = 0)
{
	int keyCount = 0;
	int countComp;
	FbxAnimCurve* pCurve = node->LclTranslation.GetCurve(layer);

	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr != nullptr)
	{
		if (attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			if (pCurve != nullptr)
			{
				countComp = pCurve->KeyGetCount();
				if (countComp > keyCount)
				{
					keyCount = countComp;//pCurve->KeyGetCount();
				}
			}

			pCurve = node->LclRotation.GetCurve(layer);
			if (pCurve != nullptr)
			{
				countComp = pCurve->KeyGetCount();
				if (countComp > keyCount)
				{
					keyCount = countComp;//pCurve->KeyGetCount();
				}
			}

			pCurve = node->LclScaling.GetCurve(layer);
			if (pCurve != nullptr)
			{
				countComp = pCurve->KeyGetCount();
				if (countComp > keyCount)
				{
					keyCount = pCurve->KeyGetCount();
				}
			}
		}
	}

	uint32 frames = keyCount;
	if (frames < maxFrames)
	{
		frames = maxFrames;
	}

	for (int i = 0; i < node->GetChildCount(); ++i)
	{

		frames = FindMostKeyFrames(node->GetChild(i), layer, frames);
	}

	return frames;
}

AnimationDescription ProcessAnimationStack(FbxAnimStack* stack, FbxNode* rootNode)
{
	AnimationDescription desc;
	if (stack->GetMemberCount<FbxAnimLayer>() > 0)
	{
		int32 count = static_cast<int32>(stack->GetLocalTimeSpan().GetStop().GetFrameCount(FbxTime::eFrames30));
		printf("Frame count? %d \n", count);
		FbxAnimLayer* layer = stack->GetMember<FbxAnimLayer>(0);
		Assert(layer != nullptr);
		uint32 maxFrames = count;//FindMostKeyFrames(rootNode, layer);

		desc = ProcessAnimationLayer(rootNode, layer, maxFrames);
	}

	return desc;
	
}

AnimationDescription ProcessAnimationLayer(FbxNode* rootNode, FbxAnimLayer* layer, uint32 frameCount)
{
	std::vector<BoneData> boneData;
	for (int32 i = 0; i < rootNode->GetChildCount(); ++i)
	{
		if (rootNode->GetChild(i)->GetChildCount())
		{
			ExtractBoneData(rootNode->GetChild(i), layer, boneData, frameCount);
			break;
		}
	}

	// Create animation description
	AnimationDescription description;
	description.bones = std::move(boneData);

	return description;

}

static BoneData RetrieveBoneCurveData(FbxNode* node, FbxAnimLayer* layer, uint32 frameCount)
{
	BoneData bone;
	bone.name = node->GetName();
	uint32 maxTime = 0;

	printf("Node name: %s\n", node->GetName());

	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if (attr == nullptr)
	{
		printf("Node %s has a null attribute... \n\n", node->GetName());
	}
	else
	{
		if (attr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			FbxAnimCurve* curve = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
			for (uint32 i = 0; i < frameCount; ++i)
			{

				printf("Number of frames: %u\n", frameCount);

				FbxTime keyTime;
				keyTime.SetTime(0, 0, 0, i, 0, 0, FbxTime::eFrames30);

				uint32 timeMS = static_cast<uint32>(keyTime.GetMilliSeconds());
				bone.timeMSPerKey.push_back(timeMS);
				if (maxTime < timeMS)
				{
					maxTime = timeMS;
				}

				if (curve != nullptr)
				{
					FbxAMatrix  matrix = node->EvaluateLocalTransform(keyTime);

					FbxVector4 Trans = matrix.GetT();
					FbxVector4 RotXYZ = matrix.GetR();
					FbxVector4 Scale = matrix.GetS();

					Vector4 trans = Vector4(
						static_cast<float>(Trans[0]),
						static_cast<float>(Trans[1]),
						static_cast<float>(Trans[2])
					);

					Quat rot = Quat(
						ROT_XYZ,
						Math::DegreesToRadians(static_cast<float>(RotXYZ[0])),
						Math::DegreesToRadians(static_cast<float>(RotXYZ[1])),
						Math::DegreesToRadians(static_cast<float>(RotXYZ[2]))
					);

					Vector4 scale = Vector4(
						static_cast<float>(Scale[0]),
						static_cast<float>(Scale[1]),
						static_cast<float>(Scale[2])
					);

					bone.translationKeys.push_back(trans);
					bone.rotationKeys.push_back(rot);
					bone.scaleKeys.push_back(scale);

					printf("Translation: x(%f) y(%f) z(%f)\nRotation: x(%f) y(%f) z(%f) w(%f)\nScale x(%f) y(%f) z(%f)\n\n",
						trans.x, trans.y, trans.z,
						rot.x, rot.y, rot.z, rot.w,
						scale.x, scale.y, scale.z
					);
				}
				else
				{
					FbxAMatrix  matrix = node->EvaluateLocalTransform();
					FbxVector4 Trans = matrix.GetT();
					FbxVector4 RotXYZ = matrix.GetR();
					FbxVector4 Scale = matrix.GetS();

					Vector4 trans = Vector4(
						static_cast<float>(Trans[0]),
						static_cast<float>(Trans[1]),
						static_cast<float>(Trans[2])
					);

					Quat rot = Quat(
						ROT_XYZ,
						Math::DegreesToRadians(static_cast<float>(RotXYZ[0])),
						Math::DegreesToRadians(static_cast<float>(RotXYZ[1])),
						Math::DegreesToRadians(static_cast<float>(RotXYZ[2]))
					);

					Vector4 scale = Vector4(
						static_cast<float>(Scale[0]),
						static_cast<float>(Scale[1]),
						static_cast<float>(Scale[2])
					);

					printf("Translation: x(%f) y(%f) z(%f)\nRotation: x(%f) y(%f) z(%f) w(%f)\nScale x(%f) y(%f) z(%f)\n\n",
						trans.x, trans.y, trans.z,
						rot.x, rot.y, rot.z, rot.w,
						scale.x, scale.y, scale.z
					);

					bone.translationKeys.push_back(trans);
					bone.rotationKeys.push_back(rot);
					bone.scaleKeys.push_back(scale);
				}

			}

		}
	}
	
	bone.totalAnimTimeMS = maxTime;
	return bone;
}

void ExtractBoneData(FbxNode* boneNode, FbxAnimLayer* layer, std::vector<BoneData>& bones, uint32 frameCount)
{
	// Get Data out...
	BoneData bone = RetrieveBoneCurveData(boneNode, layer, frameCount);
	if (!bone.rotationKeys.empty())
	{
		bones.push_back(bone);
	}

	for (int32 i = 0; i < boneNode->GetChildCount(); ++i)
	{
		ExtractBoneData(boneNode->GetChild(i), layer, bones, frameCount);
	}
}


//////////////////////////////////////////////////////////////////////////
// Process Mesh
//////////////////////////////////////////////////////////////////////////
void ProcessMesh(FbxNode* node, const std::vector<Hierarchy>& bones, std::vector<VertexWithFace>& vertList, std::vector<SkinData>& skinData, ModelData& modelData)
{
	FbxNodeAttribute* attrib = node->GetNodeAttribute();
	if (attrib != nullptr)
	{
		FbxNodeAttribute::EType attributeType = attrib->GetAttributeType();

		if (attributeType == FbxNodeAttribute::eMesh)
		{
			// TODO - allow for multiple meshes to be exported through this converter
			FbxMesh* mesh = node->GetMesh();
			int32 polygonCount = mesh->GetPolygonCount();
			FbxVector4* controlPoints = mesh->GetControlPoints();

			if (bones.size() > 0)
			{
				skinData = ProcessSkinningData(bones, mesh);
			}


			modelData.faces.Reserve(polygonCount);
			int32 vertexId = 0;
			Face face;
			for (int32 i = 0; i < polygonCount; ++i)
			{
				int32 polygonSize = mesh->GetPolygonSize(i);
				Assert(polygonSize == 3);
				for (int32 j = 0; j < polygonSize; ++j)
				{
					int32 controlPointIndex = mesh->GetPolygonVertex(i, j);

					VertexWithFace vert;
					vert.v.x = static_cast<float>(controlPoints[controlPointIndex][0]);
					vert.v.y = static_cast<float>(controlPoints[controlPointIndex][1]);
					vert.v.z = static_cast<float>(controlPoints[controlPointIndex][2]);

					vert.weightIndex = controlPointIndex;

					// Finding UVs
					for (int32 k = 0; k < mesh->GetElementUVCount(); ++k)
					{
						FbxGeometryElementUV* uv = mesh->GetElementUV(k);
						switch (uv->GetMappingMode())
						{
							case FbxGeometryElement::eByControlPoint:
							{
								switch (uv->GetReferenceMode())
								{
									case FbxGeometryElement::eDirect:
									{
										vert.v.u = static_cast<float>(uv->GetDirectArray().GetAt(controlPointIndex)[0]);
										vert.v.v = static_cast<float>(uv->GetDirectArray().GetAt(controlPointIndex)[1]);
									}break;
									case FbxGeometryElement::eIndexToDirect:
									{
										int32 id = uv->GetIndexArray().GetAt(controlPointIndex);
										vert.v.u = static_cast<float>(uv->GetDirectArray().GetAt(id)[0]);
										vert.v.v = static_cast<float>(uv->GetDirectArray().GetAt(id)[1]);
									}break;
									default:
									{
									}break;
								}
							}break;

							case FbxGeometryElement::eByPolygonVertex:
							{
								int32 texUVIndex = mesh->GetTextureUVIndex(i, j);
								switch (uv->GetReferenceMode())
								{
									case FbxGeometryElement::eDirect:
									case FbxGeometryElement::eIndexToDirect:
									{
										vert.v.u = static_cast<float>(uv->GetDirectArray().GetAt(texUVIndex)[0]);
										vert.v.v = static_cast<float>(uv->GetDirectArray().GetAt(texUVIndex)[1]);
									}break;
									default:
									{
									}break;
								}
							}break;

							case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
							case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
							case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
							default:
							{
							}break;
						}
					}

					for (int32 k = 0; k < mesh->GetElementNormalCount(); ++k)
					{
						FbxGeometryElementNormal* normal = mesh->GetElementNormal(k);

						if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
						{
							vert.v.nx = static_cast<float>(normal->GetDirectArray().GetAt(controlPointIndex)[0]);
							vert.v.ny = static_cast<float>(normal->GetDirectArray().GetAt(controlPointIndex)[1]);
							vert.v.nz = static_cast<float>(normal->GetDirectArray().GetAt(controlPointIndex)[2]);
						}
						else if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
						{
							switch (normal->GetReferenceMode())
							{
								case FbxGeometryElement::eDirect:
								{
									vert.v.nx = static_cast<float>(normal->GetDirectArray().GetAt(vertexId)[0]);
									vert.v.ny = static_cast<float>(normal->GetDirectArray().GetAt(vertexId)[1]);
									vert.v.nz = static_cast<float>(normal->GetDirectArray().GetAt(vertexId)[2]);
								}break;

								case FbxGeometryElement::eIndexToDirect:
								{
									int32 id = normal->GetIndexArray().GetAt(vertexId);
									vert.v.nx = static_cast<float>(normal->GetDirectArray().GetAt(id)[0]);
									vert.v.ny = static_cast<float>(normal->GetDirectArray().GetAt(id)[1]);
									vert.v.nz = static_cast<float>(normal->GetDirectArray().GetAt(id)[2]);
								}break;

								default:
								{
								}break;
							}
						}
					}

					// TODO - If tangents exist, then they need to be used from the fbx file and not generated!
					for (int32 k = 0; k < mesh->GetElementTangentCount(); ++i)
					{
						FbxGeometryElementTangent* tangent = mesh->GetElementTangent(k);

						if (tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
						{
							vert.v.nx = static_cast<float>(tangent->GetDirectArray().GetAt(controlPointIndex)[0]);
							vert.v.ny = static_cast<float>(tangent->GetDirectArray().GetAt(controlPointIndex)[1]);
							vert.v.nz = static_cast<float>(tangent->GetDirectArray().GetAt(controlPointIndex)[2]);
						}
						else if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
						{
							switch (tangent->GetReferenceMode())
							{
								case FbxGeometryElement::eDirect:
								{
									vert.v.nx = static_cast<float>(tangent->GetDirectArray().GetAt(vertexId)[0]);
									vert.v.ny = static_cast<float>(tangent->GetDirectArray().GetAt(vertexId)[1]);
									vert.v.nz = static_cast<float>(tangent->GetDirectArray().GetAt(vertexId)[2]);
								}break;
								case FbxGeometryElement::eIndexToDirect:
								{
									int32 id = tangent->GetIndexArray().GetAt(vertexId);
									vert.v.nx = static_cast<float>(tangent->GetDirectArray().GetAt(id)[0]);
									vert.v.ny = static_cast<float>(tangent->GetDirectArray().GetAt(id)[1]);
									vert.v.nz = static_cast<float>(tangent->GetDirectArray().GetAt(id)[2]);
								}
								break;
								default:
									break; // other reference modes not shown here!
							}
						}
					}

					if (j == 0)
					{
						face.v0 = vertexId;
					}
					else if (j == 1)
					{
						face.v1 = vertexId;
					}
					else if (j == 2)
					{
						face.v2 = vertexId;
					}
					vert.faceIndex = vertexId++;

					vertList.push_back(vert);
				}

				modelData.faces.Add(face);
			}


// 			DisplayMesh(node, vertList, modelData);
// 			FbxMesh* mesh = reinterpret_cast<FbxMesh*>(node->GetNodeAttribute());
// 			auto textures = GetTextureData(mesh);
// 			for (auto& tex : textures)
// 			{
// 				modelData.textures.Add(tex);
// 			}
		}

	}

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		ProcessMesh(node->GetChild(i), bones, vertList, skinData, modelData);
	}
}

std::vector<TexturePath> GetTextureData(FbxGeometry* pGeometry)
{
	Assert(pGeometry->GetNode() != nullptr);
	std::vector<TexturePath> textureData;
	int numMats = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int i = 0; i < numMats; ++i)
	{
		FbxSurfaceMaterial* mat = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(i);
		if (mat != nullptr)
		{
			for (int j = 0; j < FbxLayerElement::sTypeTextureCount; ++j)
			{
				FbxProperty property = mat->FindProperty(FbxLayerElement::sTextureChannelNames[j]);
				if (property.IsValid())
				{
					int textureCount = property.GetSrcObjectCount<FbxTexture>();
					for (int k = 0; k < textureCount; ++k)
					{
						FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(k);
						if (layeredTexture != nullptr)
						{
							// TODO - get something with layered textures in here
							Assert(false);
						}
						else
						{
							FbxTexture* texture = property.GetSrcObject<FbxTexture>(j);
							Assert(texture);

							TexturePath tex;
							
							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
							FbxProceduralTexture* proceduralTexture = FbxCast<FbxProceduralTexture>(texture);
							if (fileTexture)
							{
								const char* filePath = fileTexture->GetFileName();
								tex.textureFilePath = filePath;
							}
							else if (proceduralTexture)
							{
								// TODO - implement procedural texture parsing here
								Assert(false);
							}

							textureData.push_back(tex);
						}
					}
				}
			}
		}
	}

	return textureData;
}

static void NormalizeWeights(const std::vector<SkinData>& unnormalizedWeights, std::vector<VertexBoneWeights>& normalizedWeights)
{
	normalizedWeights.resize(unnormalizedWeights.size());
	for (uint32 i = 0; i < unnormalizedWeights.size(); ++i)
	{
		VertexBoneWeights weights = {};
		uint32 minSizeBoneList = std::min(weights.MaxWeightInfluences, 
			static_cast<uint32>(unnormalizedWeights[i].bonesWeights.size()));

		float sumWeightsBeforeNorm = 0.f;
		for (uint32 j = 0; j < minSizeBoneList; ++j)
		{
			auto& boneWeight = unnormalizedWeights[i].bonesWeights[j];
			weights.boneIndices[j] = boneWeight.first;
			weights.weights[j] = boneWeight.second;
			sumWeightsBeforeNorm += boneWeight.second;
		}

		float normalizedCheck = 0.f;
		for (auto& weight : weights.weights)
		{
			weight = weight / sumWeightsBeforeNorm;
			normalizedCheck += weight;
		}

		Assert(Math::IsEqual(normalizedCheck, 1.0f));

		normalizedWeights[i] = weights;
	}


	printf("\n---NORMALIZED WEIGHTS---\n\n");
	for (uint32 i = 0; i < normalizedWeights.size(); ++i)
	{
		printf("Control point[%u]: bone indices{ %u, %u, %u, %u } weights{ %f, %f, %f, %f }\n",
			i, normalizedWeights[i].boneIndices[0], normalizedWeights[i].boneIndices[1], normalizedWeights[i].boneIndices[2], normalizedWeights[i].boneIndices[3],
			normalizedWeights[i].weights[0], normalizedWeights[i].weights[1], normalizedWeights[i].weights[2], normalizedWeights[i].weights[3]
		);
	}
}

static void ReduceVertices(const std::vector<VertexWithFace>& sortedVerts, const std::vector<SkinData>& weights, ModelData& modelData)
{
	std::vector<VertexBoneWeights> normWeights;
	NormalizeWeights(weights, normWeights);

	unsigned int* indexData = reinterpret_cast<unsigned int*>(modelData.faces.GetData());
	
	for (int i = 0; i < (int)sortedVerts.size(); ++i)
	{
		VertexWithFace curr = sortedVerts[i];
		VertexWithFace comp;
		if (i != (int)sortedVerts.size() - 1)
		{
			comp = sortedVerts[i + 1];
		}


		if (normWeights.size() > 0)
		{
			modelData.weights.Add(normWeights[curr.weightIndex]);
		}

		if (curr.v == comp.v)
		{
			int index = i + 1;
			int correctVertIndex = modelData.vertices.Size();
			indexData[curr.faceIndex] = correctVertIndex;

			modelData.vertices.Add(curr.v);

			do
			{
				indexData[comp.faceIndex] = correctVertIndex;

				++index;
				if (index < (int)sortedVerts.size())
				{
					comp = sortedVerts[index];
				}
				else
				{
					break;
				}
			} while (curr.v == comp.v);

			i = index - 1;
		}
		else
		{
			int correctVertIndex = modelData.vertices.Size();
			indexData[curr.faceIndex] = correctVertIndex;
			modelData.vertices.Add(curr.v);
		}
	}
}

static void ChunkModel(const std::string& fileName)
{
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "vodka \"%s.model\" \"%s_model.chnk\" -t VERTS_TYPE -n \"%s\"",
		fileName.c_str(), fileName.c_str(), fileName.c_str());

	sprintf_s(systemCall, "vodka \"%s.model\" \"%s_model.chnk\" -t VERTS_TYPE -n \"%s\"",
		fileName.c_str(), fileName.c_str(), fileName.c_str());

	system(systemCall);
	system("del *.model");

	printf("Chunked Model %s\n", fileName.c_str());
}

static void ChunkWeights(const std::string& fileName)
{
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "vodka \"%s.wghts\" \"%s_weights.chnk\" -t WEIGHTS_TYPE -n \"%s\"",
		fileName.c_str(), fileName.c_str(), fileName.c_str());

	system(systemCall);
	system("del *.wghts");

	printf("Chunked weights for %s\n", fileName.c_str());
}

static void ChunkTexture(const std::string& fileName, uint32 textureIndex)
{
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "vodka \"%s.tex\" \"%s_t%d.chnk\" -t TEXTURE_TYPE -n \"%s_t%d\"",
		fileName.c_str(),
		fileName.c_str(), textureIndex,
		fileName.c_str(), textureIndex);

	system(systemCall);
	// TODO - This system delete isn't at all cross platform...
	system("del *.tex");

	// TODO - Think about having the texture names be the actual names of their files...
	printf("Chunked Texture #%d\n", textureIndex);
}

void ReduceAndSaveModelData(const std::vector<VertexWithFace>& verts, const std::vector<SkinData>& weights, ModelData& modelData)
{
	if (verts.size() > 0)
	{
		// Sort the list
		std::vector<VertexWithFace> vertsSorted(verts);

		printf("---UNSORTED VERTS WITH WEIGHT INDICES---\n");
		for (uint32 i = 0; i < vertsSorted.size(); ++i)
		{
			VertexWithFace& vert = vertsSorted[i];
			printf("Vertex: x(%f), y(%f), z(%f) - Weight Index: %u\n",
				vert.v.x, vert.v.y, vert.v.z, vert.weightIndex
			);
		}

		std::sort(vertsSorted.begin(), vertsSorted.end());

		ReduceVertices(vertsSorted, weights, modelData);

		GenerateMeshTangents(modelData.vertices, modelData.faces);

// 		for (uint32 i = 0; i < modelData.weights.size(); ++i)
// 		{
// 			VertexBoneWeights& weight = modelData.weights[i];
// 			printf("Weights for vertex %u: \n", i);
// 			printf("BIdx[%u] - %f, BIdx[%u] - %f, BIdx[%u] - %f, BIdx[%u] - %f\n\n",
// 				weight.boneIndices[0], weight.weights[0],
// 				weight.boneIndices[1], weight.weights[1],
// 				weight.boneIndices[2], weight.weights[2],
// 				weight.boneIndices[3], weight.weights[3]);
// 		}

		//Assert(modelData.vertices.size() == modelData.weights.size());

// 		printf("---FINAL VERTEX LIST---\n");
// 		for (uint32 i = 0; i < modelData.vertices.size(); ++i)
// 		{
// 			Vertex& vert = modelData.vertices[i];
// 			printf("Vertex[%u]: x(%f), y(%f), z(%f), nx(%f), ny(%f), nz(%f), u(%f), v(%f)\n",
// 				i, vert.x, vert.y, vert.z, vert.nx, vert.ny, vert.nz, vert.u, vert.v
// 			);
// 		}

		std::vector<Vector4> convertedPositions;
		convertedPositions.reserve(modelData.vertices.Size());
		for (const Vertex& vert : modelData.vertices)
		{
			convertedPositions.push_back(Vector4(vert.x, vert.y, vert.z));
		}

		SphereBounds boundingSphere;
		RitterSphere(boundingSphere, convertedPositions.data(), convertedPositions.size());

		// Save the data

		// TODO - Move saving to another function
		std::string file = modelData.modelName + ".model";

		File::Handle modelHandle;
		File::Result result = File::Open(modelHandle, file.c_str(), File::Mode::WRITE);
		Assert(result == File::Result::SUCCESS);

		ModelFileHeader modelHeader;
		modelHeader.boundingSphere = boundingSphere;
		modelHeader.numVerts = static_cast<uint32>(modelData.vertices.Size());
		modelHeader.numFaces = static_cast<uint32>(modelData.faces.Size());
		size_t len = modelData.modelName.length();
		len = len >= OBJECT_NAME_SIZE ? OBJECT_NAME_SIZE - 1 : len;
		modelData.modelName.copy(modelHeader.objName, len);

		result = File::Write(modelHandle, &modelHeader, sizeof(ModelFileHeader));
		Assert(result == File::Result::SUCCESS);

		File::Tell(modelHandle, modelHeader.vertBufferOffset);

		result = File::Write(modelHandle, modelData.vertices.GetData(), sizeof(Vertex) * modelData.vertices.Size());
		Assert(result == File::Result::SUCCESS);
		File::Tell(modelHandle, modelHeader.facesBufferOffset);

		result = File::Write(modelHandle, modelData.faces.GetData(), sizeof(Face) * modelData.faces.Size());
		Assert(result == File::Result::SUCCESS);

		File::Seek(modelHandle, File::Location::BEGIN, 0);
		result = File::Write(modelHandle, &modelHeader, sizeof(ModelFileHeader));
		Assert(result == File::Result::SUCCESS);

		result = File::Close(modelHandle);
		Assert(result == File::Result::SUCCESS);

		printf("Chunking Model Data... \n");
		ChunkModel(modelData.modelName);


		if (modelData.weights.Size() > 0)
		{
			// Chunk out the weights for the skin
			file = modelData.modelName + ".wghts";
			File::Handle weightsFile;
			result = File::Open(weightsFile, file.c_str(), File::Mode::WRITE);
			Assert(result == File::Result::SUCCESS);

			result = File::Write(weightsFile, modelData.weights.GetData(), sizeof(VertexBoneWeights) * modelData.weights.Size());
			Assert(result == File::Result::SUCCESS);

			result = File::Close(weightsFile);
			Assert(result == File::Result::SUCCESS);

			printf("Chunking Skin Data for mesh %s... \n", modelData.modelName.c_str());
			ChunkWeights(modelData.modelName);
		}


		// Write to texture file
		if (modelData.textures.Size() > 0)
		{
			printf("Chunking Textures... \n");
			for (uint32 i = 0; i < modelData.textures.Size(); ++i)
			{
				const TexturePath tex = modelData.textures[i];
				Path filePath(tex.textureFilePath.c_str());

				if (!filePath.DoesFileExist())
				{
					String texFile = filePath.GetFileName();
					filePath = modelData.directoryPath.c_str();
					filePath /= texFile;
					printf("%s\n", filePath.GetString());
					Assert(filePath.DoesFileExist());
				}
				String texFileName = filePath.GetFileNameWithoutExtension();

				char systemCall[SystemCallLength];
				if (compressionFormat != CompressionFormat::Invalid)
				{
					String formatStr = CompressionFormatString(compressionFormat);
					sprintf_s(systemCall, "TextureConverter -f \"%s\" -c %s",
						filePath.GetString(), *formatStr.GetLowerCase());
				}
				else
				{
					sprintf_s(systemCall, "TextureConverter -f \"%s\"",
						filePath.GetString());
				}
				system(systemCall);
				ChunkTexture(*texFileName, i);
			}
		}
	}
}

void PackModelData(const std::string& fileName)
{
	printf("Packing the Model Data...\n\n");
	char systemCall[SystemCallLength];

	sprintf_s(systemCall, "liu \"%s.pak\" -v v0.1 -n \"%s\"",
		fileName.c_str(), fileName.c_str());
	system(systemCall);

	system("del *.chnk");

	// TODO - find the actual move location within file system. Make this more intelligent
	std::string moveLocation = "../Assets/";
	moveLocation += "Models/";

	sprintf_s(systemCall, "move /y %s.pak %s%s.pak > nul",
		fileName.c_str(), moveLocation.c_str(), fileName.c_str());

	system(systemCall);

	printf("Moved Package to %s \n\n", moveLocation.c_str());
}
