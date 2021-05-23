// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Scene/GameWorld.hpp"
#include "Math/IntVector2.hpp"
#include "ECS/World.hpp"
#include "RenderPipeline/RenderPipeline.hpp"

class Window;
class ProfilerStatistics;
class GameInput;
class ApplicationInputMap;
class ApplicationEventDispatcher;

namespace UI
{
class Context;
}

class MusaEngine final
{
public:
	MusaEngine(UI::Context& context, const ApplicationEventDispatcher& inputDispatcher);

	void StartupEngine(Window& window);
	void ShutdownEngine();

	void StartRunningEngine();
	void StopRunningEngine();


	inline GameInput& GetGameInput() { return *gameInput; }
	inline bool ShouldRun() const { return running; }
	inline IntVector2 GetViewDimensions() const { return IntVector2(viewport->GetWidth(), viewport->GetHeight()); }

	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	void UnloadContent();
	
	void UpdateAndRender(f32 tick);
	void GatherFrameMetrics();

	void SetInputHandler(ApplicationInputMap& inputHandler);

private:
	void SetupWindowContext(Window& window);
	void InitializeSceneView();
	void PushApplicationEventsToWorld();

	void RenderFrame();

private:
	FrameRenderTargets engineTargets;
	UniquePtr<GameWorld> world;
	UniquePtr<Viewport> viewport;
	UniquePtr<GameInput> gameInput;
	UniquePtr<Musa::RenderPipeline> renderPipeline;
	const ApplicationEventDispatcher& inputDispatcher;
	UI::Context* uiContext = nullptr;

	ApplicationInputMap* inputMap = nullptr;
	// TODO - This and the other "world" aren't really needed...
	Musa::World ecsWorld;

	bool running = false;
};