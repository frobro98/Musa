// Copyright 2020, Nathan Blane

#include "GameWorld.hpp"
#include "Scene/Scene.hpp"
#include "ScreenView.hpp"
#include "Window/Window.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Camera/CameraManager.h"
#include "Engine/FrameRenderTargets.hpp"

void InitializeRenderTarget(GraphicsPipelineDescription& init)
{
	RenderTargetDescription desc = {};
	desc.colorAttachments.Resize(1);
	
	desc.colorAttachments[0].format = ImageFormat::RGBA_8u;
	desc.colorAttachments[0].load = LoadOperation::Clear;
	desc.colorAttachments[0].store = StoreOperation::Store;
	desc.colorAttachments[0].stencilLoad = LoadOperation::DontCare;
	desc.colorAttachments[0].sampleCount = 1;

	desc.depthAttachment.format = ImageFormat::DS_32f_8u;
	desc.depthAttachment.load = LoadOperation::Clear;
	desc.depthAttachment.store = StoreOperation::Store;
	desc.depthAttachment.stencilLoad = LoadOperation::DontCare;
	desc.depthAttachment.stencilStore = StoreOperation::DontCare;

	desc.hasDepth = true;

	init.renderTargets = desc;
}

GraphicsPipelineDescription GetDefaultDescription()
{
	GraphicsPipelineDescription graphicsInit = {};
	InitializeRenderTarget(graphicsInit);

	graphicsInit.vertexInputs = GetVertexInput<Vertex>();
	graphicsInit.rasterizerDesc = RasterDesc<>();
	graphicsInit.depthStencilTestDesc = DepthTestDesc<>();
	graphicsInit.blendingDescs[0] = BlendDesc<>();

	graphicsInit.topology = PrimitiveTopology::TriangleList;

	return graphicsInit;
}

GameWorld::GameWorld(const Window& window)
	: gameObjectManager(MakeUnique<GameObjectManager>(*this)),
	renderObjectManager(MakeUnique<RenderObjectManager>()),
	screenView(MakeUnique<ScreenView>(window.GetWidth(), window.GetHeight())),
	scene(MakeUnique<Scene>())
{
}

void GameWorld::TickWorld(float deltaTime)
{
	scene->Tick(deltaTime);
}

void GameWorld::PushToRenderState()
{
	Camera* mainCamera = GetCameraManager().GetActiveCamera();
	screenView->AssociateCameraWithView(*mainCamera);
	renderObjectManager->SequenciallyPull(*screenView);
}

void GameWorld::RegisterRenderInfo(const GameObject& go, MeshRenderInfo& renderInfo)
{
	renderObjectManager->RegisterGameObject(go, renderInfo);
}

void GameWorld::UnregisterRenderInfo(const GameObject& go)
{
	renderObjectManager->UnregisterGameObject(go);
}

Scene& GameWorld::GetScene()
{
	return *scene;
}


