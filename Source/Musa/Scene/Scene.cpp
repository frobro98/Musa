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
#include "Graphics/RenderContext.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "RenderPipeline/BatchPrimitives.hpp"

#include "Debugging/MetricInterface.hpp"

DECLARE_METRIC_GROUP(SceneFrame);
METRIC_STAT(BeginRenderFrame, SceneFrame);
METRIC_STAT(EndRenderFrame, SceneFrame);


// static void BuildGBufferDescription(RenderTargetAttachment& gbufferDesc, ImageFormat format)
// {
// 	gbufferDesc.format = format;
// 	gbufferDesc.load = LoadOperation::Clear;
// 	gbufferDesc.store = StoreOperation::Store;
// 	gbufferDesc.stencilLoad = LoadOperation::DontCare;
// 	gbufferDesc.stencilStore = StoreOperation::DontCare;
// 	gbufferDesc.sampleCount = 1;
// }
// 
// static void BuildGBufferDepth(RenderTargetAttachment& depthDesc)
// {
// 	depthDesc.format = ImageFormat::DS_32f_8u;
// 	depthDesc.load = LoadOperation::Clear;
// 	depthDesc.store = StoreOperation::Store;
// 	depthDesc.stencilLoad = LoadOperation::DontCare;
// 	depthDesc.stencilStore = StoreOperation::DontCare;
// 	depthDesc.sampleCount = 1;
// }

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
// 	BuildGBufferDescription(gbufferTargets.colorAttachments[0], ImageFormat::RGBA_16f);
// 	BuildGBufferDescription(gbufferTargets.colorAttachments[1], ImageFormat::RGBA_16f);
// 	BuildGBufferDescription(gbufferTargets.colorAttachments[2], ImageFormat::RGBA_8norm);
// 	BuildGBufferDepth(gbufferTargets.depthAttachment);


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
	Assert(lights.HasRoom());
	lights.Add(&light);
}

void Scene::RemoveLightFromScene(Light& light)
{
	Light* lightPtr = &light;
	lights.Remove(lightPtr);
}

void Scene::Tick(float deltaTime)
{
	for (auto& go : activeGameObjects)
	{
		go->Update(deltaTime);
	}
}

void Scene::RenderScene(const GBuffer& gbuffer, const SceneRenderTargets& sceneTargets, RenderTarget& uiTarget, RenderObjectManager& renderManager, Viewport& viewport)
{
	BEGIN_TIMED_BLOCK(BeginRenderFrame);
	renderer->BeginRenderFrame(viewport.GetNativeViewport());
	END_TIMED_BLOCK(BeginRenderFrame);

	sceneRendering->RenderScene(*renderer, *this, gbuffer, sceneTargets, uiTarget, renderManager, viewport, view->view);

	BEGIN_TIMED_BLOCK(EndRenderFrame);
	renderer->EndRenderFrame(viewport.GetNativeViewport());
	END_TIMED_BLOCK(EndRenderFrame);
}

void Scene::SetView(ScreenView& view_)
{
	view = &view_;

	// TODO - NEed to resize the render targets when the viewport changes
	//gbufferTargets.targetExtents = { (float32)view->GetScreenWidth(), (float32)view->GetScreenHeight() };
}

