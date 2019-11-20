#pragma once

#include "Types/UniquePtr.hpp"
#include "Scene/GameWorld.hpp"
#include "Math/IntVector2.hpp"

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

	void InitializeGraphics();
	void SetupWindowContext(Window& window);

	void InitializeSceneView();

	void RunEngine();

	void StartEngine();
	void StopEngine();

	inline bool ShouldRun() const { return running; }
	inline GameInput& GetGameInput() { return *gameInput; }
	inline IntVector2 GetViewDimensions() const { return IntVector2(viewport->GetWidth(), viewport->GetHeight()); }

	// TODO - This shouldn't really be a function. It should be part of either initialization of a default scene or when loading a scene
	void LoadContent();
	
	void UpdateAndRenderWorld(float32 tick);
	void GatherFrameMetrics();

private:
	UniquePtr<GameWorld> world;
	UniquePtr<Viewport> viewport;
	UniquePtr<GameInput> gameInput;
	UI::Context* uiContext = nullptr;

	bool running = false;
};