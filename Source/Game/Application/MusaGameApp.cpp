// Copyright 2020, Nathan Blane

#include "Application/Musa.hpp"
#include "MusaGameApp.hpp"
#include "Engine/FrameData.hpp"
#include "File/DirectoryLocations.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Logging/LogFunctions.hpp"
#include "Logging/Sinks/ConsoleWindowSink.hpp"
#include "Logging/Sinks/DebugOutputWindowSink.hpp"
#include "Logging/Sinks/LogFileSink.hpp"

DEFINE_LOG_CHANNEL(GameAppLog);

constexpr i32 windowWidth = 1080;
constexpr i32 windowHeight = 720;

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaGameApp();
}

static void InitAppLogging()
{
	// TODO - This should be named whatever the game name is determined to be
	Path logFilePath = Path(EngineLogPath()) / "musa.log";

#if M_DEBUG
	GetLogger().InitLogging(LogLevel::Debug);
#else
	GetLogger().InitLogging(LogLevel::Info);
#endif

	GetLogger().AddLogSink(new ConsoleWindowSink);
	GetLogger().AddLogSink(new DebugOutputWindowSink);
	GetLogger().AddLogSink(new LogFileSink(logFilePath));

	MUSA_INFO(GameAppLog, "Launching Musa Application...");
}

void MusaGameApp::AppInit()
{
	InitAppLogging();
	Assert(gApp == this);

	MUSA_INFO(GameAppLog, "Initializing Graphics Layer");
	GetGraphicsInterface().InitializeGraphics();

	InitializeOSInput();
	InitializeApplicationWindow();

	gameEngine = MakeUnique<MusaEngine>(*uiContext, *inputRouter);
	gameEngine->SetInputHandler(inputMap);

	gameEngine->StartupEngine(*appWindow);

	gameEngine->LoadContent();
}

void MusaGameApp::AppLoop(f32 tick, const DynamicArray<Input::Event>& /*frameInputs*/)
{
	// Process Inputs that happened this frame


	gameEngine->UpdateAndRender(tick);

	gameEngine->GatherFrameMetrics();
}

void MusaGameApp::AppDeinit()
{
	gameEngine->UnloadContent();
	gameEngine->ShutdownEngine();
	gameEngine.Reset();

	// TODO - image views are trying to be destroyed when still in use by command buffer in flight...
	GetGraphicsInterface().DeinitializeGraphics();
}

