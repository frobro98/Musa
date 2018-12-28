
#include "File/FileCore/File.h"

#include "VulkanShader.h"
#include "VulkanDevice.h"
#include "String/String.h"
#include "DirectoryLocations.h"

// WALL_WRN_PUSH
// #include <shaderc/shaderc.hpp>
// WALL_WRN_POP

Array<uint8> LoadSPVShader(const tchar* shaderFile)
{
	String filePath(EngineShaderSrcPath());
	filePath += shaderFile;
	File::Handle fHandle;
	File::Result result  = File::Open(fHandle, *filePath, File::Mode::READ);
	// TODO - This probably shouldn't be an assert. This should look in other places and then assert if it can't find files in the other places as well
	assert(result == File::Result::SUCCESS);

	uint32 fileSize;
	File::Seek(fHandle, File::Location::END, 0);
	File::Tell(fHandle, fileSize);
	File::Seek(fHandle, File::Location::BEGIN, 0);

	Array<uint8> fileData(fileSize);
	result = File::Read(fHandle, fileData.GetData(), fileSize);
	assert(result == File::Result::SUCCESS);

	File::Close(fHandle);

// 	std::string source(reinterpret_cast<char*>(fileData.GetData()), fileData.Size());
// 
// 	shaderc::Compiler compiler;
// 	shaderc::CompileOptions options;
// 	options.SetWarningsAsErrors();
// 	shaderc::AssemblyCompilationResult compileResult = compiler.CompileGlslToSpvAssembly(
// 		source, shaderc_glsl_vertex_shader, "test shader", options
// 	);
// 
// 	if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success)
// 	{
// 		printf("%s\n", compileResult.GetErrorMessage().c_str());
// 		return fileData;
// 	}

	return fileData;
}


VulkanShader::VulkanShader(VulkanDevice * device)
	: logicalDevice(device)
{
}

VulkanShader::~VulkanShader()
{
	vkDestroyShaderModule(logicalDevice->GetNativeHandle(), shaderModule, nullptr);
	delete[] shaderSource;
}

void VulkanShader::Compile(const Array<uint8>& source, ShaderStage stage)
{
	shaderStage = stage;
	sourceSize = source.Size();
	uint32 size = source.Size() / sizeof(uint32);
	shaderSource = new uint32[size];
	memcpy(shaderSource, source.GetData(), sourceSize);

	VkShaderModuleCreateInfo shaderModInfo = {};
	shaderModInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModInfo.codeSize = sourceSize;
	shaderModInfo.pCode = shaderSource;

	CHECK_VK(vkCreateShaderModule(logicalDevice->GetNativeHandle(), &shaderModInfo, nullptr, &shaderModule));
}
