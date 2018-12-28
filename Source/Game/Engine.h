#pragma once

#include "Platform.h"
#include "Graphics.h"
#include "Time/Timer.h"
#include "Time/Time.h"
#include "Time/EngineTick.h"

// TODO - Get rid of vulkan references...
#include "Renderer/Vulkan/VulkanRenderingInfo.h"

class Window;
class GameWorld;

class Engine
{
public:
	Engine() = default;
	virtual ~Engine() = default;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	void Run();
protected:
	virtual void Init() = 0;
	virtual void LoadContent() = 0;
	virtual void UpdateGame(float deltaTime) = 0;
	void Update(float tick);
	void Render();
	virtual void UnloadContent() = 0;
	virtual void DeInit() = 0;

private:
	void InitializeECS();
	void InitializeWorld();
	void InitializeWindow();
	void InitializeRendering();
	void InitializeTime();

	void InitializeRenderInfo();
	void InitializeLayout();

	void CalculateFrameTime() const;

// TODO - Some of this data will be moved because it shouldn't live in engine anyways.
private:
	EngineTick tickTimer;

protected:
	Timer globalTimer;
	Timer internalTimer;
	RenderPipelineInfo renderingInfo;
	class Renderer* renderEngine;
	Window* window;
	GameWorld* world;

	// TODO - Get rid of vulkan references
	class VulkanPipelineLayout* pipelineLayout = nullptr;
	int32 width = 800;
	int32 height = 600;
	
};

