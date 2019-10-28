#pragma once

#include "Time/EngineTick.h"
#include "Types/UniquePtr.hpp"
#include "Scene/GameWorld.hpp"

class Window;
class ProfilerStatistics;
class GameUIContext;
class GameInput;

class MusaEngine final
{
public:
	MusaEngine(GameUIContext& context);

	void InitializeGraphics();
	void SetupWindowContext(Window& window);

	void RunEngine();

	void StopEngine();

	inline GameInput& GetGameInput() { return *gameInput; }

private:
	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void EngineFrame();
	
	void UpdateAndRenderWorld(float32 tick);
	void GatherFrameMetrics();

private:
	EngineTick frameTick;
	UniquePtr<GameWorld> world;
	UniquePtr<Viewport> viewport;
	UniquePtr<GameInput> gameInput;
	GameUIContext* uiContext = nullptr;

	bool running = false;
};