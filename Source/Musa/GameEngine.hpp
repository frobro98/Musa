#pragma once

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
	
	void UpdateAndRenderWorld();
	void GatherFrameMetrics();

private:
	ProfilerStatistics* profilingStats = nullptr;
	GameWorld* world = nullptr;
	bool running = false;
};