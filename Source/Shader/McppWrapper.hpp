#pragma once

#include "mcpp/mcpp.h"
#include "ShaderStructure.hpp"
#include "Containers/Map.h"

class ShaderPreprocessor
{
public:
	ShaderPreprocessor(ShaderIntermediate& intermediateData);
	ShaderPreprocessor& operator=(const ShaderPreprocessor&) = delete;

	void Preprocess();
	const char* ErrorString() const;
	bool Success() const;
	const char* PreprocessedOutput() const;

private:
	static int Mcpp_Process(void* user_data, const char* filename, const char** out_contents, size_t* out_contents_size);
	file_loader GetLoader();

private:
	Map<String, Array<char>> filePathToContents;
	String preprocessedOutput;
	String preprocessError;
	ShaderIntermediate& intermediate;
	int mcppCode = 0;
};