// Copyright 2020, Nathan Blane

#include "ShaderResourceManager.hpp"
#include "ShaderResource.hpp"



ShaderResourceManager::~ShaderResourceManager()
{

}

ShaderID ShaderResourceManager::LoadShaderFile(const tchar* shaderName)
{
	Assert(pathToIdMap.Find(shaderName) == nullptr);

	return ShaderID{};
}

bool ShaderResourceManager::TryFindShaderID(const tchar* shaderName, ShaderID& id)
{
	ShaderID* foundID = pathToIdMap.Find(shaderName);
	if (foundID)
	{
		id = *foundID;
	}
	return foundID != nullptr;
}

ShaderResource* ShaderResourceManager::FindShaderResource(ShaderID id)
{
	ShaderResource** res = resourceMap.Find(id);
	return res != nullptr ? *res : nullptr;
}

ShaderResourceManager& GetShaderResourceManager()
{
	static ShaderResourceManager shaderManager;
	return shaderManager;
}
