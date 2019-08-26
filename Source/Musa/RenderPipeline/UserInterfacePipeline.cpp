#include "UserInterfacePipeline.hpp"
#include "Mesh/GeometryPrimitives.h"
#include "Font/FontCache.hpp"
#include "Texture/Texture2D/Texture.h"
#include "Graphics/Renderer.hpp"
#include "Graphics/ResourceArray.hpp"
#include "Graphics/ResourceInitializationDescriptions.hpp"
#include "Shader/ShaderObjects/SimplePrimitiveRendering.hpp"
#include "Scene/ScreenView.hpp"


void RenderTextOnScreen(Renderer& renderer, const View& view, const String& text)
{
	const tchar space = 0x20;
	FontID id = StringView("Ariel");
	Font* font = GetLoadedFont(id);

	DynamicArray<PrimitiveVertex> stringVerts;
	stringVerts.Reserve(text.Length() * 4);
	DynamicArray<uint32> stringTris;
	stringTris.Reserve((text.Length() * 3) * 2);

	uint32 startIndex = 0;
	Vector2 currentTextPosition;
	for (uint32 i = 0; i < text.Length(); ++i)
	{
		tchar character = text[i];
		FontCharDescription* charDesc = font->fontCharacterMap.Find(character);
		
		if (charDesc->characterCode != space)
		{
			const float fontTexWidth = (float32)font->fontTexture->mipLevels[0].width;
			const float fontTexHeight = (float32)font->fontTexture->mipLevels[0].height;
			const float negX = currentTextPosition.x;
			const float posY = currentTextPosition.y - charDesc->characterHeightOffset;
			const float posX = negX + charDesc->texelWidth;
			const float negY = posY - charDesc->texelHeight;

			const float uNormCoord = charDesc->uTexelStart / fontTexWidth;
			const float vNormCoord = charDesc->vTexelStart / fontTexHeight;
			const float uNormSize = charDesc->texelWidth / fontTexWidth;
			const float vNormSize = charDesc->texelHeight / fontTexHeight;

			stringVerts.Add(PrimitiveVertex{
				Vector3(negX, posY, 0),
				Vector2(uNormCoord, vNormCoord + vNormSize),
				Color32::White()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(negX, negY, 0),
				Vector2(uNormCoord, vNormCoord),
				Color32::White()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(posX, posY, 0),
				Vector2(uNormCoord + uNormSize, vNormCoord + vNormSize),
				Color32::White()
				});
			stringVerts.Add(PrimitiveVertex{
				Vector3(posX, negY, 0),
				Vector2(uNormCoord + uNormSize, vNormCoord),
				Color32::White()
				});

			
			stringTris.Add(startIndex + 0);
			stringTris.Add(startIndex + 1);
			stringTris.Add(startIndex + 2);

			stringTris.Add(startIndex + 2);
			stringTris.Add(startIndex + 1);
			stringTris.Add(startIndex + 3);
			startIndex += 4;
		}

		currentTextPosition.x += charDesc->texelWidth;
		// TODO - handle newline character
	}

	GraphicsPipelineDescription desc = {};
	renderer.InitializeWithRenderState(desc);
	desc.vertexShader = &GetShader<SimplePrimitiveVert>()->GetNativeShader();
	desc.fragmentShader = &GetShader<SimplePrimitiveFrag>()->GetNativeShader();
	desc.rasterizerDesc = RasterDesc();
	desc.blendingDescs[0] = BlendDesc();// <ColorMask::RGB, BlendOperation::Add, >()
	desc.blendingDescs[1] = BlendDesc();
	desc.blendingDescs[2] = BlendDesc();
	desc.depthStencilTestDesc = DepthTestDesc();
	desc.topology = PrimitiveTopology::TriangleList;
	desc.vertexInputs = GetVertexInput<PrimitiveVertex>();
	renderer.SetGraphicsPipeline(desc);

	renderer.SetUniformBuffer(*view.viewBuffer, 0);
	renderer.SetTexture(*font->fontTexture->gpuResource, *SamplerDesc(), 1);

	renderer.DrawRawIndexed(stringVerts, stringTris, 1);
}
