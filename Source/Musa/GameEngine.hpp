#pragma once

class GameWorld;

class GameEngine final
{
public:

	void RunEngine();

private:
	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void EngineFrame();

private:
	GameWorld* world = nullptr;
	bool running = false;
};