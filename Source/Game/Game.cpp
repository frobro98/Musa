#include "Game/Game.h"
#include "GameObject/GameObjectManager.h"
#include "Shader/Material.h"
#include "Renderer/RenderingEngine.h"
#include "Texture2D/Texture.h"
#include "Texture2D/TextureManager.h"
#include "Model/Model.h"
#include "Model/ModelFactory.h"
#include "Model/MeshManager.h"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Input/Input.hpp"
#include "DebugOutput.h"
#include "Archiver/ArchiveQuery.h"
#include "Model/Mesh.h"
#include "Animation/Skeleton/SkeletonManager.h"
#include "Animation/Skeleton/Skeleton.h"
#include "Animation/AnimationClipFactory.h"
#include "GameObject/AnimationGameObject.h"
#include "Archiver/SkeletonHeader.h"

#include "DirectoryLocations.h"
#include "String/String.h"

#include "GameObject/DemoGameObjects/CameraObject.h"

#include "ECS/Entity.hpp"
#include "ECS/Components/TransformComponent.hpp"
#include "ECS/Components/CameraComponent.hpp"
#include "ECS/Components/RenderComponent.hpp"

#include "ECS/Systems/CameraSystem.hpp"
#include "ECS/Systems/TransformSystem.hpp"
#include "ECS/Systems/RenderSystem.hpp"

#include "ECS/SystemsManager.hpp"

using namespace Musa;

void InitializeMainCamera()
{
	CameraObject* cam = new CameraObject("Main Camera");
	GetGameObjectManager().Add(cam);
	cam->SetupGodCam();

	CameraManager::SetActiveCamera("Main Camera");
}

void Game::Init()
{
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	Viewport viewport = { 0, 0, width, height };

	Camera* camera = new Camera;
	camera->SetViewport(viewport);
	camera->SetPerspective(45.f, aspect, .1f, 10000.f);
	camera->SetOrientationAndPosition(Vector(0/*1000*/, 0/*1000*/, 0), Vector(0, 0.f, 155.f), Vector(0, 1, 0));

	// TODO - Make this more of a factory than a camera set up then add sort of thing
	CameraManager::AddCamera(camera, "Main Camera");
	CameraManager::SetActiveCamera("Main Camera");


	TransformSystem* transSys = CreateSystem<TransformSystem>();
	CameraSystem* cameraSystem = CreateSystem<CameraSystem>();
	RenderSystem* renderingSystem = CreateSystem<RenderSystem>();

	GetSystemsManager().AddSystem(*transSys);
	GetSystemsManager().AddSystemDependentOn(*cameraSystem, *transSys);
	GetSystemsManager().AddSystemDependentOn(*renderingSystem, *cameraSystem);
}

void Game::LoadContent()
{
// 	TransformComponent* transformComp = CreateComponent<TransformComponent>();
// 	CameraComponent* cameraComp = CreateComponent<CameraComponent>();
// 	RenderComponent* renderComp = CreateComponent<RenderComponent>();

	Entity* entity = CreateEntity("Test entity");
	AttachComponentsTo<TransformComponent, CameraComponent, RenderComponent>(*entity);
	CameraComponent* comp = Musa::GetComponentFrom<CameraComponent>(*entity);
	TransformComponent* trans = Musa::GetComponentFrom<TransformComponent>(*entity);
	
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	Viewport viewport = { 0, 0, width, height };

	comp->aspectRatio = aspect;
	comp->viewport = viewport;
	comp->fov = 45.f;
	comp->nearPlane = .1f;
	comp->farPlane = 10000.f;
	comp->lookAt = Vector();
	trans->SetPosition(Vector(0, 0, 155));

	CompileShaders();
	PreparePipelines();

// 	{
// 		Path texturePath(EngineTexturePath() + "phoenix_FFT.tex");
// 		TextureManager::LoadTextureFromFile(texturePath, "redBrickFFT");
// 	}
// 
// 	{
// 		Path texturePath(EngineTexturePath() + "phoenix_Box.tex");
// 		TextureManager::LoadTextureFromFile(texturePath, "redBrickBox");
// 	}
// 
// 	{
// 		Path texturePath(EngineTexturePath() + "phoenix_Kaiser.tex");
// 		TextureManager::LoadTextureFromFile(texturePath, "redBrickKaiser");
// 	}

	{
		Path texturePath(EngineTexturePath() + "RedBrick.tex");
		TextureManager::LoadTextureFromFile(texturePath, "redBrick");
	}

	{
		Path texturePath(EngineTexturePath() + "default.tex");
		TextureManager::LoadTextureFromFile(texturePath, "defaultTex");
	}

	{
		Path texturePath(EngineTexturePath() + "defaultBC1.tex");
		TextureManager::LoadTextureFromFile(texturePath, "defaultBC1");
	}

	{
		Path texturePath(EngineTexturePath() + "defaultBC3.tex");
		TextureManager::LoadTextureFromFile(texturePath, "defaultBC3");
	}

	{
		Path texturePath(EngineTexturePath() + "defaultBC7.tex");
		TextureManager::LoadTextureFromFile(texturePath, "defaultBC7");
	}

//	ProcessPackages();

	//MeshManager::LoadPrimitive(Primitive::Box);
	MeshManager::LoadPrimitive(Primitive::Plane);
	MeshManager::LoadPrimitive(Primitive::Sphere);
	MeshManager::LoadPrimitive(Primitive::Pyramid);

	CompressionTestScene();
	//MipmappingDemo();
	//TintedMipDemo();
}

void Game::UpdateGame([[maybe_unused]] float time)
{
}

void Game::UnloadContent()
{
}

void Game::DeInit()
{
}

void Game::CompressionTestScene()
{
	InitializeMainCamera();

	Mesh* plane = MeshManager::FindMesh(Mesh::PlaneName);
	Material* defaultMat = new Material("defaultPipeline", "defaultTex", Color::Magenta());
	Material* defaultBC1Mat = new Material("defaultPipeline", "defaultBC1", Color(1, 1, 1, 1));
	Material* defaultBC3Mat = new Material("defaultPipeline", "defaultBC3", Color(1, 1, 1, 1));
	Material* defaultBC7Mat = new Material("defaultPipeline", "defaultBC7", Color(1, 1, 1, 1));

	GameObject* planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, defaultMat));
	planeObject->SetPos(-105, 0, 0);
	planeObject->SetScale(50, 50, 50);

	planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, defaultBC1Mat));
	planeObject->SetPos(-35, 0, 0);
	planeObject->SetScale(50, 50, 50);

	planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, defaultBC3Mat));
	planeObject->SetPos(35, 0, 0);
	planeObject->SetScale(50, 50, 50);

	planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, defaultBC7Mat));
	planeObject->SetPos(105, 0.f, 0);
	planeObject->SetScale(50, 50, 50);
}

void Game::MipmappingDemo()
{
	InitializeMainCamera();

	Mesh* plane = MeshManager::FindMesh(Mesh::PlaneName);
	Material* fftMat = new Material("defaultPipeline", "redBrickFFT", Color::Magenta());
	Material* boxMat = new Material("defaultPipeline", "redBrickBox", Color::Magenta());
	Material* kaiserMat = new Material("defaultPipeline", "redBrickKaiser", Color::Magenta());

	GameObject* planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, fftMat));
	planeObject->SetScale(50, 50, 50);

	planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, boxMat));
	planeObject->SetScale(50, 50, 50);
	planeObject->SetPos(Vector(-75, 0, 0));

	planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, kaiserMat));
	planeObject->SetScale(50, 50, 50);
	planeObject->SetPos(Vector(75, 0, 0));
}

void Game::TintedMipDemo()
{
	InitializeMainCamera();

	Mesh* plane = MeshManager::FindMesh(Mesh::PlaneName);
	Material* fftMat = new Material("defaultPipeline", "redBrick", Color::Magenta());

	GameObject* planeObject = GetGameObjectManager().CreateAndAdd<GameObject>();
	planeObject->SetModel(ModelFactory::CreateModel(plane, fftMat));
	planeObject->SetScale(50, 50, 50);
}

void Game::ModelTestScene()
{
	Mesh* plane = MeshManager::FindMesh(Mesh::PlaneName);
	//Mesh* spaceship = MeshManager::FindMesh("SpaceShip");
	Material* spaceshipMat = new Material("defaultPipeline", "space_frigate_t0", Color(1, 1, 1, 1));
	Material* spaceshipBC1Mat = new Material("defaultPipeline", "space_frigate_bc1", Color(1, 1, 1, 1));
	Material* spaceshipBC3Mat = new Material("defaultPipeline", "space_frigate_bc3", Color(1, 1, 1, 1));
	Material* spaceshipBC7Mat = new Material("defaultPipeline", "space_frigate_bc7", Color(1, 1, 1, 1));

	GameObject* ship = GetGameObjectManager().CreateAndAdd<GameObject>();
	ship->SetModel(ModelFactory::CreateModel(plane, spaceshipMat));
	ship->SetPos(-105, 0, 0);
	ship->SetScale(50, 50, 50);

	ship = GetGameObjectManager().CreateAndAdd<GameObject>();
	ship->SetModel(ModelFactory::CreateModel(plane, spaceshipBC1Mat));
	ship->SetPos(-35, 0, 0);
	ship->SetScale(50, 50, 50);

	ship = GetGameObjectManager().CreateAndAdd<GameObject>();
	ship->SetModel(ModelFactory::CreateModel(plane, spaceshipBC3Mat));
	ship->SetPos(35, 0, 0);
	ship->SetScale(50, 50, 50);

	ship = GetGameObjectManager().CreateAndAdd<GameObject>();
	ship->SetModel(ModelFactory::CreateModel(plane, spaceshipBC7Mat));
	ship->SetPos(105, 0.f, 0);
	ship->SetScale(50, 50, 50);

}

void Game::AnimationTestScene()
{
	uint8* chunkData = nullptr;
	uint32 chunkSize, hash;
	Skeleton* humanoidSkel = nullptr;
	Skeleton* teddySkel = nullptr;

	String assetPath = EngineAssetPath();

	// Loading the Humanoid
	//*
	assetPath += "Models/humanoid_anim_nocompress.pak";
	if (ProcessPackage(*assetPath, Chunk::SKEL_TYPE, "humanoid_Skel", chunkData, chunkSize, hash))
	{
		humanoidSkel = SkeletonManager::CreateSkeleton(chunkData, hash);
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::ANIM_TYPE, "shot_anim", chunkData, chunkSize, hash))
	{
		assert(humanoidSkel != nullptr);
		AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(chunkData);
		humanoidSkel->AddAnimation(clip);
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::ANIM_TYPE, "punch_anim", chunkData, chunkSize, hash))
	{
		assert(humanoidSkel != nullptr);
		AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(chunkData);
		humanoidSkel->AddAnimation(clip);
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::ANIM_TYPE, "run_anim", chunkData, chunkSize, hash))
	{
		assert(humanoidSkel != nullptr);
		AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(chunkData);
		humanoidSkel->AddAnimation(clip);
	}
	delete[] chunkData;

	assetPath = EngineAssetPath();
	assetPath += "Models/humanoid.pak";
	if (ProcessPackage(*assetPath, Chunk::VERTS_TYPE, "humanoid", chunkData, chunkSize, hash))
	{
		uint8* weightData;
		uint32 weightChunkSize, weightHash;
		if (ProcessPackage(*assetPath, Chunk::WEIGHTS_TYPE, "humanoid", weightData, weightChunkSize, weightHash))
		{
			MeshManager::LoadMeshFromPak(chunkData, weightData, "Humanoid");
			delete[] weightData;
		}
		else
		{
			MeshManager::LoadMeshFromPak(chunkData, "Humanoid");
		}
	}
	delete[] chunkData;

	//*/

	// Loading the teddy
	//*
	assetPath = EngineAssetPath();
	assetPath += "Models/teddy_TGA_anim.pak";
	if (ProcessPackage(*assetPath, Chunk::SKEL_TYPE, "teddy_TGA_Skel", chunkData, chunkSize, hash))
	{
		teddySkel = SkeletonManager::CreateSkeleton(chunkData, hash);
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::ANIM_TYPE, "idle_anim", chunkData, chunkSize, hash))
	{
		assert(teddySkel != nullptr);
		AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(chunkData);
		teddySkel->AddAnimation(clip);
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::ANIM_TYPE, "walk_anim", chunkData, chunkSize, hash))
	{
		assert(teddySkel != nullptr);
		AnimationClip* clip = AnimationClipFactory::CreateAnimationClip(chunkData);
		teddySkel->AddAnimation(clip);
	}
	delete[] chunkData;

	assetPath = EngineAssetPath();
	assetPath += "Models/teddy_TGA.pak";
	if (ProcessPackage(*assetPath, Chunk::VERTS_TYPE, "teddy_TGA", chunkData, chunkSize, hash))
	{
		uint8* weightData;
		uint32 weightChunkSize, weightHash;
		if (ProcessPackage(*assetPath, Chunk::WEIGHTS_TYPE, "teddy_TGA", weightData, weightChunkSize, weightHash))
		{
			MeshManager::LoadMeshFromPak(chunkData, weightData, "Teddy");
			delete[] weightData;
		}
		else
		{
			MeshManager::LoadMeshFromPak(chunkData, "Teddy");
		}
	}
	delete[] chunkData;

	//*/


	AnimationGameObject* go = new AnimationGameObject;
	go->SetModel(ModelFactory::CreateModel(MeshManager::FindMesh("Humanoid"), new Material("SkinningPipeline", nullptr, Color{ 1, 1, 1, 1 })));
	GetGameObjectManager().Add(go);

	Quat rot(ROT_XYZ, -Math::PiOver2, 0, 0);
	go->SetWorld(rot);
	go->SetSkeleton(*humanoidSkel);
	go->SetupInputs();
	go->SetDebug(false);

	go = new AnimationGameObject;
	go->SetModel(ModelFactory::CreateModel(MeshManager::FindMesh("Teddy"), new Material("SkinningPipeline", nullptr, Color{ 1, 1, 1, 1 })));
	GetGameObjectManager().Add(go);

	go->SetSkeleton(*teddySkel);
	go->SetupInputs();
	go->SetDebug(false);
	go->SetCurrentClip(0);
}

void Game::ProcessPackages()
{
	String assetPath = EngineAssetPath();
	assetPath += "Models/astroboy.pak";

	uint8* chunkData = nullptr;
	uint32 chunkSize, hash;
	if (ProcessPackage(*assetPath, Chunk::VERTS_TYPE, "astroboy", chunkData, chunkSize, hash))
	{
		MeshManager::LoadMeshFromPak(chunkData, "AstroBoy");
	}
	delete[] chunkData;

	if (ProcessPackage(*assetPath, Chunk::TEXTURE_TYPE, "astroboy_t0", chunkData, chunkSize, hash))
	{
		Array<uint8> textureData(chunkData, chunkSize);
		TextureManager::LoadTexture(textureData, "astroboy_t0");
	}
	delete[] chunkData;


	assetPath = EngineAssetPath();
	assetPath += "Models/space_frigate.pak";
	if (ProcessPackage(*assetPath, Chunk::VERTS_TYPE, "space_frigate", chunkData, chunkSize, hash))
	{
		MeshManager::LoadMeshFromPak(chunkData, "SpaceShip");
	}
	delete[] chunkData;

}



// TODO - Get rid of vulkan being in my fucking game....
#include "Renderer/Vulkan/VulkanPipeline.h"
#include "Renderer/Vulkan/VulkanShader.h"
#include "Renderer/Vulkan/VulkanShaderManager.h"
#include "Renderer/Vulkan/VulkanRenderingInfo.h"
#include "Renderer/Vulkan/VulkanDescriptorSet.h"
#include "Renderer/PipelineManager.h"

void Game::PreparePipelines()
{
	// TODO - get rid of hack!
	VulkanDescriptorSetLayout* descriptorSetLayout = new VulkanDescriptorSetLayout(renderEngine->GetDevice());
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 2);
	descriptorSetLayout->BindLayout();

	pipelineLayout = new VulkanPipelineLayout(renderEngine->GetDevice(), { descriptorSetLayout });

	Array<uint8> vertCode = LoadSPVShader("constantColor.vs.spv");
	Array<uint8> fragCode = LoadSPVShader("constantColor.fs.spv");

	VulkanShader* vertShader = VulkanShaderManager::CreateShader();
	VulkanShader* fragShader = VulkanShaderManager::CreateShader();
	vertShader->Compile(vertCode, ShaderStage::Stage_Vert);
	fragShader->Compile(fragCode, ShaderStage::Stage_Frag);
	VulkanShaderManager::AddShader(vertShader, fragShader, "Wireframe");

	renderingInfo.shaders[Stage_Vert] = vertShader;
	renderingInfo.shaders[Stage_Frag] = fragShader;

	renderingInfo.rasterizer.cullMode = VK_CULL_MODE_NONE;
	renderingInfo.rasterizer.polygonMode = VK_POLYGON_MODE_LINE;

	PipelineManager::CreatePipeline("wireframePipeline", pipelineLayout, renderingInfo);
}

static void CreateSkinningPipeline(VulkanDevice& device)
{
	// TODO - get rid of hack!
	VulkanDescriptorSetLayout* descriptorSetLayout = new VulkanDescriptorSetLayout(&device);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 2);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 3);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 4);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 5);
	descriptorSetLayout->BindLayout();

	VulkanPipelineLayout* pipelineLayout = new VulkanPipelineLayout(&device, { descriptorSetLayout });
		
	Array<uint8> vertCode = LoadSPVShader("gpuColorSkinning.vs.spv");
	Array<uint8> fragCode = LoadSPVShader("gpuColorSkinning.fs.spv");

	VulkanShader* vertShader = VulkanShaderManager::CreateShader();
	VulkanShader* fragShader = VulkanShaderManager::CreateShader();
	vertShader->Compile(vertCode, ShaderStage::Stage_Vert);
	fragShader->Compile(fragCode, ShaderStage::Stage_Frag);
	VulkanShaderManager::AddShader(vertShader, fragShader, "Skinning");

	RenderPipelineInfo renderingInfo = {};

	Memset(renderingInfo.shaders, 0, sizeof(VulkanShader*) * ArraySize(renderingInfo.shaders));
	renderingInfo.shaders[Stage_Vert] = vertShader;
	renderingInfo.shaders[Stage_Frag] = fragShader;


	{
		RenderPipelineInfo::VertexBinding vertBinding;
		vertBinding.binding = 0;
		vertBinding.stride = sizeof(Vertex);
		vertBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		renderingInfo.vertexBindings.Add(vertBinding);

		vertBinding.binding = 1;
		vertBinding.stride = sizeof(VertexBoneWeights);
		renderingInfo.vertexBindings.Add(vertBinding);
	}

	{
		RenderPipelineInfo::VertexAttribute vertAttrib;
		vertAttrib.binding = 0;

		// First binding for position information!!
		// Position
		vertAttrib.location = 0;
		vertAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
		vertAttrib.offset = offsetof(Vertex, x);
		renderingInfo.vertexAttributes.Add(vertAttrib);

		// Normal
		vertAttrib.location = 1;
		vertAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
		vertAttrib.offset = offsetof(Vertex, nx);
		renderingInfo.vertexAttributes.Add(vertAttrib);

		// UV texture coordinates
		vertAttrib.location = 2;
		vertAttrib.format = VK_FORMAT_R32G32_SFLOAT;
		vertAttrib.offset = offsetof(Vertex, u);
		renderingInfo.vertexAttributes.Add(vertAttrib);

		// Color
		// 		vertAttrib.location = 3;
		// 		vertAttrib.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		// 		vertAttrib.offset = offsetof(Vertex, color);
		// 		renderingInfo.vertexAttributes.Add(vertAttrib);

		// Second binding for skinning information!!
		vertAttrib.binding = 1;

		// Weights
		vertAttrib.location = 3;
		vertAttrib.format = VK_FORMAT_R32G32B32A32_SFLOAT;
		vertAttrib.offset = offsetof(VertexBoneWeights, weights);
		renderingInfo.vertexAttributes.Add(vertAttrib);

		vertAttrib.location = 4;
		vertAttrib.format = VK_FORMAT_R32G32B32A32_UINT;
		vertAttrib.offset = offsetof(VertexBoneWeights, boneIndices);
		renderingInfo.vertexAttributes.Add(vertAttrib);
	}

	renderingInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	// Rasterizer
	renderingInfo.rasterizer.cullMode = VK_CULL_MODE_NONE;
	renderingInfo.rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	renderingInfo.rasterizer.depthBiasEnabled = false;
	renderingInfo.rasterizer.depthBiasClamp = 0.f;
	renderingInfo.rasterizer.depthBiasConstant = 0.f;
	renderingInfo.rasterizer.depthBiasSlope = 0.f;
	renderingInfo.rasterizer.depthClampEnabled = false;

	// Depth/Stencil
	renderingInfo.depthStencilInfo = {};
	renderingInfo.depthStencilInfo.depthTestEnabled = true;
	renderingInfo.depthStencilInfo.depthWriteEnabled = true;
	renderingInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	renderingInfo.depthStencilInfo.stencilTestEnabled = false;

	// Color Blend
	RenderPipelineInfo::ColorBlendAttachment colorAttachement = {};
	colorAttachement.colorMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorAttachement.enableBlending = false;
	colorAttachement.srcBlend = VK_BLEND_FACTOR_ONE;
	colorAttachement.dstBlend = VK_BLEND_FACTOR_ZERO;
	colorAttachement.colorBlendOperation = VK_BLEND_OP_ADD;
	colorAttachement.srcAlphaBlend = VK_BLEND_FACTOR_ONE;
	colorAttachement.dstAlphaBlend = VK_BLEND_FACTOR_ZERO;
	colorAttachement.alphaBlendOperation = VK_BLEND_OP_ADD;
	renderingInfo.blendAttachments.Add(colorAttachement);

	renderingInfo.logicOperation = VK_LOGIC_OP_COPY;


	PipelineManager::CreatePipeline("SkinningPipeline", pipelineLayout, renderingInfo);
}

void Game::CompileShaders()
{
	// TODO - get rid of hack!
	VulkanDescriptorSetLayout* descriptorSetLayout = new VulkanDescriptorSetLayout(renderEngine->GetDevice());
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 3);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4);
	descriptorSetLayout->BindLayout();

	pipelineLayout = new VulkanPipelineLayout(renderEngine->GetDevice(), { descriptorSetLayout });

	Array<uint8> vertCode = LoadSPVShader("phongMultiTex.vs.spv");
	Array<uint8> fragCode = LoadSPVShader("phongMultiTex.fs.spv");

	VulkanShader* vertShader = VulkanShaderManager::CreateShader();//"default.vs.spv", ShaderStage::Vert);
	VulkanShader* fragShader = VulkanShaderManager::CreateShader();// "default.fs.spv", ShaderStage::Frag);
	vertShader->Compile(vertCode, ShaderStage::Stage_Vert);
	fragShader->Compile(fragCode, ShaderStage::Stage_Frag);
	VulkanShaderManager::AddShader(vertShader, fragShader, "MultiShaderPhong");

	renderingInfo.shaders[Stage_Vert] = vertShader;
	renderingInfo.shaders[Stage_Frag] = fragShader;

	renderingInfo.rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;

	PipelineManager::CreatePipeline("multiTexPipeline", pipelineLayout, renderingInfo);

	CreateSkinningPipeline(*renderEngine->GetDevice());
}
