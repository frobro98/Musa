// Copyright 2020, Nathan Blane

#pragma once

#include "String/String.h"
#include "CoreAPI.hpp"

// TODO - figure out namespacing convention for these
// TODO - Add actual Path and Directory creation classes
//namespace Internal
//{
CORE_API String EngineExeFullPath();

inline String EngineRootPath()
{
	return "../../../";
}

inline String EngineAssetPath()
{
	return EngineRootPath() + "Assets/";
}

inline String EngineFontPath()
{
	return EngineAssetPath() + "Fonts/";
}

inline String EngineShaderSrcPath()
{
	return EngineAssetPath() + "Shaders/";
}

inline String EngineGeneratedShaderPath()
{
	return EngineAssetPath() + "Shaders/Generated/";
}

inline String EngineSourcePath()
{
	return EngineRootPath() + "Source/";
}

inline String EngineTexturePath()
{
	return EngineAssetPath() + "Textures/";
}

//}