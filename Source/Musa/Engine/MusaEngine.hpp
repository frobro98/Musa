// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/UniquePtr.hpp"
#include "Math/IntVector2.hpp"
#include "ECS/World.hpp"
#include "Engine/FrameRenderTargets.hpp"
#include "Scene/Viewport.hpp"
#include "Scene/ScreenView.hpp"
#include "RenderPipeline/RenderPipeline.hpp"

class Window;
class ProfilerStatistics;
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

	void RenderFrame();

private:
	FrameRenderTargets engineTargets;
	UniquePtr<Viewport> viewport;
	UniquePtr<ScreenView> screenView;
	UniquePtr<Musa::RenderPipeline> renderPipeline;
	const ApplicationEventDispatcher& inputDispatcher;
	UI::Context* uiContext = nullptr;

	ApplicationInputMap* inputMap = nullptr;
	// TODO - This and the other "world" aren't really needed...
	Musa::World ecsWorld;

	bool running = false;
};