#pragma once

#include "Time/EngineTick.h"

class GameWorld;
class ProfilerStatistics;

class GameEngine final
{
public:

	void RunEngine();

private:
	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void EngineFrame();
	
	void UpdateAndRenderWorld(float32 tick);
	void GatherFrameMetrics();

private:
	EngineTick frameTick;
	GameWorld* world = nullptr;
	bool running = false;
};