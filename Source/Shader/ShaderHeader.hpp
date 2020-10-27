// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Serialization/DeserializeBase.hpp"
#include "Serialization/SerializeBase.hpp"
#include "Shader/ShaderTables.hpp"
#include "Shader/ShaderID.hpp"

struct ShaderHeader
{
	ShaderID id;
	// TODO - This doesn't contain the inputs and outputs of the shader. Put this data into the shader header
	ShaderConstantTable resourceTable;
	ShaderConstantNameMap resourceNames;
};

forceinline void Serialize(SerializeBase& ser, const ShaderHeader& shaderHeader)
{
	Serialize(ser, shaderHeader.id.bytecodeHash);
	Serialize(ser, shaderHeader.resourceTable);
	Serialize(ser, shaderHeader.resourceNames);
}

forceinline void Deserialize(DeserializeBase& ser, ShaderHeader& shaderHeader)
{
	Deserialize(ser, shaderHeader.id.bytecodeHash);
	Deserialize(ser, shaderHeader.resourceTable);
	Deserialize(ser, shaderHeader.resourceNames);
}
