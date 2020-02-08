
#include "GameWorld.hpp"
#include "Scene/Scene.hpp"
#include "ScreenView.hpp"
#include "Window/Window.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Graphics/GraphicsInterface.hpp"

void InitializeRenderTarget(GraphicsPipelineDescription& init)
{
	RenderTargetDescription desc = {};
	
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
	desc.numColorAttachments = 1;

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
	: scene(new Scene)
{
	ScreenView* view = new ScreenView(window.GetWidth(), window.GetHeight());
	scene->SetView(*view);
	scene->InitializeScene();
}

GameWorld::~GameWorld()
{
	delete scene;
}

void GameWorld::TickWorld(float deltaTime)
{
	scene->Tick(deltaTime);
}

void GameWorld::PushToRenderState()
{
	scene->PushStateToGpu();
}

void GameWorld::RenderWorld(Viewport& viewport)
{
	scene->RenderScene(viewport);
}

Scene& GameWorld::GetScene()
{
	return *scene;
}


