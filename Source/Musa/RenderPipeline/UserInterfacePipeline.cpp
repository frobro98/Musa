#include "UserInterfacePipeline.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Font/FontCache.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Shader/ShaderObjects/FontRendering.hpp"
#include "Scene/ScreenView.hpp"
#include "BatchPrimitives.hpp"
#include "Graphics/UniformBuffers.h"
#include "Math/MatrixUtilities.hpp"

static NativeUniformBuffer* viewBuffer = nullptr;

void RenderTextOnScreen(Renderer& renderer, const View& view, const String& text, float textScale)
{
	if (viewBuffer == nullptr)
	{
		viewBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(ViewPropertiesBuffer));
	}

	textScale = 1.f;

	const tchar space = 0x20;
	FontID id = StringView("Ariel");
	Font* font = GetLoadedFont(id);

	DynamicArray<PrimitiveVertex> stringVerts;
	stringVerts.Reserve(text.Length() * 4);
	DynamicArray<uint32> stringTris;
	stringTris.Reserve((text.Length() * 3) * 2);

	uint32 startIndex = 0;
	Vector2 currentTextPosition(0, 0);
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

// 			BatchedQuadDescription desc = {};
// 			desc.color = Color32::Black();
// 			desc.position = Vector3(posX - halfWidth, posY - halfHeight, 0);
// 			desc.width = charDesc->width * textScale;
// 			desc.height = charDesc->height * textScale;
// 			BatchWireQuadPrimitive(desc);

			stringVerts.Add(PrimitiveVertex{
				Vector3(negX, posY, 0),
				Vector2(uNormCoord, vNormCoord + vNormSize),
				Color32::Red()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(negX, negY, 0),
				Vector2(uNormCoord, vNormCoord),
				Color32::Red()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(posX, posY, 0),
				Vector2(uNormCoord + uNormSize, vNormCoord + vNormSize),
				Color32::Red()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(posX, negY, 0),
				Vector2(uNormCoord + uNormSize, vNormCoord),
				Color32::Red()
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

	const float32 halfWidth = view.description.viewport.width * .5f;
	const float32 halfHeight = view.description.viewport.height * .5f;

	ViewPropertiesBuffer buffer = {};
	buffer.projectionTransform = Math::ConstructOrthographicMatrix(-halfWidth, halfWidth, -halfHeight, halfHeight, 1.f, 10000.f);
	buffer.viewTransform = view.transforms.viewMatrix;//Matrix(IDENTITY);
	buffer.viewPosition = view.description.origin;
	GetGraphicsInterface().PushBufferData(*viewBuffer, &buffer);


	renderer.SetUniformBuffer(*view.viewBuffer, 0);
	renderer.SetTexture(*font->fontTexture->gpuResource, *SamplerDesc(), 1);

	renderer.DrawRawIndexed(stringVerts, stringTris, 1);
}
