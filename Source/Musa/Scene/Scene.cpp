#include "Scene.hpp"
#include "Platform.h"
#include "ScreenView.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Camera/ViewFrustum.hpp"
#include "GameObject/GameObject.h"
#include "Thread/JobSystem/JobUtilities.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "GameObject/RenderObjectManager.hpp"
#include "Scene/Viewport.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "RenderPipeline/SceneRendering.h"

// TODO - Get rid of these vulkan includes
#include "Graphics/Vulkan/VulkanViewport.hpp"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanStagingBufferManager.hpp"

static void BuildGBufferDescription(ColorDescription& gbufferDesc, ImageFormat format)
{
	gbufferDesc.format = format;
	gbufferDesc.load = LoadOperation::Clear;
	gbufferDesc.store = StoreOperation::Store;
	gbufferDesc.stencilLoad = LoadOperation::DontCare;
	gbufferDesc.stencilStore = StoreOperation::DontCare;
	gbufferDesc.sampleCount = 1;
}

static void BuildGBufferDepth(DepthStencilDescription& depthDesc)
{
	depthDesc.format = ImageFormat::DS_32f_8u;
	depthDesc.load = LoadOperation::Clear;
	depthDesc.store = StoreOperation::Store;
	depthDesc.stencilLoad = LoadOperation::DontCare;
	depthDesc.stencilStore = StoreOperation::DontCare;
	depthDesc.sampleCount = 1;
}

//////////////////////////////////////////////////////////////////////////
// Scene Definition
//////////////////////////////////////////////////////////////////////////

Scene::~Scene()
{
	for (auto go : gameObjectsInScene)
	{
		delete go;
	}

	for (auto tex : gbufferTextures.colorTargets)
	{
		delete tex;
	}
	delete gbufferTextures.depthTarget;
}

void Scene::InitializeScene()
{
	BuildGBufferDescription(gbufferTargets.colorDescs[0], ImageFormat::RGBA_16f);
	BuildGBufferDescription(gbufferTargets.colorDescs[1], ImageFormat::RGBA_16f);
	BuildGBufferDescription(gbufferTargets.colorDescs[2], ImageFormat::RGBA_8norm);
	BuildGBufferDepth(gbufferTargets.depthDesc);

	gbufferTargets.targetCount = GBufferCount;

	sceneRendering = new SceneRendering;
}

void Scene::AddGameObjectToScene(GameObject& object)
{
	activeGameObjects.Add(&object);
	gameObjectsInScene.Add(&object);
}

void Scene::RemoveGameObjectFromScene(GameObject& object)
{
	activeGameObjects.RemoveAll(&object);
	gameObjectsInScene.RemoveAll(&object);
}

void Scene::AddMeshInfoToScene(MeshRenderInfo& obj)
{
	renderingInfo.Add(&obj);
}

void Scene::RemoveMeshInfoFromScene(MeshRenderInfo& obj)
{
	renderingInfo.RemoveAll(&obj);
}

void Scene::AddLightToScene(Light& light)
{
	bool emptySlotFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == nullptr)
		{
			lights[i] = &light;
			emptySlotFound = true;
			break;
		}
	}

	Assert(emptySlotFound);
}

void Scene::RemoveLightFromScene(Light& light)
{
	bool lightFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == &light)
		{
			lights[i] = nullptr;
			lightFound = true;
			break;
		}
	}

	Assert(lightFound);
}

void Scene::Tick(float deltaTime)
{
	for (auto& go : activeGameObjects)
	{
		go->Update(deltaTime);
	}
}

void Scene::PushStateToGpu()
{
	Camera* cam = GetCameraManager().GetActiveCamera();
	view->AssociateCameraWithView(*cam);

	GetRenderObjectManager().SequenciallyPull(view->view);
}

void Scene::RenderScene(Viewport& viewport)
{
	if (!gbuffersInitialized)
	{
		CreateGBuffer();
		gbuffersInitialized = true;
	}

	if (viewport.GetNativeViewport().CanProceedWithRender())
	{
		GetGraphicsInterface().GetGraphicsDevice()->GetStagingBufferManager().ProcessDeferredReleases();

		sceneRendering->RenderScene(*this, viewport, view->view);

		viewport.GetNativeViewport().SubmitFrame();
	}
}

void Scene::SetView(ScreenView& view_)
{
	view = &view_;

	gbufferTargets.targetExtents = { (float32)view->GetScreenWidth(), (float32)view->GetScreenHeight() };
}

void Scene::CreateGBuffer()
{
	gbufferTextures.targetCount = GBufferCount;

	for (uint32 i = 0; i < GBufferCount; ++i)
	{
		ColorDescription& desc = gbufferTargets.colorDescs[i];
		VulkanTexture* target = GetGraphicsInterface().CreateEmptyTexture2D(
			view->GetScreenWidth(), view->GetScreenHeight(), 
			desc.format, 1, TextureUsage::RenderTarget
		);
		TextureSamplerCreateParams params;
		target->sampler = GetGraphicsInterface().CreateTextureSampler(params);
		gbufferTextures.colorTargets[i] = target;
	}

	VulkanTexture* depthTarget = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		gbufferTargets.depthDesc.format, 1, 
		TextureUsage::DepthStencilTarget
	);
	TextureSamplerCreateParams params;
	depthTarget->sampler = GetGraphicsInterface().CreateTextureSampler(params);
	gbufferTextures.depthTarget = depthTarget;
}
