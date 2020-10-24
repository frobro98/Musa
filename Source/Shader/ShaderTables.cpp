// Copyright 2020, Nathan Blane

#include "ShaderTables.hpp"

ShaderConstantTable ConstructShaderConstantTable(ShaderConstantNameMap& nameMap)
{
	ShaderConstantTable table;
	auto& map = nameMap.map;
	for (auto& pair : map)
	{
		ShaderResourceInfo& info = pair.second;
		if (info.type == ShaderResourceType::UniformBuffer)
		{
			info.tableIndex = (u16)table.uniformBufferResourceInfo.Size();
			info.type = ShaderResourceType::UniformBuffer;

			ShaderBindInfo tableInfo;
			tableInfo.bindIndex = info.bindIndex;
			tableInfo.size = info.size;
			table.uniformBufferResourceInfo.Add(tableInfo);
		}
		else if (info.type == ShaderResourceType::TextureSampler)
		{
			info.tableIndex = (u16)table.textureResourceInfo.Size();
			info.type = ShaderResourceType::TextureSampler;

			ShaderBindInfo tableInfo;
			tableInfo.bindIndex = info.bindIndex;
			tableInfo.size = info.size;
			table.textureResourceInfo.Add(tableInfo);
		}
		else
		{
			Assert(false);
		}
	}

	return table;
}

ShaderResourceTable ConstructShaderResourceTable(const ShaderConstantTable& constTable)
{
	ShaderResourceTable table = {};
	table.uniformBufferResourceInfo.Resize(constTable.uniformBufferResourceInfo.Size());
	table.textureResourceInfo.Resize(constTable.textureResourceInfo.Size());

	return table;
}
