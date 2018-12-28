#pragma once

#include "String/String.h"

// TODO - figure out namespacing convention for these
// TODO - Add actual Path and Directory creation classes
//namespace Internal
//{
String EngineExeFullPath();

inline String EngineRootPath()
{
	return "../../../";
}

inline String EngineAssetPath()
{
	return EngineRootPath() + "Assets/";
}

inline String EngineSourcePath()
{
	return EngineRootPath() + "Source/";
}

inline String EngineShaderSrcPath()
{
	return EngineSourcePath() + "Shader/ShaderSrc/";
}

inline String EngineTexturePath()
{
	return EngineAssetPath() + "Textures/";
}

//}