#include "Engine.h"
#include "Math/MathEngine.h"
#include "Camera/CameraManager.h"
#include "Camera/Camera.h"

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Model/Model.h"
#include "Texture2D/TextureManager.h"
#include "Texture2D/Texture.h"
#include "Input/Input.hpp"
#include "Windowing/Window.h"
#include "Animation/AnimationTimerManager.h"
#include "Scene/GameWorld.hpp"

#include "Renderer/RenderingEngine.h"
#include "Renderer/PipelineManager.h"

#include "ECS/SystemsManager.hpp"

using namespace Musa;

// TODO - Remove anything referencing vulkan...
#include "Renderer/Vulkan/VulkanShader.h"
#include "Renderer/Vulkan/VulkanShaderManager.h"
#include "Renderer/Vulkan/VulkanDescriptorSet.h"
#include "Renderer/Vulkan/VulkanDevice.h"
#include "Renderer/Vulkan/VulkanPipeline.h"

void Engine::Run()
{
	InitializeTime();

	InitializeWindow();

	InitializeWorld();

	InitializeRendering();

	InitializeECS();

	Init();

	LoadContent();

	while (window->IsActive())
	{
		tickTimer.Lap();
		Time elapsedTime = internalTimer.Toc();
		AnimationTimerManager::Update(elapsedTime);

		GetInputManager().Update();
		//CalculateFrameTime();

		// TODO - Need to have some sort of frame tick
		float tick = tickTimer.GetMilliseconds();
		Update(tick);

		internalTimer.Tic();
		tickTimer.Start();

		Render();

		window->SwapBuffers();
	}

	UnloadContent();

	DeInit();

	delete window;
}

float prevMouseX, prevMouseY = 0.f;
float mouseX, mouseY = 0.f;


void Engine::Update(float tick)
{
	GetSystemsManager().ProcessDirtyEntities();
	GetSystemsManager().TickSystems(tick);

	CameraManager::Update();
	//GetGameObjectManager().Update(tick);
	world->TickWorld(tick);
	UpdateGame(tick);
}

void Engine::Render()
{
	renderEngine->RenderFrame();
}

void Engine::UnloadContent()
{
}

void Engine::DeInit()
{
}

void Engine::InitializeECS()
{
	GetSystemsManager().Initialize();
}

void Engine::InitializeWorld()
{
	// TODO - This probably will need to be a lot more than just creating a damn World class
	world = new GameWorld();

	GetGameObjectManager().Initialize(*world);
}

void Engine::InitializeRendering()
{
	// Need to figure out if this is actually needed
	renderEngine = new Renderer;
	renderEngine->Initialize();
	renderEngine->SetupRendering(window);

	InitializeRenderInfo();
	InitializeLayout();
	
	PipelineManager::CreatePipeline("defaultPipeline", pipelineLayout, renderingInfo);

	//TextureManager::LoadTexture(TextureManager::DefaultFileName, TextureManager::DefaultTexture);
}

void Engine::InitializeTime()
{
	EngineTick::Init();

	tickTimer.Start();
	globalTimer.Tic();
	internalTimer.Tic();
}

void Engine::InitializeRenderInfo()
{
	Array<uint8> vertCode = LoadSPVShader("default.vs.spv");
	Array<uint8> fragCode = LoadSPVShader("default.fs.spv");

// 	Array<uint8> vertCode = LoadSPVShader("phong.vert");
// 	Array<uint8> fragCode = LoadSPVShader("phong.frag");

	VulkanShader* vertShader = VulkanShaderManager::CreateShader();
	VulkanShader* fragShader = VulkanShaderManager::CreateShader();
	vertShader->Compile(vertCode, ShaderStage::Stage_Vert);
	fragShader->Compile(fragCode, ShaderStage::Stage_Frag);
	VulkanShaderManager::AddShader(vertShader, fragShader, VulkanShaderManager::DefaultShader);

	Memset(renderingInfo.shaders, 0, sizeof(VulkanShader*) * ArraySize(renderingInfo.shaders));
	renderingInfo.shaders[Stage_Vert] = vertShader;
	renderingInfo.shaders[Stage_Frag] = fragShader;

	// Vertex Input Description
	//for (const auto& binding : Vertex::GetBindingDescription())
	{
		RenderPipelineInfo::VertexBinding vertBinding;
		vertBinding.binding = 0;
		vertBinding.stride = sizeof(Vertex);
		vertBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		renderingInfo.vertexBindings.Add(vertBinding);
	}

	{
		RenderPipelineInfo::VertexAttribute vertAttrib;
		vertAttrib.binding = 0;

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
	colorAttachement.enableBlending = true;
	colorAttachement.srcBlend = VK_BLEND_FACTOR_ONE;
	colorAttachement.dstBlend = VK_BLEND_FACTOR_ZERO;
	colorAttachement.colorBlendOperation = VK_BLEND_OP_ADD;
	colorAttachement.srcAlphaBlend = VK_BLEND_FACTOR_ONE;
	colorAttachement.dstAlphaBlend = VK_BLEND_FACTOR_ZERO;
	colorAttachement.alphaBlendOperation = VK_BLEND_OP_ADD;
	renderingInfo.blendAttachments.Add(colorAttachement);

	renderingInfo.logicOperation = VK_LOGIC_OP_COPY;

	// Viewport/Scissor
	//renderInfo.viewportScissorCount = 1;
}

void Engine::InitializeLayout()
{
	// TODO - This needs to be moved out of Engine.cpp when I have actual shader reflection
	VulkanDescriptorSetLayout* descriptorSetLayout = new VulkanDescriptorSetLayout(renderEngine->GetDevice());
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 2);
	//descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 3);
	descriptorSetLayout->BindLayout();

	pipelineLayout = new VulkanPipelineLayout(renderEngine->GetDevice(), { descriptorSetLayout });
}

void Engine::CalculateFrameTime() const
{
	static float sec = 0.f;
	sec += tickTimer.GetSeconds();
	
	if (sec > 1.f)
	{
		float fps = 1.f / tickTimer.GetSeconds();
		printf("FPS: %f\n", fps);
		//SetWindowText(mhMainWnd, outs.str().c_str());
		sec = 0;
	}
}

void Engine::InitializeWindow()
{
	// TODO - Move this console set up somewhere else
	FILE* stdOut = nullptr;
	{
		AllocConsole();
		freopen_s(&stdOut, "CONOUT$", "w", stdout);
	}

	//const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	width = 1080;//videoMode->width; // 800;
	height = 720;//videoMode->height;// 600;

	
	window = new Window(width, height);
	window->Initialize();
	window->SetAsActiveWindow();

	GetInputManager().Initialize(*window);
}