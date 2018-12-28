#pragma once

#include "PlatformDefinitions.h"
#include "Math/MathEngine.h"
#include "Graphics.h"
#include "Texture/Color.hpp"

struct Texture;
class ShaderProgram;
class Matrix;

// TODO - Remove all vulkan from Material.h/.cpp
class VulkanPipeline;

class Material
{
public:
	Material();
	Material(const char* pipelineName, const char* textureName, const Color& color);
	~Material() = default;

	void EnableWireframe();
	void DisableWireframe();

	void SetPipeline(VulkanPipeline* pipeline_);
	void SetTexture0(Texture* texture0);
	void SetTexture1(Texture* texture1);
	void SetTexture2(Texture* texture2);
	void SetColor(const Color& color);

	Texture* GetTexture0() const { return texture0; }
	Texture* GetTexture1() const { return texture1; }
	VulkanPipeline* GetPipeline() const { return pipeline; }
	Color GetColor() const { return diffuseColor; }

private:

	int32 colorUniform;
	int32 modelUniform;
	int32 viewUniform;
	int32 projUniform;
	int32 viewPosUniform;

	Color diffuseColor;

	// TODO - The vulkan pipeline doesn't belong here...
	VulkanPipeline* pipeline;
	Texture* texture0;
	Texture* texture1 = nullptr;
	Texture* texture2 = nullptr;

	uint32 pad[2];
};