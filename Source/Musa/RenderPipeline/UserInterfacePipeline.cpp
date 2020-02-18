#include "UserInterfacePipeline.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Font/FontCache.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/RenderContext.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Scene/ScreenView.hpp"
#include "BatchPrimitives.hpp"
#include "Graphics/UniformBuffers.h"
#include "Math/MatrixUtilities.hpp"
#include "Utilities/CoreUtilities.hpp"
#include "String/String.h"
#include "Containers/Stack.hpp"

#include "Graphics/BatchCollection.hpp"
#include "Debugging/MetricInterface.hpp"
#include "Debugging/ProfilerStatistics.hpp"
#include "Input/Input.hpp"
#include "Visualization/ScreenTextItem.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/RenderContextUtilities.hpp"

#include "Shader/ShaderObjects/ScreenRendering.hpp"


#include "Types/UniquePtr.hpp"
#include "UI/UIContext.hpp"

DECLARE_METRIC_GROUP(TextDisplay);
METRIC_STAT(TextSetup, TextDisplay);
METRIC_STAT(TextRenderSetupCommands, TextDisplay);
METRIC_STAT(TextFormatting, TextDisplay);
METRIC_STAT(TextDisplayRender, TextDisplay);
METRIC_STAT(RenderToScreen, TextDisplay);

static BatchCollection collection;
// TODO - Leak but this shouldn't be global anyways....
static NativeUniformBuffer* viewBuffer = nullptr;

DynamicArray<ScreenTextItem> screenTextItems;
DynamicArray<PrimitiveVertex> stringVerts;
DynamicArray<uint32> stringTris;

static void AddTextToScreen(const tchar* text, float32 textScale, const Vector2& screenPosition, const Color32& color)
{
	screenTextItems.Add(ScreenTextItem{ text, color, screenPosition, textScale });
}

static Vector2 GetStartingWorldFromScreen(const View& view, const Vector2& screenPosition)
{
	Matrix4 invScreen = Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);//Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);
	Matrix4 invOrtho = Math::ConstructFastInverseOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, .1f, 10000.f);
	Vector4 worldPos = Vector4(screenPosition, 0, 1) * invScreen * invOrtho;
	return Vector2(worldPos.x, worldPos.y);
}

static Rect GetTextBoxRect(const ScreenTextItem& item, Font& font)
{
	Rect ret;
	const String& text = item.text;
	for (uint32 i = 0; i < text.Length(); ++i)
	{
		tchar character = text[i];
		const FontCharDescription* charDesc = font.fontCharacterMap.Find(character);
		ret.width += charDesc->advance;
		ret.height = Max(ret.height, charDesc->height * item.scale);
	}

	return ret;
}

static void RenderScreenText(RenderContext& renderer, EngineStatView& statView, const View& view)
{
	if (viewBuffer == nullptr)
	{
		auto buff = GetGraphicsInterface().CreateUniformBuffer(sizeof(ViewPropertiesBuffer));
		viewBuffer = buff.Release();
		ViewPropertiesBuffer buffer = {};
		buffer.projectionTransform = Math::ConstructOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, 1.f, 10000.f);
		buffer.viewTransform = Matrix4(IDENTITY);
		buffer.viewPosition = view.description.origin;
		GetGraphicsInterface().PushBufferData(*viewBuffer, &buffer);
	}
	if (Input::IsPressed(Inputs::Key_F1))
	{
		statView.ToggleStats();
	}

	if (statView.AreStatsVisible())
	{
		constexpr float32 width = 250.f;
		constexpr float32 consoleRectHalfWidth = width / 2.f;
		const float32 consoleRectHalfHeight = view.description.viewport.height * .5f;
		Vector2 topLeft = GetStartingWorldFromScreen(view, Vector2(0, 0));
		BatchedQuadDescription quadDesc = {};
		quadDesc.position = Vector3(topLeft.x + consoleRectHalfWidth, topLeft.y - consoleRectHalfHeight, 0);
		quadDesc.width = width;
		quadDesc.height = view.description.viewport.height;
		quadDesc.color = Color32::Black();
		collection.BatchQuad(quadDesc);

		collection.RenderBatches(renderer, GetShader<SimplePrimitiveVert>()->GetNativeShader(), GetShader<SimplePrimitiveFrag>()->GetNativeShader(), *viewBuffer, *(WhiteTexture()->gpuResource));
	}

	BEGIN_TIMED_BLOCK(TextFormatting);
	statView.PushStatsToView(screenTextItems);
	END_TIMED_BLOCK(TextFormatting);

	if (!screenTextItems.IsEmpty())
	{
		const tchar space = 0x20;
		FontID id = StringView("Ariel");
		Font* font = GetLoadedFont(id);

		uint32 startIndex = 0;

		BEGIN_TIMED_BLOCK(TextSetup);

		for (const auto& item : screenTextItems)
		{
			const String& text = item.text;
			float32 textScale = item.scale;

			Vector2 currentTextPosition = GetStartingWorldFromScreen(view, item.screenPosition);
			for (uint32 i = 0; i < text.Length(); ++i)
			{
				tchar character = text[i];
				FontCharDescription* charDesc = font->fontCharacterMap.Find(character);

				if (charDesc->characterCode != space)
				{
					const uint32 texWidth = font->fontTexture->GetWidth();
					const uint32 texHeight = font->fontTexture->GetHeight();

					const float halfWidth = charDesc->width * textScale * .5f;
					const float halfHeight = charDesc->height * textScale * .5f;

					const float negX = currentTextPosition.x;
					const float posY = currentTextPosition.y - (charDesc->characterHeightOffset * textScale);
					const float posX = negX + (charDesc->width * textScale);
					const float negY = posY - (charDesc->height * textScale);

					const float uNormCoord = charDesc->normTextureCoords.x;
					const float vNormCoord = charDesc->normTextureCoords.y;
					const float uNormSize = charDesc->normCharacterWidth;
					const float vNormSize = charDesc->normCharacterHeight;

					stringVerts.Add(PrimitiveVertex{
						Vector3(negX, posY, 0),
						Vector2(uNormCoord, vNormCoord + vNormSize),
						item.color
						});
					stringVerts.Add(PrimitiveVertex{
						Vector3(negX, negY, 0),
						Vector2(uNormCoord, vNormCoord),
						item.color
						});
					stringVerts.Add(PrimitiveVertex{
						Vector3(posX, posY, 0),
						Vector2(uNormCoord + uNormSize, vNormCoord + vNormSize),
						item.color
						});
					stringVerts.Add(PrimitiveVertex{
						Vector3(posX, negY, 0),
						Vector2(uNormCoord + uNormSize, vNormCoord),
						item.color
						});


					stringTris.Add(startIndex + 0);
					stringTris.Add(startIndex + 1);
					stringTris.Add(startIndex + 2);

					stringTris.Add(startIndex + 2);
					stringTris.Add(startIndex + 1);
					stringTris.Add(startIndex + 3);
					startIndex += 4;
				}

				currentTextPosition.x += (charDesc->advance * textScale);
				// TODO - handle newline character
			}
		}

		END_TIMED_BLOCK(TextSetup);

		GraphicsPipelineDescription desc = {};
		renderer.InitializeWithRenderState(desc);
		desc.vertexShader = &GetShader<SimplePrimitiveVert>()->GetNativeShader();
		desc.fragmentShader = &GetShader<SimplePrimitiveFrag>()->GetNativeShader();
		desc.rasterizerDesc = RasterDesc();
		desc.blendingDescs[0] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha>();
		desc.blendingDescs[1] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha>();
		desc.blendingDescs[2] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha>();
		desc.blendingDescs[3] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha>();
		desc.depthStencilTestDesc = DepthTestDesc<CompareOperation::LessThanOrEqual, false>();
		desc.topology = PrimitiveTopology::TriangleList;
		desc.vertexInputs = GetVertexInput<PrimitiveVertex>();

		BEGIN_TIMED_BLOCK(TextRenderSetupCommands);
		renderer.SetGraphicsPipeline(desc);

		renderer.SetUniformBuffer(*viewBuffer, 0);
		renderer.SetTexture(*font->fontTexture->gpuResource, *SamplerDesc(), 1);

		renderer.DrawRawIndexed(stringVerts, stringTris, 1);
		END_TIMED_BLOCK(TextRenderSetupCommands);

		screenTextItems.Clear();
		stringVerts.Clear();
		stringTris.Clear();
	}
}

//////////////////////////////////////////////////////////////////////////
// Entry to UI Rendering
//////////////////////////////////////////////////////////////////////////

namespace DeferredRender
{

using RenderTargetList = FixedArray<const RenderTarget*, MaxColorTargetCount>;

void RenderUI(RenderContext& renderContext, UI::Context& ui, EngineStatView& statView, const RenderTarget& sceneColorTarget, const RenderTarget& uiRenderTarget, const View& view)
{
	REF_CHECK(renderContext, ui, uiRenderTarget);

	DynamicArray<Color32> clearColors;
	// UI Render
	{
		RenderTargetList uiColorTarget;
		uiColorTarget.Add(&uiRenderTarget);
		RenderTargetDescription targetDescription = CreateRenderTargetDescription(uiColorTarget, nullptr, RenderTargetAccess::None);
		NativeRenderTargets uiRenderTarget = CreateNativeRenderTargets(uiColorTarget, nullptr);

		TransitionTargetsToWrite(renderContext, uiRenderTarget);

		clearColors = { Color32(0, 0, 0, 0) };
		renderContext.SetRenderTarget(targetDescription, uiRenderTarget, clearColors);

		// TODO - This needs to be rendered not to the gbuffer, but to the final resulting image. 
		BEGIN_TIMED_BLOCK(TextDisplayRender);
		RenderScreenText(renderContext, statView, view);
		END_TIMED_BLOCK(TextDisplayRender);

		TransitionTargetsToRead(renderContext, uiRenderTarget);
	}

	// Render to back buffer
	{
		clearColors = { Color32(0, 0, 0) };
		NativeTexture* backBuffer = renderContext.GetBackBuffer();
		NativeRenderTargets backBufferTarget = {};
		backBufferTarget.colorTargets.Add(backBuffer);

		RenderTargetDescription targetDescription = {};
		targetDescription.colorAttachments.Resize(1);
		targetDescription.targetExtents = { (float32)view.description.viewport.width, (float32)view.description.viewport.height };
		targetDescription.hasDepth = false;

		RenderTargetAttachment& colorDesc = targetDescription.colorAttachments[0];
		colorDesc.format = ImageFormat::BGRA_8norm;
		colorDesc.load = LoadOperation::Clear;
		colorDesc.store = StoreOperation::Store;
		colorDesc.stencilLoad = LoadOperation::DontCare;
		colorDesc.stencilStore = StoreOperation::DontCare;
		colorDesc.sampleCount = 1;

		TransitionTargetsToWrite(renderContext, backBufferTarget);

		// Set target to back buffer
		renderContext.SetRenderTarget(targetDescription, backBufferTarget, clearColors);

		// Render sceneColor and UI to back buffer
		{
			SCOPED_TIMED_BLOCK(RenderToScreen);

			// TODO - This should be a pipeline helper function
			uint32 viewWidth = (uint32)view.description.viewport.width;
			uint32 viewHeight = (uint32)view.description.viewport.height;
			renderContext.SetViewport(0, 0, viewWidth, viewHeight, 0, 1);
			renderContext.SetScissor(0, 0, viewWidth, viewHeight);

			GraphicsPipelineDescription desc = {};
			renderContext.InitializeWithRenderState(desc);
			desc.vertexInputs = {};
			desc.rasterizerDesc = { 1.25f, 1.75f, FillMode::Full, CullingMode::Front };
			desc.blendingDescs[0] = BlendDesc<ColorMask::RGBA, BlendOperation::Add, BlendFactor::One, BlendFactor::One, BlendOperation::Add, BlendFactor::One, BlendFactor::One>();
			desc.depthStencilTestDesc = DepthTestDesc();
			desc.topology = PrimitiveTopology::TriangleList;
			desc.vertexShader = &GetShader<ScreenRenderVert>()->GetNativeShader();
			desc.fragmentShader = &GetShader<ScreenRenderFrag>()->GetNativeShader();
			renderContext.SetGraphicsPipeline(desc);

			renderContext.SetTexture(*sceneColorTarget.nativeTarget, *SamplerDesc(), 0);
			renderContext.SetTexture(*uiRenderTarget.nativeTarget, *SamplerDesc(), 1);

			renderContext.Draw(3, 1);
		}

		TransitionTargetsToRead(renderContext, backBufferTarget);
	}



	// Determine if UI needs to render

	// If rendering, set up
	{
		// Need to have some sort of primitive store that gets populated with all of the information per ui widget

		//ui.PrepareUIForRender();

		// With the render primitives and data stored in some way, need to actually set up rendering

		// Set framebuffer to be the one that is used for UI

		// Render to that framebuffer
	}
}
}

