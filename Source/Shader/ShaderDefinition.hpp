// Copyright 2020, Nathan Blane

#pragma once

#include <type_traits>

#include "Types/Uncopyable.hpp"
#include "String/String.h"
#include "Path/Path.hpp"
#include "Shader/ShaderStages.hpp"
#include "Shader/ShaderStructure.hpp"
#include "Containers/DynamicArray.hpp"
#include "Utilities/TemplateUtils.hpp"

class ShaderObject;

template <typename... Defines>
using TypesAreStringPairs = typename std::enable_if_t<
	//std::conjunction<
		(sizeof...(Defines) % 2 == 0) &&
		all_convertable_to_v<const tchar*, Defines...>
	//>::value
>;

static uint32 definitionCounter = 0;

// Contains shader meta data and used to uniquely identify a shader
// Similar to the component type
struct ShaderDefinition final : private Uncopyable
{
	using InitializeWithCompiledOutputFunc = ShaderObject* (&)(const ShaderCompiledOutput& compiledOutput);

	template <typename... ShaderDefs>
	ShaderDefinition(ShaderStage stage, const tchar* relativeSourcePath, const tchar* entryPoint, InitializeWithCompiledOutputFunc shaderObjFunc, ShaderDefs... defines);

	inline const ShaderCompilerDefinitions& GetCompilerDefines() const { return definitions; }
	inline ShaderObject* GetCompiledShaderObject(const ShaderCompiledOutput& output) const { return compiledShaderFunc(output); }

	friend uint32 GetHash(const ShaderDefinition* def)
	{
		return def->definitionHash;
	}

private:

	template <typename Def0, typename Def1, typename = TypesAreStringPairs<Def0,Def1>>
	void AddShaderDefines(Def0 def0, Def1 def1);

	template<typename Def0, typename Def1, typename... Defines, typename = TypesAreStringPairs<Def0, Def1>>
	void AddShaderDefines(Def0 def0, Def1 def1, Defines... defines);

	void AddInstanceToDefinitionCaches();

public:
	const Path sourcePath;
	const InitializeWithCompiledOutputFunc compiledShaderFunc;
	const tchar* const shaderEntryName;
	const ShaderStage shaderStage;
	const uint32 definitionHash;

private:
	ShaderCompilerDefinitions definitions;
};

void InitializeShaders();

template <typename ShaderType>
ShaderObject* GetShader()
{
	return GetAssociatedShader(ShaderType::Definition);
}
ShaderObject* GetAssociatedShader(const ShaderDefinition& definition);

const DynamicArray<ShaderDefinition*>& GetDefinitionList();



//////////////////////////////////////////////////////////////////////////

template<typename ...ShaderDefs>
ShaderDefinition::ShaderDefinition(ShaderStage stage, const tchar* relativeSourcePath, const tchar* entryPoint, InitializeWithCompiledOutputFunc shaderObjFunc, ShaderDefs... defines)
	: sourcePath(relativeSourcePath),
	compiledShaderFunc(shaderObjFunc),
	shaderEntryName(entryPoint),
	shaderStage(stage),
	definitionHash(definitionCounter++)
{
	definitions.shaderStage = shaderStage;
	AddInstanceToDefinitionCaches();

	AddShaderDefines(defines...);
}

template<typename Def0, typename Def1, typename>
inline void ShaderDefinition::AddShaderDefines(Def0 def0, Def1 def1)
{
	auto& definitionMap = definitions.definitions;
	definitionMap.Add(def0, def1);
}

template<typename Def0, typename Def1, typename... Defines, typename>
inline void ShaderDefinition::AddShaderDefines(Def0 def0, Def1 def1, Defines... defines)
{
	auto& definitionMap = definitions.definitions;
	definitionMap.Add(def0, def1);
	AddShaderDefines(defines...);
}


#define DECLARE_SHADER()							\
	public:											\
		static const ShaderDefinition Definition 

#define DECLARE_VERTEX_SHADER(RelativeSourcePath, EntryPoint, CompiledFunc, ...)		\
	public:																				\
		static const ShaderDefinition Definition {								\
			ShaderStage::Vertex, RelativeSourcePath, EntryPoint, CompiledFunc,			\
			"VERT_SHADER", "1", __VA_ARGS__}

#define DECLARE_FRAGMENT_SHADER(RelativeSourcePath, EntryPoint, CompiledFunc, ...)		\
	public:																				\
		static const ShaderDefinition Definition;{								\
			ShaderStage::Fragment, RelativeSourcePath, EntryPoint, CompiledFunc,		\
			"FRAG_SHADER", "1", __VA_ARGS__}

#define DEFINE_SHADER(ShaderObjectClass, Stage, RelativeSourcePath, EntryPoint, CompiledFunc, ...)	\
	const ShaderDefinition ShaderObjectClass::Definition(					\
		Stage,																			\
		RelativeSourcePath, EntryPoint,													\
		CompiledFunc, __VA_ARGS__)

#define DEFINE_VERTEX_SHADER(ShaderObjectClass, RelativeSourcePath, EntryPoint, CompiledFunc, ...)	\
	DEFINE_SHADER(ShaderObjectClass, 													\
			ShaderStage::Vertex, RelativeSourcePath,									\
			EntryPoint, CompiledFunc,													\
			"VERT_SHADER", "1", __VA_ARGS__)

#define DEFINE_FRAGMENT_SHADER(ShaderObjectClass, RelativeSourcePath, EntryPoint, CompiledFunc, ...)	\
	DEFINE_SHADER(ShaderObjectClass,													\
			ShaderStage::Fragment, RelativeSourcePath,									\
			EntryPoint, CompiledFunc,													\
			"FRAG_SHADER", "1", __VA_ARGS__)
	

