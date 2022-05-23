
#include "Entry/MusaEntry.hpp"
#include "MusaEditorApp.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderContextUtilities.hpp"
#include "Graphics/ResourceBlob.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Scene/Viewport.hpp"
#include "Graphics/ResourceArray.hpp"

#include "Shader/Shader.hpp"
#include "Shader/ShaderResourceManager.hpp"
#include "Shader/ShaderResource.hpp"

#include "Logging/Sinks/ConsoleWindowSink.hpp"
#include "Logging/Sinks/DebugOutputWindowSink.hpp"

// ImGUI
#include "ImGUI/imgui.h"

MusaApp* CreateMusaApplication(char* /*cmdlineArgs*/)
{
	return new MusaEditorApp;
}

// TODO - Uniform buffer specific for ImGUI
struct ImGuiTransform
{
	Vector2 scale;
	Vector2 translate;
};

struct ImGuiVertex
{
	Vector3 position;
	Vector2 uv;
	Color32 color;
};

// TODO - Vertex specific for ImGui
template <>
inline const VertexInputDescriptionList GetVertexInput<ImDrawVert>()
{
	return VertexInputDescriptionList
	{
		// List
		{{
			{
				VertexInputType::F32_2,		// Type
				sizeof(ImDrawVert),			// Stride
				offsetof(ImDrawVert, pos),	// Offset
				0,							// Binding
				0							// Location
			},

			{
				VertexInputType::F32_2,		// Type
				sizeof(ImDrawVert),			// Stride
				offsetof(ImDrawVert, uv),	// Offset
				0,							// Binding
				1							// Location
			},

			{
				VertexInputType::U32,		// Type
				sizeof(ImDrawVert),			// Stride
				offsetof(ImDrawVert, col),	// Offset
				0,							// Binding
				2							// Location
			}
		}},
		// Count
		3
	};
}

static Viewport* viewport = nullptr;
static NativeVertexBuffer* imguiVertBuffer = nullptr;
static NativeIndexBuffer* imguiIndexBuffer = nullptr;
static NativeUniformBuffer* imguiTransform;
static NativeTexture* nativeFontTex = nullptr;
static NativeSampler* fontSampler = nullptr;
static ShaderID vertShader;
static ShaderID fragShader;
static GraphicsPipelineDescription imguiPipeline;
static bool showDemoWindow = true;

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
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

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

	// init graphics related objects
	viewport = new Viewport(appWindow->GetWindowHandle(), appWindow->GetWidth(), appWindow->GetHeight());
	vertShader = Shader::FindOrLoadShaderFile("ImGuiTransform.mvs");
	fragShader = Shader::FindOrLoadShaderFile("ImGuiRender.mfs");

	SamplerDescription samplerDesc = {};
	samplerDesc.filter = SamplerFilter::Linear;
	samplerDesc.mipMode = SamplerMipmapMode::Linear;
	samplerDesc.addrModeU = SamplerAddressMode::Repeat;
	samplerDesc.addrModeV = SamplerAddressMode::Repeat;
	samplerDesc.minLod = -1000;
	samplerDesc.maxLod = 1000;
	//samplerDesc.maxAnisotropy = 1.f;
	fontSampler = GetGraphicsInterface().CreateTextureSampler(samplerDesc);

	imguiPipeline.vertexInputs = GetVertexInput<ImDrawVert>();
	imguiPipeline.rasterizerDesc = RasterDesc<FillMode::Full, CullingMode::None>();
	imguiPipeline.blendingDescs[0] = BlendDesc<
		ColorMask::RGBA,
		BlendOperation::Add, // Color
		BlendFactor::SrcAlpha,
		BlendFactor::OneMinusSrcAlpha,
		BlendOperation::Add, // Alpha
		BlendFactor::One,
		BlendFactor::OneMinusSrcAlpha>();
	imguiPipeline.depthStencilTestDesc = DepthTestDesc<CompareOperation::None, false>();
	imguiPipeline.topology = PrimitiveTopology::TriangleList;
	imguiPipeline.vertexShader = GetShaderResourceManager().FindShaderResource(vertShader)->GetVertexShader();
	imguiPipeline.fragmentShader = GetShaderResourceManager().FindShaderResource(fragShader)->GetFragmentShader();

	imguiTransform = GetGraphicsInterface().CreateUniformBuffer(sizeof(ImGuiTransform));
	imguiVertBuffer = GetGraphicsInterface().CreateVertexBuffer(sizeof(ImDrawVert));
	imguiIndexBuffer = GetGraphicsInterface().CreateIndexBuffer(sizeof(ImDrawIdx), sizeof(ImDrawIdx));
}

void MusaEditorApp::AppLoop(f32 /*tick*/, const DynamicArray<Input::Event>& frameInputs)
{
	ImGuiIO& io = ImGui::GetIO();

	// Push input to imgui
	for (const auto& event : frameInputs)
	{
		switch (event.type)
		{
			case Input::EventType::Mouse:
				//Input::MouseEvent mouseEvent = event.mouseEvent;
				//mouseEvent.
				break;
			case Input::EventType::Button:
				Input::ButtonEvent button = event.buttonEvent;
				if (Input::IsMouseButtonEvent(button.button))
				{
					io.MouseDown[button.button - Input::Mouse_LeftButton] = button.downState.endedDown;
				}
				break;
			case Input::EventType::Analog:
				break;
			default:
				Assert(false);
				break;
		}
	}

	if (nativeFontTex == nullptr)
	{
		u8* pixels;
		i32 width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		ResourceBlob blob(pixels, width * height * 4);
		nativeFontTex = GetGraphicsInterface().CreateInitializedTexture2D(blob, width, height, ImageFormat::RGBA_8norm, 1, TextureUsage::SampledResource);
	}

	io.DisplaySize = ImVec2((f32)appWindow->GetWidth(), (f32)appWindow->GetHeight());
	IntVector2 mousePos = Input::GetMousePosition();
	io.MousePos = ImVec2((f32)mousePos.x, (f32)mousePos.y);

	ImGui::NewFrame();

	if(showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	ImGui::Render();

	// Actually render to GPU
	NOT_USED ImDrawData* drawData = ImGui::GetDrawData();

	// Update vb/ib with new data
	if (drawData->TotalVtxCount > 0)
	{
		size_t vertexSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
		size_t indexSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);
		if (imguiVertBuffer == nullptr || imguiVertBuffer->size < vertexSize)
		{
			GetGraphicsInterface().DestroyVertexBuffer(imguiVertBuffer);
			imguiVertBuffer = GetGraphicsInterface().CreateVertexBuffer(vertexSize);
		}
		if (imguiIndexBuffer == nullptr || imguiIndexBuffer->size < indexSize)
		{
			GetGraphicsInterface().DestroyIndexBuffer(imguiIndexBuffer);
			imguiIndexBuffer = GetGraphicsInterface().CreateIndexBuffer(indexSize, sizeof(ImDrawIdx));
		}

		ImDrawVert* vertData = reinterpret_cast<ImDrawVert*>(GetGraphicsInterface().LockVertexBuffer(imguiVertBuffer, imguiVertBuffer->size, 0));
		ImDrawIdx* idxData = reinterpret_cast<ImDrawIdx*>(GetGraphicsInterface().LockIndexBuffer(imguiIndexBuffer, imguiIndexBuffer->size, 0));

		// Loop through cmd list from imgui
		for (int i = 0; i < drawData->CmdListsCount; ++i)
		{
			const ImDrawList* cmdList = drawData->CmdLists[i];
			Memcpy(vertData, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
			Memcpy(idxData, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
			vertData += cmdList->VtxBuffer.Size;
			idxData += cmdList->IdxBuffer.Size;
		}

		GetGraphicsInterface().UnlockVertexBuffer(imguiVertBuffer);
		GetGraphicsInterface().UnlockIndexBuffer(imguiIndexBuffer);
	}

	ImGuiTransform transform = {};
	transform.scale.x = 2.f / drawData->DisplaySize.x;
	transform.scale.y = 2.f / drawData->DisplaySize.y;
	transform.translate.x = -1.f - drawData->DisplayPos.x * transform.scale.x;
	transform.translate.y = -1.f - drawData->DisplayPos.y * transform.scale.y;
	GetGraphicsInterface().PushBufferData(*imguiTransform, &transform);

	RenderContext& context = *GetGraphicsInterface().GetRenderContext();

	context.BeginRenderFrame(viewport->GetNativeViewport());

	// Set up back buffer render
	NativeTexture* backBuffer = context.GetBackBuffer();
	NativeRenderTargets backBufferTarget = {};
	backBufferTarget.colorTargets.Add(backBuffer);
	backBufferTarget.extents = { (f32)viewport->GetWidth(), (f32)viewport->GetHeight() };

	TransitionTargetsToWrite(context, backBufferTarget);

	i32 drawWidth = (i32)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
	i32 drawHeight = (i32)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
	if (drawWidth <= 0 || drawHeight <= 0)
		return;

	BeginRenderPassInfo beginInfo = {};
	beginInfo.colorAttachments.Resize(1);
	beginInfo.clearColors.Add(Color32(1.f, .5f, .5f));
	beginInfo.targets = backBufferTarget;

	ColorDescription& colorDesc = beginInfo.colorAttachments[0];
	colorDesc.format = ImageFormat::BGRA_8norm;
	colorDesc.access = RenderTargetAccess::Write;
	colorDesc.op = ColorTargetOperations::Clear_Store;
	colorDesc.sampleCount = 1;

	imguiPipeline.colorAttachments = beginInfo.colorAttachments;

	if (drawData->TotalVtxCount > 0)
	{
		context.BeginRenderPass(beginInfo);
		context.SetViewport(0, 0, drawWidth, drawHeight, 0, 1);
		context.SetGraphicsPipeline(imguiPipeline);
		context.SetVertexBuffer(*imguiVertBuffer);
		context.SetUniformBuffer(*imguiTransform, 0);
		context.SetTexture(*nativeFontTex, *fontSampler, 1);

		ImVec2 clipOffset = drawData->DisplayPos;
		ImVec2 clipScale = drawData->FramebufferScale;
		u32 globalVertOffset = 0;
		u32 globalIdxOffset = 0;
		for (i32 idx = 0; idx < drawData->CmdListsCount; ++idx)
		{
			const ImDrawList* cmdList = drawData->CmdLists[idx];
			for (i32 l = 0; l < cmdList->CmdBuffer.Size; ++l)
			{
				const ImDrawCmd* cmd = &cmdList->CmdBuffer[l];

				ImVec4 clipRect;
				clipRect.x = (cmd->ClipRect.x - clipOffset.x) * clipScale.x;
				clipRect.y = (cmd->ClipRect.y - clipOffset.y) * clipScale.y;
				clipRect.z = (cmd->ClipRect.z - clipOffset.x) * clipScale.x;
				clipRect.w = (cmd->ClipRect.w - clipOffset.y) * clipScale.y;

				if (clipRect.x < drawWidth && clipRect.y < drawHeight && clipRect.z >= 0.f && clipRect.w >= 0.f)
				{
					// Ensure clipRect isn't below 0
					clipRect.x = Math::Max(clipRect.x, 0.f);
					clipRect.y = Math::Max(clipRect.y, 0.f);

					context.SetScissor(
						(u32)(clipRect.x), (u32)(clipRect.y),
						(u32)(clipRect.z - clipRect.x),
						(u32)(clipRect.w - clipRect.y)
					);

					constexpr u32 indicesPerPrimitive = 3;
					u32 numPrimitives = cmd->ElemCount / indicesPerPrimitive;
					context.DrawPrimitiveIndexed(*imguiIndexBuffer, numPrimitives, 1, cmd->IdxOffset + globalIdxOffset, cmd->VtxOffset + globalVertOffset, 0);
				}

			}
			globalVertOffset += cmdList->VtxBuffer.Size;
			globalIdxOffset += cmdList->IdxBuffer.Size;
		}

		context.EndRenderFrame(viewport->GetNativeViewport());
	}
}

void MusaEditorApp::AppDeinit()
{
	ImGui::DestroyContext();

}
