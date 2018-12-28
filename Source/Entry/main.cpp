
#include "Containers/Array.h"
#include "Containers/List.h"
#include "Containers/Map.h"

#include "Platform.h"
#include "DebugOutput.h"
#include "Game/Game.h"
#include "String/String.h"
#include "Math/MathEngine.h"
#include "String/CStringUtilities.hpp"
#include "Assertion.h"

#include "File/FileCore/File.h"
#include "DirectoryLocations.h"
WALL_WRN_PUSH
#include "glslang/Public/ShaderLang.h"
#include "spirv/GlslangToSpv.h"
#include "spirv/disassemble.h"
#include "Shader/ThirdParty/spirv_reflect.h"
WALL_WRN_POP

#include <string.h>
#include "Shader/ShaderCompiler.h"
#include "Shader/McppWrapper.hpp"
#include <utility>

#include "File/Path.hpp"
#include "Archiver/FileSerializer.hpp"
#include "Archiver/FileDeserializer.hpp"

static const TBuiltInResource DefaultTBuiltInResource = {
	/* .MaxLights = */ 32,
	/* .MaxClipPlanes = */ 6,
	/* .MaxTextureUnits = */ 32,
	/* .MaxTextureCoords = */ 32,
	/* .MaxVertexAttribs = */ 64,
	/* .MaxVertexUniformComponents = */ 4096,
	/* .MaxVaryingFloats = */ 64,
	/* .MaxVertexTextureImageUnits = */ 32,
	/* .MaxCombinedTextureImageUnits = */ 80,
	/* .MaxTextureImageUnits = */ 32,
	/* .MaxFragmentUniformComponents = */ 4096,
	/* .MaxDrawBuffers = */ 32,
	/* .MaxVertexUniformVectors = */ 128,
	/* .MaxVaryingVectors = */ 8,
	/* .MaxFragmentUniformVectors = */ 16,
	/* .MaxVertexOutputVectors = */ 16,
	/* .MaxFragmentInputVectors = */ 15,
	/* .MinProgramTexelOffset = */ -8,
	/* .MaxProgramTexelOffset = */ 7,
	/* .MaxClipDistances = */ 8,
	/* .MaxComputeWorkGroupCountX = */ 65535,
	/* .MaxComputeWorkGroupCountY = */ 65535,
	/* .MaxComputeWorkGroupCountZ = */ 65535,
	/* .MaxComputeWorkGroupSizeX = */ 1024,
	/* .MaxComputeWorkGroupSizeY = */ 1024,
	/* .MaxComputeWorkGroupSizeZ = */ 64,
	/* .MaxComputeUniformComponents = */ 1024,
	/* .MaxComputeTextureImageUnits = */ 16,
	/* .MaxComputeImageUniforms = */ 8,
	/* .MaxComputeAtomicCounters = */ 8,
	/* .MaxComputeAtomicCounterBuffers = */ 1,
	/* .MaxVaryingComponents = */ 60,
	/* .MaxVertexOutputComponents = */ 64,
	/* .MaxGeometryInputComponents = */ 64,
	/* .MaxGeometryOutputComponents = */ 128,
	/* .MaxFragmentInputComponents = */ 128,
	/* .MaxImageUnits = */ 8,
	/* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
	/* .MaxCombinedShaderOutputResources = */ 8,
	/* .MaxImageSamples = */ 0,
	/* .MaxVertexImageUniforms = */ 0,
	/* .MaxTessControlImageUniforms = */ 0,
	/* .MaxTessEvaluationImageUniforms = */ 0,
	/* .MaxGeometryImageUniforms = */ 0,
	/* .MaxFragmentImageUniforms = */ 8,
	/* .MaxCombinedImageUniforms = */ 8,
	/* .MaxGeometryTextureImageUnits = */ 16,
	/* .MaxGeometryOutputVertices = */ 256,
	/* .MaxGeometryTotalOutputComponents = */ 1024,
	/* .MaxGeometryUniformComponents = */ 1024,
	/* .MaxGeometryVaryingComponents = */ 64,
	/* .MaxTessControlInputComponents = */ 128,
	/* .MaxTessControlOutputComponents = */ 128,
	/* .MaxTessControlTextureImageUnits = */ 16,
	/* .MaxTessControlUniformComponents = */ 1024,
	/* .MaxTessControlTotalOutputComponents = */ 4096,
	/* .MaxTessEvaluationInputComponents = */ 128,
	/* .MaxTessEvaluationOutputComponents = */ 128,
	/* .MaxTessEvaluationTextureImageUnits = */ 16,
	/* .MaxTessEvaluationUniformComponents = */ 1024,
	/* .MaxTessPatchComponents = */ 120,
	/* .MaxPatchVertices = */ 32,
	/* .MaxTessGenLevel = */ 64,
	/* .MaxViewports = */ 16,
	/* .MaxVertexAtomicCounters = */ 0,
	/* .MaxTessControlAtomicCounters = */ 0,
	/* .MaxTessEvaluationAtomicCounters = */ 0,
	/* .MaxGeometryAtomicCounters = */ 0,
	/* .MaxFragmentAtomicCounters = */ 8,
	/* .MaxCombinedAtomicCounters = */ 8,
	/* .MaxAtomicCounterBindings = */ 1,
	/* .MaxVertexAtomicCounterBuffers = */ 0,
	/* .MaxTessControlAtomicCounterBuffers = */ 0,
	/* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
	/* .MaxGeometryAtomicCounterBuffers = */ 0,
	/* .MaxFragmentAtomicCounterBuffers = */ 1,
	/* .MaxCombinedAtomicCounterBuffers = */ 1,
	/* .MaxAtomicCounterBufferSize = */ 16384,
	/* .MaxTransformFeedbackBuffers = */ 4,
	/* .MaxTransformFeedbackInterleavedComponents = */ 64,
	/* .MaxCullDistances = */ 8,
	/* .MaxCombinedClipAndCullDistances = */ 8,
	/* .MaxSamples = */ 4,
	/* .limits = */{
	/* .nonInductiveForLoops = */ 1,
	/* .whileLoops = */ 1,
	/* .doWhileLoops = */ 1,
	/* .generalUniformIndexing = */ 1,
	/* .generalAttributeMatrixVectorIndexing = */ 1,
	/* .generalVaryingIndexing = */ 1,
	/* .generalSamplerIndexing = */ 1,
	/* .generalVariableIndexing = */ 1,
	/* .generalConstantMatrixVectorIndexing = */ 1,
} };


void MCppTest()
{
	ShaderSettings inputs;
	ShaderIntermediate intermediate(inputs);
	ShaderPreprocessor preprocessor(intermediate);
	preprocessor.Preprocess();

	String extensions = "#extension GL_ARB_separate_shader_objects : enable\n#extension GL_ARB_shading_language_420pack : enable";
	String shaderHeader = "#version 450 core\n";
	shaderHeader += extensions;
	


	Assert(preprocessor.Success());

}

void LoadShaderTest()
{
	String shaderPath(EngineShaderSrcPath());
	const char* fileName = "phong.frag";
	shaderPath += fileName;
	File::Handle fHandle;
	assert(File::Open(fHandle, *shaderPath, File::Mode::READ) == File::Result::SUCCESS);
	uint32 fileSize;
	File::Seek(fHandle, File::Location::END, 0);
	File::Tell(fHandle, fileSize);
	File::Seek(fHandle, File::Location::BEGIN, 0);

	Array<uint8> fileData(fileSize+1);
	const char* test = reinterpret_cast<const char*>(fileData.GetData());
	UNUSED(test);
	memset(fileData.GetData(), 0, fileSize+1);
	assert(File::Read(fHandle, fileData.GetData(), fileSize) == File::Result::SUCCESS);
	File::Close(fHandle);

	glslang::InitializeProcess();
	const char* fileText = reinterpret_cast<const char*>(fileData.GetData());
	//int32 len = (int)fileSize;
	const char* filePath = *shaderPath;
	glslang::TShader shader(EShLangVertex);
	shader.setStringsWithLengthsAndNames(&fileText, nullptr, &filePath, 1);
	shader.setEntryPoint("main");
	if (!shader.parse(&DefaultTBuiltInResource, 400, true, EShMessages::EShMsgVulkanRules))
	{
		const char* errors = shader.getInfoLog();
		printf("%s\n", errors);
	}
	glslang::TProgram program;
	program.addShader(&shader);
	if (!program.link(EShMsgVulkanRules))
	{
		const char* errors = program.getInfoLog();
		printf("%s\n", errors);
	}
	program.buildReflection();
	if (program.getIntermediate(EShLangVertex))
	{
		std::vector<uint32> spirv;
		spv::SpvBuildLogger logger;
		glslang::GlslangToSpv(*program.getIntermediate(EShLangVertex), spirv, &logger);

		
		SpvReflectShaderModule module = {};
		SpvReflectResult result = spvReflectCreateShaderModule(spirv.size() * sizeof(uint32), spirv.data(), &module);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		uint32_t count = 0;
		result = spvReflectEnumerateDescriptorSets(&module, &count, NULL);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectDescriptorSet*> sets(count);
		result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (uint32 i = 0; i < sets[0]->binding_count; ++i)
		{
			SpvReflectDescriptorBinding* binding =  sets[0]->bindings[i];
			(void)binding;
			int k = 0;
			k += 1;
		}

		result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectInterfaceVariable*> inputs(count);
		result = spvReflectEnumerateInputVariables(&module, &count, inputs.data());
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (auto& input : inputs)
		{
			(void)input;
			int k = 0;
			k += 1;
		}

		result = spvReflectEnumerateOutputVariables(&module, &count, nullptr);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		spvReflectDestroyShaderModule(&module);
	}

	glslang::FinalizeProcess();
}

void StringTest()
{
	String string = "Hello";
	int32 ind = string.FindFirst("ell");
	assert(ind == 1);

	ind = string.FindLast("llo");
	assert(ind == 2);

	ind = string.FindFirst("/");
	assert(ind == -1);

	ind = string.FindLast("8");
	assert(ind == -1);

	String upper = string.GetUpperCase();
	assert(upper == "HELLO");

	String lower = string.GetLowerCase();
	assert(lower == "hello");

	string.Remove(4);
	Assert(string == "Hell");
	string.Remove(2, 2);
	Assert(string == "He");

	Path path(EngineRootPath());
	path.MakeAbsolute();
}

void DataStructureTest()
{

	int num = 0;
	Map<int, int> map;
	map[0] = 300;
	num = map[300];
	num = map[332];
	num = map[364];
	assert(map.Remove(332));

	Map<String, int> strMap;
	strMap["Hello"] = num;
	assert(strMap["Hello"] == 0);
	strMap["Hello"] = 300;
	assert(strMap["Hello"] == 300);
	String hey = "Hello";
	strMap[hey] = 250;
	assert(strMap["Hello"] == 250);
	strMap["World"] = 300;
	assert(strMap.TryFind("Hello", num));
	assert(num == 250);
	assert(strMap.TryFind("World", num));
	assert(num == 300);

	strMap.Add("test", 500);
	assert(strMap["test"] == 500);

	Map<int, int> testMap = { { 12, 350 },{ 3298, 12 } };
	assert(testMap[12] == 350);
	assert(testMap[3298] == 12);

	for (const auto& pair : testMap)
	{
		Debug::Printf("First: %d Second: %d\n", pair.first, pair.second);
	}

	List<int> l;
	l.Add(5);
	l.Add(4);
	l.Add(3);
	l.Add(2);
	l.Add(1);
	l.Add(0);

	Debug::Print("Numbers in list: ");
	for (auto i : l)
	{
		Debug::Printf("%d, ", i);
	}
	Debug::Print("\n\n");

	List<String> strList;
	strList.Add("Hello");
	strList.Add("World");
	strList.Add("Test");
	strList.Add("Test Again");

	Debug::Print("Strings in list: ");
	for (const auto& s : strList)
	{
		Debug::Printf("%s, ", *s);
	}
	Debug::Print("\n\n");

	Array<String> arr = { "This", "is", "another", "test.", "Woohoo" };

	Debug::Print("Strings in list: ");
	for (const auto& s : arr)
	{
		Debug::Printf("%s, ", *s);
	}
	Debug::Print("\n\n");
}

struct SerialTest
{
	Array<uint32> arr;
	uint32 a;
	uint32 b;
	int32 c;
	uint32 pad1[2];
	uint64 x;
	float y;
	uint32 pad;
	double z;
};

void Serialize(SerializeBase& ser, const SerialTest& test)
{
	Serialize(ser, test.arr);
	Serialize(ser, test.a);
	Serialize(ser, test.b);
	Serialize(ser, test.c);
	Serialize(ser, test.x);
	Serialize(ser, test.y);
	Serialize(ser, test.z);
}

void Deserialize(DeserializeBase& ser, SerialTest& test)
{
	Deserialize(ser, test.arr);
	Deserialize(ser, test.a);
	Deserialize(ser, test.b);
	Deserialize(ser, test.c);
	Deserialize(ser, test.x);
	Deserialize(ser, test.y);
	Deserialize(ser, test.z);
}

void FileSerializationTest()
{

	Path filePath(EngineRootPath());
	filePath /= "SerializationTest.bin";

	SerialTest test;
	test.arr = { 0x11111111, 0x22222222, 0x33333333 };
	test.a = 0xAAAAAAAA;
	test.b = 0xBBBBBBBB;
	test.c = 0x7FFFFFFF;
	test.x = 0x0;
	test.y = 1.23456f;
	test.z = 7.890;

	{
		FileSerializer serializer(filePath);
		Serialize(serializer, test);
	}

	SerialTest readTest;
	{
		FileDeserializer deserialize(filePath);
		Deserialize(deserialize, readTest);
	}

	Assert(readTest.arr.Contains(0x11111111));
	Assert(readTest.arr.Contains(0x22222222));
	Assert(readTest.arr.Contains(0x33333333));
	Assert(readTest.a == 0xAAAAAAAA);
	Assert(readTest.b == 0xBBBBBBBB);
	Assert(readTest.c == 0x7FFFFFFF);
	Assert(readTest.x == 0x0);
	Assert(readTest.y == 1.23456f);
	Assert(readTest.z == 7.890);
}

int CALLBACK WinMain(HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int /*nCmdShow*/)

{
	FILE* stdOut = nullptr;
	{
		AllocConsole();
		freopen_s(&stdOut, "CONOUT$", "w", stdout);
	}

	//LoadShaderTest();
	//MCppTest();
	//StringTest();
	//DataStructureTest();
	//FileSerializationTest();

	Game game;
	game.Run();

	return 0;
}
