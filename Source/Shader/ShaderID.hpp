// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

struct ShaderID
{
	// This should be the path name, not just the file name
	const tchar* shaderName;
	u32 bytecodeHash;
};

forceinline bool operator==(const ShaderID& lhs, const ShaderID& rhs)
{
	return lhs.bytecodeHash == rhs.bytecodeHash &&
		(Strcmp(lhs.shaderName, rhs.shaderName) == 0);
}

forceinline bool operator!=(const ShaderID& lhs, const ShaderID& rhs)
{
	return lhs.bytecodeHash != rhs.bytecodeHash &&
		(Strcmp(lhs.shaderName, rhs.shaderName) != 0);
}

forceinline u32 GetHash(const ShaderID& id)
{
	u32 hash = id.bytecodeHash;
	u32 nameHash = GetHash(id.shaderName);
	HashCombine(hash, nameHash);
	return hash;
}
