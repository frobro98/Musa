
#include "Entry/MusaEntry.hpp"
#include "MusaEditorApp.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderContextUtilities.hpp"
#include "Graphics/ResourceBlob.hpp"
#include "Scene/Viewport.hpp"

#include "Logging/Sinks/ConsoleWindowSink.hpp"
#include "Logging/Sinks/DebugOutputWindowSink.hpp"

// ImGUI
#include "ImGUI/imgui.h"

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaEditorApp;
}

static Viewport* viewport = nullptr;
static UniquePtr<NativeTexture> nativeFontTex;

void MusaEditorApp::AppInit()
{
	GetLogger().InitLogging(LogLevel::Debug);
	GetLogger().AddLogSink(new ConsoleWindowSink);
	GetLogger().AddLogSink(new DebugOutputWindowSink);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendPlatformName = "Musa Editor";
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;

	io.KeyMap[ImGuiKey_Tab] = Input::Key_Tab;
	io.KeyMap[ImGuiKey_LeftArrow] = Input::Key_ArrowLeft;
	io.KeyMap[ImGuiKey_RightArrow] = Input::Key_ArrowRight;
	io.KeyMap[ImGuiKey_UpArrow] = Input::Key_ArrowUp;
	io.KeyMap[ImGuiKey_DownArrow] = Input::Key_ArrowDown;
	io.KeyMap[ImGuiKey_PageUp] = Input::Key_PageUp;
	io.KeyMap[ImGuiKey_PageDown] = Input::Key_PageDown;
	io.KeyMap[ImGuiKey_Home] = Input::Key_Home;
	io.KeyMap[ImGuiKey_End] = Input::Key_End;
	io.KeyMap[ImGuiKey_Insert] = Input::Key_Insert;
	io.KeyMap[ImGuiKey_Delete] = Input::Key_Delete;
	io.KeyMap[ImGuiKey_Backspace] = Input::Key_Backspace;
	io.KeyMap[ImGuiKey_Space] = Input::Key_Space;
	io.KeyMap[ImGuiKey_Enter] = Input::Key_Enter;
	io.KeyMap[ImGuiKey_Escape] = Input::Key_Escape;
	io.KeyMap[ImGuiKey_KeyPadEnter] = Input::Key_NumEnter;
	io.KeyMap[ImGuiKey_A] = Input::Key_A;
	io.KeyMap[ImGuiKey_C] = Input::Key_C;
	io.KeyMap[ImGuiKey_V] = Input::Key_V;
	io.KeyMap[ImGuiKey_X] = Input::Key_X;
	io.KeyMap[ImGuiKey_Y] = Input::Key_Y;
	io.KeyMap[ImGuiKey_Z] = Input::Key_Z;

	GetGraphicsInterface().InitializeGraphics();

	Assert(gApp == this);

	InitializeOSInput();
	InitializeApplicationWindow();

	viewport = new Viewport(appWindow->GetWindowHandle(), appWindow->GetWidth(), appWindow->GetHeight());
}

void MusaEditorApp::AppLoop(f32 /*tick*/, const DynamicArray<Input::Event>& /*frameInputs*/)
{
	// Push input to imgui
// 	for (const auto& event : frameInputs)
// 	{
// 
// 	}

	ImGuiIO& io = ImGui::GetIO();
	if (!nativeFontTex.IsValid())
	{
		u8* pixels;
		i32 width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		ResourceBlob blob(pixels, width * height * 4);
		nativeFontTex = GetGraphicsInterface().CreateInitializedTexture2D(blob, width, height, ImageFormat::RGBA_8u, 1, TextureUsage::SampledResource);
	}

	io.DisplaySize = ImVec2((f32)appWindow->GetWidth(), (f32)appWindow->GetHeight());
	IntVector2 mousePos = Input::GetMousePosition();
	io.MousePos = ImVec2((f32)mousePos.x, (f32)mousePos.y);

	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	ImGui::Render();

	RenderContext& context = *GetGraphicsInterface().GetRenderContext();

	context.BeginRenderFrame(viewport->GetNativeViewport());

	// Set up back buffer render
	NativeTexture* backBuffer = context.GetBackBuffer();
	NativeRenderTargets backBufferTarget = {};
	backBufferTarget.colorTargets.Add(backBuffer);
	
	DynamicArray<Color32> clearColors = { Color32(.5f, .5f, .5f) };

	RenderTargetDescription targetDescription = {};
	targetDescription.colorAttachments.Resize(1);
	targetDescription.targetExtents = { (f32)viewport->GetWidth(), (f32)viewport->GetHeight() };
	targetDescription.hasDepth = false;

	RenderTargetAttachment& colorDesc = targetDescription.colorAttachments[0];
	colorDesc.format = ImageFormat::BGRA_8norm;
	colorDesc.load = LoadOperation::Clear;
	colorDesc.store = StoreOperation::Store;
	colorDesc.stencilLoad = LoadOperation::DontCare;
	colorDesc.stencilStore = StoreOperation::DontCare;
	colorDesc.sampleCount = 1;

	TransitionTargetsToWrite(context, backBufferTarget);

	context.SetRenderTarget(targetDescription, backBufferTarget, clearColors);
	context.SetViewport(0, 0, viewport->GetWidth(), viewport->GetHeight(), 0, 1);
	context.SetScissor(0, 0, viewport->GetWidth(), viewport->GetHeight());

	// Actually render to GPU
	NOT_USED ImDrawData* drawData = ImGui::GetDrawData();

	context.EndRenderFrame(viewport->GetNativeViewport());
}

void MusaEditorApp::AppDeinit()
{
	ImGui::DestroyContext();

}