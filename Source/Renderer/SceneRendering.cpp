
#include "Camera/Camera.h"
#include "DebugOutput.h"

#include "SceneRendering.h"
#include "Vulkan/VulkanMemory.h"
#include "Vulkan/VulkanCreateInfos.h"
#include "Vulkan/VulkanAbstractions.h"
#include "Vulkan/VulkanUtilites.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanQueue.h"
#include "Vulkan/VulkanCommandBuffer.h"
#include "Vulkan/VulkanPipeline.h"
#include "Vulkan/VulkanDescriptorSet.h"
#include "Vulkan/VulkanSwapchain.h"
#include "Vulkan/VulkanComputePipeline.h"
#include "Vulkan/VulkanShader.h"
#include "Vulkan/VulkanShaderManager.h"

#include "DrawList.h"
#include "Renderer.h"
#include "RenderingEntity.h"
#include "RenderingEntityManager.h"
#include "ComputeEntity.h"
#include "ComputeEntityManager.h"

#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Texture2D/Texture.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Shader/Material.h"
#include "Animation/Skeleton/SkeletonInstance.h"

#include "Archiver/SkeletonHeader.h"



void SceneRendering::RenderScene(Renderer* renderer_)
{
	renderer = renderer_;

	SetActiveCamera(CameraManager::GetActiveCamera());

	VulkanCommandBuffer* cmdBuffer = VulkanCommandBufferManager::GetActiveGraphicsBuffer();
	VulkanSwapchain* swapchain = renderer->GetSwapchain();
	cmdBuffer->BeginRenderpass(swapchain->GetActiveFramebuffer());

	// TODO - Research viewport normalization and if I should be doing that
	Viewport cameraViewport = activeCamera->GetCameraViewport();
	VkViewport viewport = { };
	viewport.width = static_cast<float>(cameraViewport.width);
	viewport.height = static_cast<float>(cameraViewport.height);
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;
	cmdBuffer->SetViewport(0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = {};
	scissor.extent = { (uint32)cameraViewport.width, (uint32)cameraViewport.height };
	cmdBuffer->SetScissor(0, 1, &scissor);

	Array<VulkanPipeline*> usedPipelines;

	// Per model:
	DrawList* drawList = renderer->GetDrawList();
	DrawList::Iterator it(drawList);
	for(; !it.AtEnd(); ++it)
	{
		// Lighting pass, determining light scene data for model

		// Model rendering, pushing everything forward to command buffer
		Model* model = it.Current();
		Material* mat = model->GetMaterial();

		// Contains all of the persistent data that will be given to the gpu
		RenderingEntity* entity = RenderingEntityManager::CreateOrGetEntity(renderer, model);

		// Transformation data
		TransformationUniformBuffer transformations;
		transformations.model = *model->GetWorld();
		transformations.view = activeCamera->GetView();
		transformations.projection = activeCamera->GetProjection();

		Array<BonePoseData> poseData = model->GetPoseData();
		if (poseData.Size() > 0)
		{
			PoseInverseUniformBuffer poseInverses;
			Memcpy(poseInverses.inverses, sizeof(PoseInverseUniformBuffer), poseData.GetData(), sizeof(BonePoseData) * poseData.Size());
			entity->GetPoseInverseBuffer()->UpdateUniforms(&poseInverses);
		}

/*		bool buildComputeBuffer = false;*/
		
		// Bind storage buffers by getting the list of compute entities

		
		ComputeEntity* computeEntity = nullptr;
		if(model->GetPoseData().Size() > 0)
		{
			computeEntity = &BuildComputeCommandBuffer(*model);
		}

		// Material Properties
		MaterialProperties props;
		props.diffuse = mat->GetColor();

		ViewProperties viewProps;
		viewProps.viewPosition = activeCamera->GetPosition();// *activeCamera->GetView();

		// Pushing the data to the buffers
		entity->GetTransformBuffer()->UpdateUniforms(&transformations);
		entity->GetMaterialPropsBuffer()->UpdateUniforms(&props);
		entity->GetViewPropertiesBuffer()->UpdateUniforms(&viewProps);

		// TODO - Get descriptor sets and update them based on information in the rendering entity
		VulkanPipeline* pipeline = mat->GetPipeline();

		VulkanDescriptorSet* descriptorSet;
		if (entity->GetDescriptorSet() == nullptr)
		{
			descriptorSet = pipeline->GetUnusedDescriptorSet();

			// Set Transform Data
			descriptorSet->SetUniformBufferInfo(entity->GetTransformBuffer()->GetBuffer(), 0);

			if (entity->GetNativeTexture()->image.handle != VK_NULL_HANDLE)
			{
				// Set Texture Data
				descriptorSet->SetSampledTextureInfo(*entity->GetNativeTexture(), 1);
			}

			// TODO - Get rid of hack!!!
			if (entity->GetNativeTexture1()->image.handle != VK_NULL_HANDLE)
			{
				// Set Texture Data
				descriptorSet->SetSampledTextureInfo(*entity->GetNativeTexture1(), 2);
				// Set Material Data
				descriptorSet->SetUniformBufferInfo(entity->GetMaterialPropsBuffer()->GetBuffer(), 3);
				// Set View Data
				descriptorSet->SetUniformBufferInfo(entity->GetViewPropertiesBuffer()->GetBuffer(), 4);
			}
			else
			{
				// Set Material Data
				descriptorSet->SetUniformBufferInfo(entity->GetMaterialPropsBuffer()->GetBuffer(), 2);
				// Set View Data
				descriptorSet->SetUniformBufferInfo(entity->GetViewPropertiesBuffer()->GetBuffer(), 3);
			}

			// SKINNING
			if (poseData.Size() > 0)
			{
				// Pose Inverses!
				descriptorSet->SetUniformBufferInfo(entity->GetPoseInverseBuffer()->GetBuffer(), 4);
				// Computed bone transforms
				descriptorSet->SetStorageBufferInfo(computeEntity->GetTransformedHierarchy().GetBuffer(), 5);
			}

			descriptorSet->UpdateDescriptorSet();

			entity->SetDescriptorSet(descriptorSet);
		}
		else
		{
			descriptorSet = entity->GetDescriptorSet();
		}


		// TODO - build graphics command buffer up, allowing for submission
		pipeline->Bind(cmdBuffer);
		pipeline->BindDescriptorSet(cmdBuffer, descriptorSet);
		entity->GetVertexBuffer()->Bind(cmdBuffer);
		if (entity->GetWeightsBuffer()->IsValid())
		{
			entity->GetWeightsBuffer()->Bind(cmdBuffer);
		}
		entity->GetIndexBuffer()->Bind(cmdBuffer);

		// Send draw command
		// TODO - move this call to the command buffer
		vkCmdDrawIndexed(cmdBuffer->GetNativeHandle(), entity->GetNumIndicies(), 1, 0, 0, 0);

		usedPipelines.Add(pipeline);
	}

	cmdBuffer->EndRenderPass();
	//cmdBuffer->End();

	// TODO - This needs to be cleaned up!!
	static uint32 shouldReset = 0;
	if (shouldReset == 1)
	{
		for (uint32 i = 0; i < usedPipelines.Size(); ++i)
		{
			// TODO - This might cause some problems down the line...
			usedPipelines[i]->ResetPipeline();
		}
		shouldReset = 0;
	}
	else
	{
		++shouldReset;
	}


	renderer = nullptr;
}

void SceneRendering::SetActiveCamera(Camera * camera)
{
	activeCamera = camera;
}

void SceneRendering::CreateComputePipelines()
{
	if (firstPassPipeline == nullptr)
	{
		CreateFirstPassPipeline();
		CreateBlendingPassPipeline();
		CreateProcessingPathPipeline();
		CreateFinalPassPipeline();
	}
}

void SceneRendering::CreateFirstPassPipeline()
{
	Array<uint8> compCode = LoadSPVShader("animFrameInterpolation.cs.spv");
	VulkanShader* compShader = VulkanShaderManager::CreateShader();
	compShader->Compile(compCode, Stage_Comp);

	VulkanDescriptorSetLayout* firstPassLayout = new VulkanDescriptorSetLayout(renderer->GetDevice());
	firstPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 0);
	firstPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1);
	firstPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 2);
	firstPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 3);
	firstPassLayout->BindLayout();

	VulkanPipelineLayout* layout = new VulkanPipelineLayout(renderer->GetDevice(), { firstPassLayout });

	firstPassPipeline = new VulkanComputePipeline(renderer->GetDevice());
	firstPassPipeline->Initialize(layout, compShader);
}

void SceneRendering::CreateBlendingPassPipeline()
{
	Array<uint8> compCode = LoadSPVShader("animationBlendClips.cs.spv");
	VulkanShader* compShader = VulkanShaderManager::CreateShader();
	compShader->Compile(compCode, Stage_Comp);

	VulkanDescriptorSetLayout* blendPassLayout = new VulkanDescriptorSetLayout(renderer->GetDevice());
	blendPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 0);
	blendPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1);
	blendPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 2);
	blendPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 3);

	blendPassLayout->BindLayout();

	VulkanPipelineLayout* layout = new VulkanPipelineLayout(renderer->GetDevice(), { blendPassLayout });

	blendingPassPipeline = new VulkanComputePipeline(renderer->GetDevice());
	blendingPassPipeline->Initialize(layout, compShader);
}

void SceneRendering::CreateProcessingPathPipeline()
{
	Array<uint8> compCode = LoadSPVShader("processLocalTransforms.cs.spv");
	VulkanShader* compShader = VulkanShaderManager::CreateShader();
	compShader->Compile(compCode, Stage_Comp);

	VulkanDescriptorSetLayout* processPassLayout = new VulkanDescriptorSetLayout(renderer->GetDevice());
	processPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 0);
	processPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1);
	processPassLayout->BindLayout();

	VulkanPipelineLayout* layout = new VulkanPipelineLayout(renderer->GetDevice(), { processPassLayout });

	processLocalsPassPipeline = new VulkanComputePipeline(renderer->GetDevice());
	processLocalsPassPipeline->Initialize(layout, compShader);
}

void SceneRendering::CreateFinalPassPipeline()
{
	Array<uint8> compCode = LoadSPVShader("animHierarchyCalculation.cs.spv");
	VulkanShader* compShader = VulkanShaderManager::CreateShader();
	compShader->Compile(compCode, Stage_Comp);

	VulkanDescriptorSetLayout* finalPassLayout = new VulkanDescriptorSetLayout(renderer->GetDevice());
	finalPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 0);
	finalPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 1);
	finalPassLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_COMPUTE_BIT, 2);
	finalPassLayout->BindLayout();

	VulkanPipelineLayout* layout = new VulkanPipelineLayout(renderer->GetDevice(), { finalPassLayout });

	finalPassPipeline = new VulkanComputePipeline(renderer->GetDevice());
	finalPassPipeline->Initialize(layout, compShader);
}

ComputeEntity& SceneRendering::BuildComputeCommandBuffer(Model& model)
{
	ComputeEntity* entity = ComputeEntityManager::CreateOrGetEntity(model);

	CreateComputePipelines();

	// Entity update buffers
	KeyFrameRange frameRange = model.GetSkeleton()->GetFrameRange();
	entity->GetFirstFrameStorage().UpdateStorage(frameRange.frame0->boneFrameData);
	entity->GetSecondFrameStorage().UpdateStorage(frameRange.frame1->boneFrameData);

	KeyframeConstanceUniformBuffer constants;
	constants.boneCount = model.GetSkeleton()->GetBoneCount();
	constants.deltaTime = frameRange.deltaTime;
	entity->GetKeyframeConstantsBuffer().UpdateUniforms(&constants);

	if (model.GetSkeleton()->IsBlending())
	{
		KeyFrameRange blendRange = model.GetSkeleton()->GetBlendRange();
		entity->GetFirstBlendFrameStorage().UpdateStorage(blendRange.frame0->boneFrameData);
		entity->GetSecondBlendFrameStorage().UpdateStorage(blendRange.frame1->boneFrameData);
		constants.deltaTime = blendRange.deltaTime;
		entity->GetBlendConstantsBuffer().UpdateUniforms(&constants);

		constants.deltaTime = model.GetSkeleton()->GetBlendDelta();
		entity->GetBlendResultingConstantsBuffer().UpdateUniforms(&constants);
	}

	static bool initialized = false;
	if (!initialized)
	{
		// Plus 1 for the default identity matrix
		Matrix* boneIden = new Matrix[constants.boneCount + 1];
		for (uint32 i = 0; i < constants.boneCount + 1; ++i)
		{
			boneIden[i].Set(IDENTITY);
		}

		entity->GetBoneMatrixStorage().UpdateStorage(boneIden);
		delete[] boneIden;

		//initialized = true;
	}

	const BoneHierarchyTable& table = model.GetSkeleton()->GetHierarchyTable();
	entity->GetHierarchyTableBuffer().UpdateStorage(&table);

	// Printing out debug info
	{
		/*
		static bool doMatrixPrint = false;

		FrameData* frameData = reinterpret_cast<FrameData*>(entity->GetInterpolatedTransformsStorage().GetMappedData());
		Matrix* linkList = reinterpret_cast<Matrix*>(entity->GetBoneMatrixStorage().GetMappedData());
		//Matrix* matList = reinterpret_cast<Matrix*>(entity->GetTransformedHierarchy().GetMappedData());
		if (!doMatrixPrint)
		{
			Debug::Print("\n\n");
			BoneHierarchyTable* mappedTable = reinterpret_cast<BoneHierarchyTable*>(entity->GetHierarchyTableBuffer().GetMappedData());
			for (uint32 i = 0; i < constants.boneCount; ++i)
			{
				Debug::Printf("Bone %u: ", i);
				for (uint32 j = 0; j < BoneHierarchyTable::BoneTableElement::MaxElementLength; ++j)
				{
					Debug::Printf("Parent %d,", mappedTable->table[i].boneParentHierarchy[j]);
				}
				Debug::Print("\n");
			}
			Debug::Print("\n\n");
		}

		
		static int count = 0;
		if (doMatrixPrint && count < 5)
		{
			Debug::Print("Bone Local Matrix data\n");
			for (uint32 i = 0; i < constants.boneCount + 1; ++i)
			{
				Matrix* mat = &linkList[i];
				Debug::Printf("Bone %u's local matrix: \n", i);
				Debug::Printf("\t m0(%f),  m1(%f),  m2(%f),  m3(%f),\n", (*mat)[m0], (*mat)[m1], (*mat)[m2], (*mat)[m3]);
				Debug::Printf("\t m4(%f),  m5(%f),  m2(%f),  m7(%f),\n", (*mat)[m4], (*mat)[m5], (*mat)[m6], (*mat)[m7]);
				Debug::Printf("\t m8(%f),  m9(%f), m10(%f), m11(%f),\n", (*mat)[m8], (*mat)[m9], (*mat)[m10], (*mat)[m11]);
				Debug::Printf("\tm12(%f), m13(%f), m14(%f), m15(%f)\n\n", (*mat)[m12], (*mat)[m13], (*mat)[m14], (*mat)[m15]);
			}
			Debug::Print("\n\n");

			Debug::Print("Frame Local Bone Transforms\n");
			for(uint32 i = 0; i < constants.boneCount; ++i)
			{
				FrameData* data = &frameData[i];
				Debug::Printf("Bone %u's interpolated data\n", i);
				Debug::Printf("Translation: x(%f), y(%f), z(%f)\n", data->translation[x], data->translation[y], data->translation[z]);
				Debug::Printf("Rotation: x(%f), y(%f), z(%f), w(%f)\n", data->rotation[x], data->rotation[y], data->rotation[z], data->rotation[w]);
				Debug::Printf("Scale: x(%f), y(%f), z(%f)\n", data->scale[x], data->scale[y], data->scale[z]);
				Debug::Print("\n");
			}

// 			Debug::Print("Transformed Matrix data\n");
// 			for (uint32 i = 0; i < constants.boneCount; ++i)
// 			{
// 				Matrix* mat = &matList[i];
// 				Debug::Printf("Bone %u's transformed matrix: \n", i);
// 				Debug::Printf("\t m0(%f),  m1(%f),  m2(%f),  m3(%f)\n,", (*mat)[m0], (*mat)[m1], (*mat)[m2], (*mat)[m3]);
// 				Debug::Printf("\t m4(%f),  m5(%f),  m2(%f),  m7(%f)\n,", (*mat)[m4], (*mat)[m5], (*mat)[m6], (*mat)[m7]);
// 				Debug::Printf("\t m8(%f),  m9(%f), m10(%f), m11(%f)\n,", (*mat)[m8], (*mat)[m9], (*mat)[m10], (*mat)[m11]);
// 				Debug::Printf("\tm12(%f), m13(%f), m14(%f), m15(%f)\n\n", (*mat)[m12], (*mat)[m13], (*mat)[m14], (*mat)[m15]);
// 			}
// 			Debug::Print("\n\n");

			count++;
		}
		doMatrixPrint = true;
		//*/
	}

	if (entity->GetFirstPassDescriptorSet() == nullptr)
	{
		VulkanDescriptorSet* ds = firstPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetFirstFrameStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetSecondFrameStorage().GetBuffer(), 1);
		ds->SetUniformBufferInfo(entity->GetKeyframeConstantsBuffer().GetBuffer(), 2);
		ds->SetStorageBufferInfo(entity->GetInterpolatedTransformsStorage().GetBuffer(), 3);
		ds->UpdateDescriptorSet();
		entity->SetFirstPassDescriptorSet(*ds);

		ds = firstPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetFirstFrameStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetSecondFrameStorage().GetBuffer(), 1);
		ds->SetUniformBufferInfo(entity->GetKeyframeConstantsBuffer().GetBuffer(), 2);
		ds->SetStorageBufferInfo(entity->GetFirstPassBlendResultStorage().GetBuffer(), 3);
		ds->UpdateDescriptorSet();
		entity->SetFirstPassBlendDescriptor(*ds);

		ds = firstPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetFirstBlendFrameStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetSecondBlendFrameStorage().GetBuffer(), 1);
		ds->SetUniformBufferInfo(entity->GetBlendConstantsBuffer().GetBuffer(), 2);
		ds->SetStorageBufferInfo(entity->GetBlendTransformsBuffer().GetBuffer(), 3);
		ds->UpdateDescriptorSet();
		entity->SetBlendingDescriptorSet(*ds);

		ds = blendingPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetFirstPassBlendResultStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetBlendTransformsBuffer().GetBuffer(), 1);
		ds->SetUniformBufferInfo(entity->GetBlendResultingConstantsBuffer().GetBuffer(), 2);
		ds->SetStorageBufferInfo(entity->GetInterpolatedTransformsStorage().GetBuffer(), 3);
		ds->UpdateDescriptorSet();
		entity->SetBlendingResultDescriptorSet(*ds);

		ds = processLocalsPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetInterpolatedTransformsStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetBoneMatrixStorage().GetBuffer(), 1);
		ds->UpdateDescriptorSet();
		entity->SetProcessingPassDescriptorSet(*ds);

		ds = finalPassPipeline->GetUnusedDescriptorSet();
		ds->SetStorageBufferInfo(entity->GetBoneMatrixStorage().GetBuffer(), 0);
		ds->SetStorageBufferInfo(entity->GetHierarchyTableBuffer().GetBuffer(), 1);
		ds->SetStorageBufferInfo(entity->GetTransformedHierarchy().GetBuffer(), 2);
		ds->UpdateDescriptorSet();
		entity->SetFinalPassDescriptorSet(*ds);
	}

	VulkanCommandBuffer* computeBuffer = VulkanCommandBufferManager::GetActiveComputeBuffer();
	if (computeBuffer->GetBufferState() == VulkanCommandBuffer::State::Initialized)
	{
		computeBuffer->Begin();
	}

	VkBufferMemoryBarrier memBarrier = {};
	memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	memBarrier.buffer = entity->GetTransformedHierarchy().GetBuffer().handle;
	memBarrier.size = entity->GetTransformedHierarchy().GetBuffer().size;
	memBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	memBarrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	// Compute and graphics queue may have different queue families
	// For the barrier to work across different queues, we need to set their family indices
	memBarrier.srcQueueFamilyIndex = renderer->GetDevice()->GetGraphicsQueue()->GetFamilyIndex();
	memBarrier.dstQueueFamilyIndex = renderer->GetDevice()->GetComputeQueue()->GetFamilyIndex();

	computeBuffer->BufferMemoryBarrier(
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		0,
		1, &memBarrier
	);
	
	constexpr uint32 WorkGroupCount = 10;

	


	if (!model.GetSkeleton()->IsBlending())
	{
		firstPassPipeline->Bind(computeBuffer);
		firstPassPipeline->BindDescriptorSet(computeBuffer, entity->GetFirstPassDescriptorSet());

		computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

		memBarrier = {};
		memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		memBarrier.buffer = entity->GetInterpolatedTransformsStorage().GetBuffer().handle;
		memBarrier.size = entity->GetInterpolatedTransformsStorage().GetBuffer().size;
		memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		computeBuffer->BufferMemoryBarrier(
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0,
			1, &memBarrier
		);
	}
	else
	{
		{
			FrameData* frameData = reinterpret_cast<FrameData*>(entity->GetBlendTransformsBuffer().GetMappedData());
			Debug::Print("Blending Locals data\n");
			for (uint32 i = 0; i < constants.boneCount + 1; ++i)
			{
				FrameData* data = &frameData[i];
				Debug::Printf("Bone %u's interpolated data\n", i);
				Debug::Printf("Translation: x(%f), y(%f), z(%f)\n", data->translation.x, data->translation.y, data->translation.z);
				Debug::Printf("Rotation: x(%f), y(%f), z(%f), w(%f)\n", data->rotation.x, data->rotation.y, data->rotation.z, data->rotation.w);
				Debug::Printf("Scale: x(%f), y(%f), z(%f)\n", data->scale.x, data->scale.y, data->scale.z);
				Debug::Print("\n");
			}
			Debug::Print("\n\n");
		}

		firstPassPipeline->Bind(computeBuffer);
		firstPassPipeline->BindDescriptorSet(computeBuffer, entity->GetFirstPassBlendDescriptorSet());

		computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

		// I think this is unnecessary
		firstPassPipeline->Bind(computeBuffer);
		firstPassPipeline->BindDescriptorSet(computeBuffer, entity->GetBlendingDescriptorSet());

		computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

		memBarrier = {};
		memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		memBarrier.buffer = entity->GetFirstPassBlendResultStorage().GetBuffer().handle;
		memBarrier.size = entity->GetFirstPassBlendResultStorage().GetBuffer().size;
		memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		computeBuffer->BufferMemoryBarrier(
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0,
			1, &memBarrier
		);

		memBarrier = {};
		memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		memBarrier.buffer = entity->GetBlendTransformsBuffer().GetBuffer().handle;
		memBarrier.size = entity->GetBlendTransformsBuffer().GetBuffer().size;
		memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		computeBuffer->BufferMemoryBarrier(
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0,
			1, &memBarrier
		);

		blendingPassPipeline->Bind(computeBuffer);
		blendingPassPipeline->BindDescriptorSet(computeBuffer, entity->GetBlendingResultDescriptorSet());

		computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

		memBarrier = {};
		memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		memBarrier.buffer = entity->GetInterpolatedTransformsStorage().GetBuffer().handle;
		memBarrier.size = entity->GetInterpolatedTransformsStorage().GetBuffer().size;
		memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		computeBuffer->BufferMemoryBarrier(
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0,
			1, &memBarrier
		);

	}


	memBarrier = {};
	memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	memBarrier.buffer = entity->GetInterpolatedTransformsStorage().GetBuffer().handle;
	memBarrier.size = entity->GetInterpolatedTransformsStorage().GetBuffer().size;
	memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	computeBuffer->BufferMemoryBarrier(
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		0,
		1, &memBarrier
	);


	processLocalsPassPipeline->Bind(computeBuffer);
	processLocalsPassPipeline->BindDescriptorSet(computeBuffer, entity->GetProcessingPassDescriptorSet());

	computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

	memBarrier = {};
	memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	memBarrier.buffer = entity->GetBoneMatrixStorage().GetBuffer().handle;
	memBarrier.size = entity->GetBoneMatrixStorage().GetBuffer().size;
	memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	memBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	memBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	computeBuffer->BufferMemoryBarrier(
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		0,
		1, &memBarrier
	);


	//If blending??

	finalPassPipeline->Bind(computeBuffer);
	finalPassPipeline->BindDescriptorSet(computeBuffer, entity->GetFinalPassDescriptorSet());

	computeBuffer->Dispatch(model.GetSkeleton()->GetBoneCount() / WorkGroupCount, 1, 1);

	memBarrier = {};
	memBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	memBarrier.buffer = entity->GetTransformedHierarchy().GetBuffer().handle;
	memBarrier.size = entity->GetTransformedHierarchy().GetBuffer().size;
	memBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
	memBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	// Compute and graphics queue may have different queue families
	// For the barrier to work across different queues, we need to set their family indices
	memBarrier.srcQueueFamilyIndex = renderer->GetDevice()->GetComputeQueue()->GetFamilyIndex();
	memBarrier.dstQueueFamilyIndex = renderer->GetDevice()->GetGraphicsQueue()->GetFamilyIndex();

	computeBuffer->BufferMemoryBarrier(
		VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
		VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
		0,
		1, &memBarrier
	);

	return *entity;
}
