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

static NativeUniformBuffer* viewBuffer = nullptr;

struct TextItem
{
	String text;
	Color32 color;
	Vector2 screenPosition;
	float32 scale;
};

DynamicArray<TextItem> screenTextItems;

void AddTextToScreen(const String& text, float32 textScale, const Vector2& screenPosition, const Color32& color)
{
	screenTextItems.Add(TextItem{text, color, screenPosition, textScale});
}

static Vector2 GetStartingWorldFromScreen(const View& view, const Vector2& screenPosition)
{
	Matrix invScreen = Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);//Math::ConstructFastInverseScreenMatrix(view.description.viewport.width, view.description.viewport.height);
	Matrix invOrtho = Math::ConstructFastInverseOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, .1f, 10000.f);
	Vector4 worldPos = Vector4(screenPosition, 0,1) * invScreen * invOrtho;
	return Vector2(worldPos.x, worldPos.y);
}

static Rect GetTextBoxRect(const TextItem& item, Font& font)
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
	}

	TextItem& item = screenTextItems[0];
	String& text = item.text;
	float32 textScale = item.scale;

	const tchar space = 0x20;
	FontID id = StringView("Ariel");
	Font* font = GetLoadedFont(id);

	DynamicArray<PrimitiveVertex> stringVerts;
	stringVerts.Reserve(text.Length() * 4);
	DynamicArray<uint32> stringTris;
	stringTris.Reserve((text.Length() * 3) * 2);

	Rect textBox = GetTextBoxRect(item, *font);

	uint32 startIndex = 0;
	Vector2 currentTextPosition = GetStartingWorldFromScreen(view, item.screenPosition);
	currentTextPosition.y += (font->newlineHeightOffset* textScale - textBox.height);
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

	GraphicsPipelineDescription desc = {};
	renderer.InitializeWithRenderState(desc);
	desc.vertexShader = &GetShader<SimplePrimitiveVert>()->GetNativeShader();
	desc.fragmentShader = &GetShader<SimplePrimitiveFrag>()->GetNativeShader();
	desc.rasterizerDesc = RasterDesc();
	desc.blendingDescs[0] = BlendDesc();
	desc.blendingDescs[1] = BlendDesc();
	desc.blendingDescs[2] = BlendDesc<ColorMask::RGB, BlendOperation::Add, BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha>();
	desc.depthStencilTestDesc = DepthTestDesc();
	desc.topology = PrimitiveTopology::TriangleList;
	desc.vertexInputs = GetVertexInput<PrimitiveVertex>();
	renderer.SetGraphicsPipeline(desc);

	ViewPropertiesBuffer buffer = {};
	buffer.projectionTransform = Math::ConstructOrthographicMatrix(view.description.viewport.width, view.description.viewport.height, 1.f, 10000.f);
	buffer.viewTransform = Matrix(IDENTITY);
	buffer.viewPosition = view.description.origin;
	GetGraphicsInterface().PushBufferData(*viewBuffer, &buffer);


	renderer.SetUniformBuffer(*viewBuffer, 0);
	renderer.SetTexture(*font->fontTexture->gpuResource, *SamplerDesc(), 1);

	renderer.DrawRawIndexed(stringVerts, stringTris, 1);
}
