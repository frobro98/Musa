
#include "GameWorld.hpp"
#include "Scene/Scene.hpp"
#include "Renderer/RenderingEngine.h"
#include "ScreenView.hpp"
#include "Windowing/Window.h"
#include "Graphics/PipelineInitDescription.hpp"


// TODO - Get rid of vulkan being in my fucking game....
#include "Graphics/Vulkan/VulkanPipeline.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include "Graphics/Vulkan/VulkanShaderManager.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"

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

	graphicsInit.vertexInputs = GetVertexInput<Mesh>();
	graphicsInit.rasterizerDesc = RasterDesc<>();
	graphicsInit.depthStencilTestDesc = DepthTestDesc<>();
	graphicsInit.blendingDescs[0] = BlendDesc<>();

	graphicsInit.topology = PrimitiveTopology::TriangleList;

	return graphicsInit;
}

GameWorld::GameWorld()
	: scene(new Scene)
{
}

GameWorld::~GameWorld()
{
	delete scene;
}

void GameWorld::Initialize(const Window& window)
{
	// Need to figure out if this is actually needed
	renderEngine = new Renderer;
	renderEngine->SetupRendering();

	ScreenView* view = new ScreenView(window.GetWidth(), window.GetHeight());
	scene->SetView(*view);
	scene->InitializeScene();
}

void GameWorld::TickWorld(float deltaTime)
{
	scene->Tick(deltaTime);
}

void GameWorld::PushToRenderState()
{
	scene->PushStateToGpu();
}

void GameWorld::RenderWorld()
{
	scene->RenderScene(*renderEngine);
}

Scene& GameWorld::GetScene()
{
	return *scene;
}


