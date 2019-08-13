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
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "RenderPipeline/BatchPrimitives.hpp"


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

	float xOffset = -100.f;
	BatchedLineDescription desc[3] = {};
	desc[0].color = Color32::Red();
	desc[0].start = Vector3(xOffset, 0, 0);
	desc[0].end = Vector3(xOffset + 50, 0, 0);

	desc[1].color = Color32::Green();
	desc[1].start = Vector3(xOffset, 0, 0);
	desc[1].end = Vector3(xOffset, 50, 0);

	desc[2].color = Color32::Blue();
	desc[2].start = Vector3(xOffset, 0, 0);
	desc[2].end = Vector3(xOffset, 0, 50);
	BatchLinePrimitives(desc, ArraySize(desc));

	renderer->BeginRenderFrame(viewport.GetNativeViewport());
	sceneRendering->RenderScene(*renderer, *this, viewport, view->view);
	renderer->EndRenderFrame(viewport.GetNativeViewport());
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
		NativeTexture* target = GetGraphicsInterface().CreateEmptyTexture2D(
			view->GetScreenWidth(), view->GetScreenHeight(), 
			desc.format, 1, TextureUsage::RenderTarget
		);
		gbufferTextures.colorTargets[i] = target;
	}

	NativeTexture* depthTarget = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		gbufferTargets.depthDesc.format, 1, 
		TextureUsage::DepthStencilTarget
	);
	gbufferTextures.depthTarget = depthTarget;
}
