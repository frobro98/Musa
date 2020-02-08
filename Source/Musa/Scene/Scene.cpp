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
#include "RenderPipeline/SceneRenderPipeline.h"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "RenderPipeline/BatchPrimitives.hpp"

#include "Debugging/MetricInterface.hpp"

DECLARE_METRIC_GROUP(SceneFrame);
METRIC_STAT(BeginRenderFrame, SceneFrame);
METRIC_STAT(EndRenderFrame, SceneFrame);


static void BuildGBufferDescription(RenderTargetAttachment& gbufferDesc, ImageFormat format)
{
	gbufferDesc.format = format;
	gbufferDesc.load = LoadOperation::Clear;
	gbufferDesc.store = StoreOperation::Store;
	gbufferDesc.stencilLoad = LoadOperation::DontCare;
	gbufferDesc.stencilStore = StoreOperation::DontCare;
	gbufferDesc.sampleCount = 1;
}

static void BuildGBufferDepth(RenderTargetAttachment& depthDesc)
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
}

void Scene::InitializeScene()
{
	BuildGBufferDescription(gbufferTargets.colorAttachments[0], ImageFormat::RGBA_16f);
	BuildGBufferDescription(gbufferTargets.colorAttachments[1], ImageFormat::RGBA_16f);
	BuildGBufferDescription(gbufferTargets.colorAttachments[2], ImageFormat::RGBA_8norm);
	BuildGBufferDepth(gbufferTargets.depthAttachment);

	gbufferTargets.numColorAttachments = GBufferCount;

	sceneRendering = new SceneRenderPipeline;

	renderer = GetGraphicsInterface().GetRenderContext();
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

	GetRenderObjectManager().SequenciallyPull();
}

void Scene::RenderScene(Viewport& viewport)
{
	if (!gbuffersInitialized)
	{
		CreateGBuffer();
		gbuffersInitialized = true;
	}

	BEGIN_TIMED_BLOCK(BeginRenderFrame);
	renderer->BeginRenderFrame(viewport.GetNativeViewport());
	END_TIMED_BLOCK(BeginRenderFrame);

	sceneRendering->RenderScene(*renderer, *this, viewport, view->view);

	BEGIN_TIMED_BLOCK(EndRenderFrame);
	renderer->EndRenderFrame(viewport.GetNativeViewport());
	END_TIMED_BLOCK(EndRenderFrame);
}

void Scene::SetView(ScreenView& view_)
{
	view = &view_;

	gbufferTargets.targetExtents = { (float32)view->GetScreenWidth(), (float32)view->GetScreenHeight() };
}

void Scene::CreateGBuffer()
{
	gbufferTextures.numColorTargets = GBufferCount;

	RenderTargetAttachment* desc = &gbufferTargets.colorAttachments[0];
	posTexture = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		desc->format, 1, TextureUsage::RenderTarget
	);

	desc = &gbufferTargets.colorAttachments[1];
	normalTexture = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		desc->format, 1, TextureUsage::RenderTarget
	);

	desc = &gbufferTargets.colorAttachments[2];
	diffuseTexture = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		desc->format, 1, TextureUsage::RenderTarget
	);
	
	depthTexture = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		gbufferTargets.depthAttachment.format, 1, 
		TextureUsage::DepthStencilTarget
	);

	gbufferTextures.colorTargets[0] = posTexture.Get();
	gbufferTextures.colorTargets[1] = normalTexture.Get();
	gbufferTextures.colorTargets[2] = diffuseTexture.Get();
	gbufferTextures.depthTarget = depthTexture.Get();
}
