// Copyright 2020, Nathan Blane

#include "Shader.hpp"
#include "ShaderResourceManager.hpp"

static ShaderResourceManager shaderManager;

ShaderID Shader::FindOrLoadShaderFile(const tchar* shaderFile)
{
	ShaderID id;
	if (!shaderManager.TryFindShaderID(shaderFile, id))
	{
		id = shaderManager.LoadShaderFile(shaderFile);
	}

	return id;
}
