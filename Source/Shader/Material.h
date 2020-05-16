// Copyright 2020, Nathan Blane

#pragma once

#include "PlatformDefinitions.h"
#include "BasicTypes/UniquePtr.hpp"
#include "Math/MathEngine.hpp"
#include "Color.hpp"
#include "Graphics/GraphicsAPIDefinitions.hpp"
#include "Shader/MaterialRenderInfo.hpp"

struct Texture;
struct TextureCube;
struct ShaderResource;
struct MaterialRenderInfo;
class ShaderProgram;
class Matrix4;

class Material
{
public:
	Material();
	Material(ShaderResource& vertShader, ShaderResource& fragShader, const char* textureName, const Color32& color);
	Material(ShaderResource& vertShader, ShaderResource& fragShader, const Texture* tex, const Color32& color);
	/*Material(const tchar* vertexShader, const tchar* fragmentShader, const char* textureName, const Color32& color);*/

	void EnableWireframe();
	void DisableWireframe();

	void SetTexture0(Texture& texture0);
	void SetTexture1(Texture& texture1);
	void SetNormalMap(Texture& normMap);
	void SetCubeTexture(TextureCube* cube);
	void SetColor(const Color32& color);
	void SetShadingModel(ShadingModel model);


	inline ShaderResource* GetVertexShader() const { return vertexShader; }
	inline ShaderResource* GetFragmentShader() const { return fragmentShader; }
	inline const Texture* GetTexture0() const { return texture0; }
	inline Texture* GetTexture1() const { return texture1; }
	inline Texture* GetNormalMap() const { return normalMap; }
	inline TextureCube* GetTextureCube() const { return cubeMap; }
	inline Color32 GetColor() const { return diffuseColor; }
// 	inline float32 GetRoughness() const { return roughness; }
// 	inline float32 GetMetallic() const { return metallic; }

	FillMode GetFillMode() const { return fillMode; }
	ShadingModel GetShadingModel() const { return shadingModel; }
	MaterialRenderInfo& GetMaterialRenderInfo() const { return *materialRendering; }

private:
	// TODO - This creation method must be refactored to be a better way of creation!
	void ConfigureMaterialInfo();

private:
	Color32 diffuseColor;
// 	float32 roughness = 0.f;
// 	float32 metallic = 0.f;

	UniquePtr<MaterialRenderInfo> materialRendering = new MaterialRenderInfo;
	ShaderResource* vertexShader;
	ShaderResource* fragmentShader;
	const Texture* texture0;
	Texture* texture1 = nullptr;
	Texture* normalMap = nullptr;
	TextureCube* cubeMap = nullptr;
	FillMode fillMode = FillMode::Full;
	ShadingModel shadingModel = ShadingModel::Lit;
};