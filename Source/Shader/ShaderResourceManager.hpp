// Copyright 2020, Nathan Blane

#pragma once

#include "Containers/Map.h"
#include "Shader/ShaderID.hpp"

class ShaderResource;

class ShaderResourceManager
{
public:
	ShaderResourceManager() = default;
	~ShaderResourceManager();

	// shaderName is a path locally to the shader, not within the file system
	ShaderID LoadShaderFile(const tchar* shaderName);
	bool TryFindShaderID(const tchar* shaderName, ShaderID& id);
	ShaderResource* FindShaderResource(ShaderID id);
private:
	Map<const tchar*, ShaderID> pathToIdMap;
	Map<ShaderID, ShaderResource*> resourceMap;
};

ShaderResourceManager& GetShaderResourceManager();