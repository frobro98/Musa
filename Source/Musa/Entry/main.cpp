
/*
#include "Containers/DynamicArray.hpp"
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

//*/

/*
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
	Assert(File::Open(fHandle, *shaderPath, File::Mode::READ) == File::Result::SUCCESS);
	uint32 fileSize;
	File::Seek(fHandle, File::Location::END, 0);
	File::Tell(fHandle, fileSize);
	File::Seek(fHandle, File::Location::BEGIN, 0);

	DynamicArray<uint8> fileData(fileSize+1);
	const char* test = reinterpret_cast<const char*>(fileData.GetData());
	UNUSED(test);
	memset(fileData.GetData(), 0, fileSize+1);
	Assert(File::Read(fHandle, fileData.GetData(), fileSize) == File::Result::SUCCESS);
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
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		uint32_t count = 0;
		result = spvReflectEnumerateDescriptorSets(&module, &count, NULL);
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectDescriptorSet*> sets(count);
		result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (uint32 i = 0; i < sets[0]->binding_count; ++i)
		{
			SpvReflectDescriptorBinding* binding =  sets[0]->bindings[i];
			(void)binding;
			int k = 0;
			k += 1;
		}

		result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		std::vector<SpvReflectInterfaceVariable*> inputs(count);
		result = spvReflectEnumerateInputVariables(&module, &count, inputs.data());
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		for (auto& input : inputs)
		{
			(void)input;
			int k = 0;
			k += 1;
		}

		result = spvReflectEnumerateOutputVariables(&module, &count, nullptr);
		Assert(result == SPV_REFLECT_RESULT_SUCCESS);

		spvReflectDestroyShaderModule(&module);
	}

	glslang::FinalizeProcess();
}

void StringTest()
{
	String string = "Hello";
	int32 ind = string.FindFirst("ell");
	Assert(ind == 1);

	ind = string.FindLast("llo");
	Assert(ind == 2);

	ind = string.FindFirst("/");
	Assert(ind == -1);

	ind = string.FindLast("8");
	Assert(ind == -1);

	String upper = string.GetUpperCase();
	Assert(upper == "HELLO");

	String lower = string.GetLowerCase();
	Assert(lower == "hello");

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
	Assert(map.Remove(332));

	Map<String, int> strMap;
	strMap["Hello"] = num;
	Assert(strMap["Hello"] == 0);
	strMap["Hello"] = 300;
	Assert(strMap["Hello"] == 300);
	String hey = "Hello";
	strMap[hey] = 250;
	Assert(strMap["Hello"] == 250);
	strMap["World"] = 300;
	Assert(strMap.TryFind("Hello", num));
	Assert(num == 250);
	Assert(strMap.TryFind("World", num));
	Assert(num == 300);

	strMap.Add("test", 500);
	Assert(strMap["test"] == 500);

	Map<int, int> testMap = { { 12, 350 },{ 3298, 12 } };
	Assert(testMap[12] == 350);
	Assert(testMap[3298] == 12);

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

	DynamicArray<String> arr = { "This", "is", "another", "test.", "Woohoo" };

	Debug::Print("Strings in list: ");
	for (const auto& s : arr)
	{
		Debug::Printf("%s, ", *s);
	}
	Debug::Print("\n\n");
}

struct SerialTest
{
	DynamicArray<uint32> arr;
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

//*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdlib>

#include "GameEngine.hpp"

int CALLBACK WinMain(HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int /*nCmdShow*/)

{
	//*
	FILE* stdOut = nullptr;
	{
		AllocConsole();
		freopen_s(&stdOut, "CONOUT$", "w", stdout);
	}
	//*/

	//LoadShaderTest();
	//MCppTest();
	//StringTest();
	//DataStructureTest();
	//FileSerializationTest();

	GameEngine engine;
	engine.RunEngine();

	// TODO - Fix crash when the engine exits. It is likely due to the graphics not being cleaned up correctly.

	return 0;
}
