
#include "GameWorld.hpp"
#include "Scene/Scene.hpp"
#include "ScreenView.hpp"
#include "Window/Window.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Graphics/GraphicsInterface.hpp"

void InitializeRenderTarget(GraphicsPipelineDescription& init)
{
	RenderTargetDescription desc = {};
	
	desc.colorDescs[0].format = ImageFormat::RGBA_8u;
	desc.colorDescs[0].load = LoadOperation::Clear;
	desc.colorDescs[0].store = StoreOperation::Store;
	desc.colorDescs[0].stencilLoad = LoadOperation::DontCare;
	desc.colorDescs[0].sampleCount = 1;

	desc.depthDesc.format = ImageFormat::DS_32f_8u;
	desc.depthDesc.load = LoadOperation::Clear;
	desc.depthDesc.store = StoreOperation::Store;
	desc.depthDesc.stencilLoad = LoadOperation::DontCare;
	desc.depthDesc.stencilStore = StoreOperation::DontCare;

	desc.targetCount = 1;

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


