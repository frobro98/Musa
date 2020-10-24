// Copyright 2020, Nathan Blane

#include "ShaderResource.hpp"
#include "Graphics/GraphicsInterface.hpp"

ShaderResource::ShaderResource(const ShaderHeader& header_, const MemoryBuffer& codeBuffer, ShaderStage::Type stage_)
	: header(header_),
	code(codeBuffer),
	stage(stage_)
{
	if (stage == ShaderStage::Vertex)
	{
		vertexShader = GetGraphicsInterface().CreateVertexShader(codeBuffer);
	}
	else if (stage == ShaderStage::Fragment)
	{
		//fragmentShader = GetGraphicsInterface().CreateFragmentShader(codeBuffer);
	}
	else
	{
		AssertStr(false, "Shader stage passed in currently isn't supported!");
	} 
}
