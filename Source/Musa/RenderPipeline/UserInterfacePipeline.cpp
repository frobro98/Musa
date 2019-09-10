#include "UserInterfacePipeline.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Font/FontCache.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/Renderer.hpp"
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

#include "Debugging/MetricInterface.hpp"
#include "Debugging/ProfilerStatistics.hpp"

DECLARE_METRIC_GROUP(TextDisplay);
METRIC_STAT(TextSetup, TextDisplay);
METRIC_STAT(TextRenderSetupCommands, TextDisplay);

static NativeUniformBuffer* viewBuffer = nullptr;

struct TextItem
{
	String text;
	Color32 color;
	Vector2 screenPosition;
	float32 scale;
};

DynamicArray<TextItem> screenTextItems;
DynamicArray<PrimitiveVertex> stringVerts;
DynamicArray<uint32> stringTris;

void AddTextToScreen(const tchar* text, float32 textScale, const Vector2& screenPosition, const Color32& color)
{
	screenTextItems.Add(TextItem{text, color, screenPosition, textScale});
}

void FormatDebugText()
{
	constexpr uint32 formattedStrLen = 256;
	Vector2 currentScreenPos(2, 2);
	const ProfiledFrameMark& frame = GetProfilingStatistics().GetPreviousFrame();

	for (auto& frameMetric : frame.frameMetrics)
	{
		tchar formattedString[formattedStrLen];
		snprintf(formattedString, formattedStrLen,
			"%s: %5.02fms"
			//" in %50s: line %4d"
			, frameMetric.metricName, frameMetric.totalMetricTimeMS
			//, entry.filename, entry.lineCount
		);
		AddTextToScreen(formattedString, .15f, currentScreenPos, Color32::White());

		currentScreenPos.y += 20;
	}
}

static Vector2 GetStartingWorldFromScreen(const View& view, const Vector2& screenPosition)
{
	Matrix invScreen = Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);//Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);
	Matrix invOrtho = Math::ConstructFastInverseOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, .1f, 10000.f);
	Vector4 worldPos = Vector4(screenPosition, 0,1) * invScreen * invOrtho;
	return Vector2(worldPos.x, worldPos.y);
}

 Rect GetTextBoxRect(const TextItem& item, Font& font)
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

void RenderText(Renderer& renderer, const View& view)
{
	if (viewBuffer == nullptr)
	{
		viewBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(ViewPropertiesBuffer));
		ViewPropertiesBuffer buffer = {};
		buffer.projectionTransform = Math::ConstructOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, 1.f, 10000.f);
		buffer.viewTransform = Matrix(IDENTITY);
		buffer.viewPosition = view.description.origin;
		GetGraphicsInterface().PushBufferData(*viewBuffer, &buffer);
	}

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
