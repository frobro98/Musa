// Copyright 2020, Nathan Blane

#include "MaterialShader.hpp"
#include "ShaderResource.hpp"

void MaterialShader::Initialize(ShaderResource& vertexShader, ShaderResource& fragmentShader)
{
	shaderResources[ShaderStage::Vertex] = &vertexShader;
	shaderResources[ShaderStage::Fragment] = &fragmentShader;
	ConstructResourceTable();
}

void MaterialShader::ConstructResourceTable()
{
	ShaderResource* vertexShaderResource = shaderResources[ShaderStage::Vertex];
	ShaderResource* fragmentShaderResource = shaderResources[ShaderStage::Fragment];
	resourceTable.resourceStores[ShaderStage::Vertex] = ConstructShaderResourceTable(vertexShaderResource->GetResourceTable());
	resourceTable.resourceStores[ShaderStage::Fragment] = ConstructShaderResourceTable(fragmentShaderResource->GetResourceTable());
}
