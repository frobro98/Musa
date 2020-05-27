// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Scene/GameWorld.hpp"
#include "Math/IntVector2.hpp"

#include "Engine/FrameRenderTargets.hpp"

class Window;
class ProfilerStatistics;
namespace UI
{
class Context;
}
class GameInput;

class MusaEngine final
{
public:
	MusaEngine(UI::Context& context);

	void StartupEngine(Window& window);
	void ShutdownEngine();

	void StartRunningEngine();
	void StopRunningEngine();


	inline bool ShouldRun() const { return running; }
	inline GameInput& GetGameInput() { return *gameInput; }
	inline IntVector2 GetViewDimensions() const { return IntVector2(viewport->GetWidth(), viewport->GetHeight()); }

	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void UnloadContent();
	
	void UpdateAndRender(f32 tick);
	void GatherFrameMetrics();

private:
	void SetupWindowContext(Window& window);
	void InitializeSceneView();

	void RenderFrame();

private:
	FrameRenderTargets engineTargets;
	UniquePtr<GameWorld> world;
	UniquePtr<Viewport> viewport;
	UniquePtr<GameInput> gameInput;
	UI::Context* uiContext = nullptr;

	bool running = false;
};