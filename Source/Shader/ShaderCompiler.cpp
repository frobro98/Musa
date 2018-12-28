#include "ShaderCompiler.h"
#include "ShaderStructure.hpp"
#include "ShaderExternal.h"
#include "McppWrapper.hpp"

#include "DirectoryLocations.h"
#include "Assertion.h"

namespace
{
// TODO - Make this in accordance with the Vulkan Device properties
static const TBuiltInResource DefaultTBuiltInResource =
{
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

static EShLanguage GetGlslangStage(ShaderStage stage)
{
	switch (stage)
	{
		case Stage_Vert:
			return EShLangVertex;
		case Stage_Frag:
			return EShLangFragment;
		case Stage_Geom:
			return EShLangGeometry;
		case Stage_TessEval:
			return EShLangTessEvaluation;
		case Stage_TessControl:
			return EShLangTessControl;
		case Stage_Comp:
			return EShLangCompute;
		case Stage_Max:
		default:
		{
			assert(false);
			return static_cast<EShLanguage>(-1);
		}
	}
}

#pragma warning(push)
#pragma warning(disable:4062)
static ShaderIntrinsics GetShaderIntrinsic(SpvOp operation)
{
	switch (operation)
	{
		case SpvOpTypeBool:
			return ShaderIntrinsics::Boolean;
		case SpvOpTypeInt:
			return ShaderIntrinsics::Integer;
		case SpvOpTypeFloat:
			return ShaderIntrinsics::Float;
		case SpvOpTypeVector:
			return ShaderIntrinsics::Vector;
		case SpvOpTypeMatrix:
			return ShaderIntrinsics::Matrix;
	}

	assert(false);
	return static_cast<ShaderIntrinsics>(-1);
}

static ShaderConstantType GetShaderConstantType(SpvReflectDescriptorType type)
{
	switch (type)
	{
		case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			return ShaderConstantType::TextureSampler;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			return ShaderConstantType::UniformBuffer;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
			return ShaderConstantType::StorageBuffer;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
			return ShaderConstantType::UniformDynamicBuffer;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
			return ShaderConstantType::StorageDynamicBuffer;
	}

	return static_cast<ShaderConstantType>(-1);
}
#pragma warning(pop)

}


static bool Preprocess(const ShaderSettings& inputs, String& preprocessedShader)
{
	ShaderIntermediate intermediate(inputs);
	ShaderPreprocessor preprocessor(intermediate);
	preprocessor.Preprocess();

	String extensions = "#extension GL_ARB_separate_shader_objects : enable\n#extension GL_ARB_shading_language_420pack : enable";
	String shaderHeader = "#version 450 core\n";
	shaderHeader += extensions;

	preprocessedShader = shaderHeader + preprocessor.PreprocessedOutput();

	// TODO - Figure out errors in preprocessing....
	printf("%s\n", preprocessor.ErrorString());
	return preprocessor.Success();

}

bool Compile(const ShaderSettings& inputs, ShaderStructure& output)
{
	String preprocessedShader;
	if (!Preprocess(inputs, preprocessedShader))
	{
		// Log
		Assert(false);
	}

	bool result = false;
	EShLanguage stage = GetGlslangStage(inputs.shaderStage);
	const tchar* shaderCode = *inputs.shaderCode;
	glslang::TShader shader(stage);
	shader.setStringsWithLengths(&shaderCode, nullptr, 1);
	shader.setEntryPoint("main");
	TBuiltInResource resources = DefaultTBuiltInResource;
	EShMessages messages = static_cast<EShMessages>(EShMsgVulkanRules | EShMsgSpvRules | EShMsgDefault);
	if (!shader.parse(&resources, 110, true, messages))
	{
		const char* errors = shader.getInfoLog();
		printf("%s\n", errors);
		// Log the errors....
		Assert(false);
	}
	else
	{
		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(messages))
		{
			const char* errors = program.getInfoLog();
			printf("%s\n", errors);
		}
		else
		{
			Assert(program.getIntermediate(stage));
			program.buildReflection();
			std::vector<uint32> spirv;
			spv::SpvBuildLogger logger;
			glslang::GlslangToSpv(*program.getIntermediate(stage), spirv, &logger);
			// Log the messages from logger...

			// Reflection
			SpvReflectShaderModule module = {};
			SpvReflectResult spvResult = spvReflectCreateShaderModule(
				spirv.size() * sizeof(uint32), spirv.data(), &module);
			Assert(spvResult == SPV_REFLECT_RESULT_SUCCESS);

			// Inputs
			{
				uint32 count;
				result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				Array<SpvReflectInterfaceVariable*> shaderInputs(count);
				result = spvReflectEnumerateInputVariables(&module, &count, shaderInputs.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				for (const auto& input : shaderInputs)
				{
					ShaderVariable var = {};
					var.name = input->name;
					var.variableType = GetShaderIntrinsic(input->type_description->op);
					var.location = input->location;
					output.locationToInputs.Add(var.location, var);
				}
			}

			// Outputs
			{
				uint32 count;
				result = spvReflectEnumerateOutputVariables(&module, &count, nullptr);
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				Array<SpvReflectInterfaceVariable*> shaderOutputs(count);
				result = spvReflectEnumerateOutputVariables(&module, &count, shaderOutputs.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				for (const auto& out : shaderOutputs)
				{
					ShaderVariable var = {};
					var.name = out->name;
					var.variableType = GetShaderIntrinsic(out->type_description->op);
					var.location = out->location;
					output.locationToOutputs.Add(var.location, var);
				}
			}


			// Constants
			{
				uint32 count;
				result = spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				Array<SpvReflectDescriptorBinding*> shaderDescriptors(count);
				result = spvReflectEnumerateDescriptorBindings(&module, &count, shaderDescriptors.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				for (const auto& binding : shaderDescriptors)
				{
					ShaderConstant constant = {};
					constant.name = binding->name;
					constant.bindingType = GetShaderConstantType(binding->descriptor_type);
					constant.binding = binding->binding;
					output.bindingToConstants.Add(constant.binding, constant);
				}
			}

			spvReflectDestroyShaderModule(&module);

			// Shader code output
			output.compiledCode.Resize(spirv.size() * sizeof(uint32));
			Memcpy(output.compiledCode.GetData(), output.compiledCode.Size(), spirv.data(), spirv.size() * sizeof(uint32));
		}
	}

	return result;
}
