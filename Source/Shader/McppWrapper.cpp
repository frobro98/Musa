#include "McppWrapper.hpp"
#include "Assertion.h"
#include "String/String.h"
#include "DirectoryLocations.h"
#include "File/FileCore/File.h"

void AddDefinition(String& definitions, const String& macro, const String& macroDef)
{
	definitions = "-D";
	definitions += macro + "=" + macroDef;
}

ShaderPreprocessor::ShaderPreprocessor(ShaderIntermediate& intermediateData)
	: intermediate(intermediateData)
{
}

void ShaderPreprocessor::Preprocess()
{
	const char* filename = "/test.vert";
	String file = EngineShaderSrcPath();
	file += filename;
	String options;

	ansichar* output = nullptr;
	ansichar* error = nullptr;
	mcppCode = mcpp_run(*options, *file, &output, &error, GetLoader());

	printf("%s\n", output);
	preprocessedOutput = output;
	if (error)
	{
		preprocessError = error;
	}

	delete output;
	delete error;
}

const char* ShaderPreprocessor::ErrorString() const
{
	return *preprocessError;
}

bool ShaderPreprocessor::Success() const
{
	return mcppCode == 0;
}

const char* ShaderPreprocessor::PreprocessedOutput() const
{
	return *preprocessedOutput;
}

int ShaderPreprocessor::Mcpp_Process(void* userData, const char* filename, const char** outContents, size_t* outContentsSize)
{
	ShaderPreprocessor* preprocessor = reinterpret_cast<ShaderPreprocessor*>(userData);
	auto& fileContentsMap = preprocessor->filePathToContents;
	String path(filename);
	Array<char> fileContents;

	// TODO - This copies the data from one array to the other...consider adding callback functionality
	bool found = fileContentsMap.TryFind(path, fileContents);
	if (!found)
	{
		File::Handle shaderFile = 0;
		auto result = File::Open(shaderFile, filename, File::Mode::READ);
		Assert(result == File::Result::SUCCESS);

		uint32 size;
		File::Size(shaderFile, size);
		fileContents.Resize(size+1);

		result = File::Read(shaderFile, fileContents.GetData(), size);
		Assert(result == File::Result::SUCCESS);

		result = File::Close(shaderFile);
		Assert(result == File::Result::SUCCESS);

		fileContents[size] = 0;
		fileContentsMap.Add(path, fileContents);
	}

	if (outContents)
	{
		*outContents = fileContents.Size() > 0 ? fileContents.GetData() : nullptr;
	}
	if (outContentsSize)
	{
		*outContentsSize = fileContents.Size();
	}

	return false;
}

file_loader ShaderPreprocessor::GetLoader()
{
	file_loader mcppLoader = {};
	mcppLoader.user_data = this;
	mcppLoader.get_file_contents = &ShaderPreprocessor::Mcpp_Process;
	return mcppLoader;
}
