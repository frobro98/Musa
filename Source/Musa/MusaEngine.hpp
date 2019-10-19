#pragma once

#include "Time/EngineTick.h"
#include "Types/UniquePtr.hpp"
#include "Scene/GameWorld.hpp"

class Window;
class ProfilerStatistics;
class GameUIContext;

class MusaEngine final
{
public:
	MusaEngine(GameUIContext& context);

	void InitializeGraphics();
	void SetupWindowContext(Window& window);

	void RunEngine();

private:
	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void EngineFrame();
	
	void UpdateAndRenderWorld(float32 tick);
	void GatherFrameMetrics();

private:
	EngineTick frameTick;
	UniquePtr<GameWorld> world;
	GameUIContext* uiContext = nullptr;
	Window* window = nullptr;

	bool running = false;
};