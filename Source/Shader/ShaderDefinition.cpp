// Copyright 2020, Nathan Blane

#include "ShaderDefinition.hpp"
#include "ShaderCompiler.h"
#include "DirectoryLocations.h"
#include "Shader/ShaderObjects/ShaderObject.hpp"

namespace 
{
// TODO - Don't want this to be a dynamic array, because we don't know how many elements there will be
// Could be wasting memory?
static DynamicArray<ShaderDefinition*>* shaderDefinitions = nullptr;
static Map<const ShaderDefinition*, ShaderObjectBase*> shaderDefinitionMap;

static void CompileAssociatedShader(const ShaderDefinition& definition)
{
	if (ShaderObjectBase* shader = nullptr; !shaderDefinitionMap.TryFind(&definition, shader))
	{
		const Path fullShaderPath = EngineShaderSrcPath() / definition.sourcePath;
		ShaderCompilerDefinitions input = definition.GetCompilerDefines();
		ShaderStructure output;
		bool succeeded = Compile(fullShaderPath.GetString(), definition.shaderEntryName, input, output);
		if (!succeeded)
		{
			Assert(false);
		}

		shader = definition.GetCompiledShaderObject(output.compiledOutput);
		shaderDefinitionMap.Add(&definition, shader);
	}
}

}

void InitializeShaders()
{
	InitializeCompiler();
	for (const auto& shaderDefinition : *shaderDefinitions)
	{
		CompileAssociatedShader(*shaderDefinition);
	}
	DeinitializeCompiler();
}

ShaderObjectBase* GetAssociatedShader(const ShaderDefinition& definition)
{
	ShaderObjectBase** obj = shaderDefinitionMap.Find(&definition);
	Assert(obj);
	return *obj;
}

const DynamicArray<ShaderDefinition*>& GetDefinitionList()
{
	if (shaderDefinitions == nullptr)
	{
		shaderDefinitions = new DynamicArray<ShaderDefinition*>();
	}

	return *shaderDefinitions;
}

void ShaderDefinition::AddInstanceToDefinitionCaches()
{
	if (shaderDefinitions == nullptr)
	{
		shaderDefinitions = new DynamicArray<ShaderDefinition*>();
	}

	shaderDefinitions->Add(this);
}
