// Copyright 2020, Nathan Blane

#include <stdio.h>

#include "Path/Path.hpp"
#include "String/CStringUtilities.hpp"
#include "File/FileSys.hpp"

#include "Shader/ShaderCompiler.h"
#include "Shader/ShaderStructure.hpp"
#include "Shader/ShaderStages.hpp"

constexpr const char* helpString =
"##-- Shader Compiler --##                                                        \n"
"    Usage: ShaderCompiler [Options] -f path/to/glsl_shader                       \n"
"                                                                                 \n"
"    Options:                                                                     \n"
"          -h               :: prints out compiler usage                          \n"
"          -p               :: preprocesses the glsl shader and saves the output  \n"
"          -o [output_file] :: designates an output file. If left out, this will  \n"
"                              default to \"filename.stage.spv\"                  \n"
"          -d def_name val  :: adds a preprocessor definition to be used when     \n"
"                              compiling the shader                               \n";

#define FILE_CHECK(x, errStr)						\
		do {										\
			if((!(x)))								\
			{										\
				printf(errStr);						\
				exit(1);							\
			}										\
		} while (false)

#define FILE_CHECK_VA(x, errStr, ...)					\
		do {										\
			if((!(x)))								\
			{										\
				printf(errStr, (__VA_ARGS__));		\
				exit(1);							\
			}										\
		} while (false)

static ShaderStage DetermineStageBasedOnExtension(const String& ext)
{
	if (ext == "vert")
	{
		return ShaderStage::Vertex;
	}
	else if (ext == "frag")
	{
		return ShaderStage::Fragment;
	}
	else if (ext == "geom")
	{
		return ShaderStage::Geometry;
	}
	else if (ext == "comp")
	{
		return ShaderStage::Compute;
	}
	else
	{
		printf("Invalid extension! Couldn't associate shader stage with \"%s\"\n", *ext);
	}

	return static_cast<ShaderStage>(-1);
}

static const char* DetermineSPVExtensionFrom(ShaderStage stage)
{
	switch (stage)
	{
		case ShaderStage::Vertex:
			return "vs.spv";
		case ShaderStage::Fragment:
			return "fs.spv";
		case ShaderStage::Geometry:
			return "gs.spv";
		case ShaderStage::TessalationEval:
			return "te.spv";
		case ShaderStage::TessalationControl:
			return "tc.spv";
		case ShaderStage::Compute:
			return "cs.spv";
		default:
			return "";
	}
}

static void SaveSPVInformation(const Path& outputShaderFile, const ShaderStructure& shaderInfo)
{
	File::Handle h;
	File::Result result = File::Open(h, outputShaderFile.GetString(), File::Mode::WRITE);
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to open file %s\n", outputShaderFile.GetString());

	result = File::Write(h, shaderInfo.compiledOutput.shaderCode.GetData(), sizeof(u32) * (u32)shaderInfo.compiledOutput.shaderCode.Size());
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to write file %s\n", outputShaderFile.GetString());

	result = File::Close(h);
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to close file %s\n", outputShaderFile.GetString());
}

static void SavePreprocessedGLSL(const Path& outputShaderFile, const PreprocessedShaderOutput& output)
{
	File::Handle h;
	File::Result result = File::Open(h, outputShaderFile.GetString(), File::Mode::WRITE);
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to open file %s\n", outputShaderFile.GetString());

	result = File::Write(h, *output.outputGlsl, output.outputGlsl.Length());
	if (result != File::Result::SUCCESS)
	{
		u32 err = GetLastError();
		printf("%u\n", err);
	}
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to write file %s\n", outputShaderFile.GetString());

	result = File::Close(h);
	FILE_CHECK_VA(result == File::Result::SUCCESS, "Error when trying to close file %s\n", outputShaderFile.GetString());
}

static void PrintHelp()
{
	printf("%s", helpString);
}

i32 main(i32 argc, char *argv[])
{
	if (argc < 2)
	{
		printf("You must pass in arguments describing what shaders to compile!\n\n");
		PrintHelp();
		return -1;
	}

// 	bool status = true;
// 	while (status) {}

	Path shaderPath;
	const char* inputFile = nullptr;
	const char* outputFile = nullptr;
	bool printHelp = false;
	bool preprocessOnly = false;
	Map<String, String> preprocessorDefinitions;
	for (i32 i = 1; i < argc; ++i)
	{
		if (Strcmp("-f", argv[i]) == 0 || Strcmp("-F", argv[i]) == 0)
		{
			++i;
			const tchar* s = Strchr(argv[i], '-');
			if (i >= argc || (s != nullptr && strlen(s) == 2))
			{
				printf("You must pass a correct type when using the -f option!\n");
				return -1;
			}

			inputFile = argv[i];
		}
		else if (Strcmp("-o", argv[i]) == 0 || Strcmp("-O", argv[i]) == 0)
		{
			++i;
			const tchar* s = Strchr(argv[i], '-');
			if (i >= argc || (s != nullptr && strlen(s) == 2))
			{
				printf("You must pass a correct type when using the -o option!\n");
				return -1;
			}
			outputFile = argv[i];
		}
		else if (Strcmp("-p", argv[i]) == 0 || Strcmp("-P", argv[i]) == 0)
		{
			preprocessOnly = true;
		}
		else if (Strcmp("-d", argv[i]) == 0 || Strcmp("-D", argv[i]) == 0)
		{
			++i;
			constexpr i32 numDefArgs = 2;
			for (i32 arg = i; arg < numDefArgs; ++arg)
			{
				const tchar* s = Strchr(argv[arg], '-');
				if (i >= argc || (s != nullptr && strlen(s) == 2))
				{
					printf("Invalid preprocessor definition argument \"%s\"\n", argv[arg]);
					return -1;
				}
			}

			preprocessorDefinitions.Add(argv[i], argv[i + 1]);
			++i;
		}
		else if (Strcmp("-h", argv[i]) == 0 || Strcmp("-H", argv[i]) == 0)
		{
			printHelp = true;
			break;
		}
		else
		{
			printf("Unknown argument \"%s\" was passed in\n", argv[i]);
		}
	}

	if (printHelp)
	{
		PrintHelp();
	}
	else
	{
		if (inputFile == nullptr)
		{
			printf("File to compile wasn't passed in!\n");
			return -1;
		}

		shaderPath = Path(inputFile);
		String name = shaderPath.GetFileName();
		String nameNoExt = shaderPath.GetFileNameWithoutExtension();
		Path pathWithoutFile = shaderPath.GetDirectoryPath();
		// Getting information out of the file name
		if (!shaderPath.DoesFileExist())
		{
			printf("File named \"%s\" doesn't exist.\n", *name);
			return -1;
		}

		String extension = shaderPath.GetFileExtension();

		String tmp;
		ShaderStage stage;
		if (preprocessorDefinitions.TryFind("FRAG_SHADER", tmp))
		{
			stage = ShaderStage::Fragment;
		}
		else if (preprocessorDefinitions.TryFind("VERT_SHADER", tmp))
		{
			stage = ShaderStage::Vertex;
		}
		else
		{
			stage = DetermineStageBasedOnExtension(extension);
		}

		Path outfile;
		if (outputFile == nullptr)
		{
			outfile = pathWithoutFile / (nameNoExt + "." + DetermineSPVExtensionFrom(stage));
		}
		else
		{
			outfile = outputFile;
		}

		ShaderCompilerDefinitions settings;
		settings.shaderStage = stage;
		settings.definitions = preprocessorDefinitions;

		if (preprocessOnly)
		{
			PreprocessedShaderOutput output;
			bool couldPreprocess = Preprocess(shaderPath.GetString(), settings, output);
			if (!couldPreprocess)
			{
				printf("Error preprocessing! : %s\n", *output.errors);
				return -1;
			}

			SavePreprocessedGLSL(outfile, output);
		}
		else
		{
			ShaderStructure resultingStructure;
			bool compiledSuccessfully = Compile(shaderPath.GetString(), "main", settings, resultingStructure);
			if (!compiledSuccessfully)
			{
				printf("File couldn't be compiled!\n");
				return -1;
			}

			SaveSPVInformation(outfile, resultingStructure);
		}
	}

	return 0;
}