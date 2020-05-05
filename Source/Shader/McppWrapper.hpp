#pragma once

#include "mcpp/mcpp.h"
#include "ShaderStructure.hpp"
#include "Containers/Map.h"
#include "Types/Uncopyable.hpp"

class Path;

class ShaderPreprocessor : private Uncopyable
{
public:
	ShaderPreprocessor(ShaderStage stage, const Map<String, String>& definitions);

	void Preprocess(const Path& pathToShader);
	const char* ErrorString() const;
	bool Success() const;
	const char* PreprocessedOutput() const;

private:
	static int Mcpp_Process(void* user_data, const char* filename, const char** out_contents, size_t* out_contents_size);
	file_loader GetLoader();

private:
	Map<String, DynamicArray<char>> filePathToContents;
	
	String preprocessedOutput;
	String preprocessError;
	const Map<String, String>& preprocessorDefinitions;
	ShaderStage shaderStage;
	int mcppCode = 0;
};