// Copyright 2020, Nathan Blane

#include "ShaderConstantHandles.hpp"
#include "ShaderTables.hpp"

u16 GetUniformBufferTableIndex(const ShaderConstantNameMap& resourceMap, const String& name)
{
	// get at the ShaderResourceDescription from name
	ShaderResourceInfo desc = resourceMap.GetResourceDescription(name);
	if (desc.type != ShaderResourceType::UniformBuffer)
	{
		MUSA_ERR(ShaderResources, "Resource name {} does not correspond to a UniformBuffer!", *name);
	}

	return desc.tableIndex;
}

u16 GetTextureTableIndex(const ShaderConstantNameMap& resourceMap, const String& name)
{
	// get at the ShaderResourceDescription from name
	ShaderResourceInfo desc = resourceMap.GetResourceDescription(name);
	if (desc.type != ShaderResourceType::UniformBuffer)
	{
		MUSA_ERR(ShaderResources, "Resource name {} does not correspond to a UniformBuffer!", *name);
	}

	return desc.tableIndex;
}