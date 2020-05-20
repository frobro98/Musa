#pragma once

#include "Containers/DynamicArray.hpp"
#include "Graphics/Vulkan/VulkanAbstractions.h"
#include "String/String.h"

struct SpirvBuffer
{
	VkDescriptorType bufferType;
	uint32 bindIndex;
};

struct SpirvSampler
{
	VkDescriptorType samplerType;
	uint32 bindIndex;
};

struct VulkanShaderHeader
{
	DynamicArray<SpirvBuffer> buffers;
	DynamicArray<SpirvSampler> samplers;
	String entryPoint;
	VkShaderStageFlags shaderStage;
	uint32 bytecodeHash;
};

// Serializing header of a shader
forceinline void Serialize(SerializeBase& ser, const SpirvBuffer& buf)
{
	uint32 type = (uint32)buf.bufferType;
	Serialize(ser, type);
	Serialize(ser, buf.bindIndex);
}

forceinline void Serialize(SerializeBase& ser, const SpirvSampler& sampler)
{
	uint32 type = (uint32)sampler.samplerType;
	Serialize(ser, type);
	Serialize(ser, sampler.bindIndex);
}

forceinline void Serialize(SerializeBase& ser, const VulkanShaderHeader& shaderHeader)
{
	Serialize(ser, (uint32)shaderHeader.shaderStage);
	Serialize(ser, shaderHeader.bytecodeHash);
	Serialize(ser, shaderHeader.buffers);
	Serialize(ser, shaderHeader.samplers);
	Serialize(ser, shaderHeader.entryPoint);
}

// Deserializing header of a shader
forceinline void Deserialize(DeserializeBase& ser, SpirvBuffer& buf)
{
	uint32 type;
	Deserialize(ser, type);
	buf.bufferType = (VkDescriptorType)type;
	Deserialize(ser, buf.bindIndex);
}

forceinline void Deserialize(DeserializeBase& ser, SpirvSampler& sampler)
{
	uint32 type;
	Deserialize(ser, type);
	sampler.samplerType = (VkDescriptorType)type;
	Deserialize(ser, sampler.bindIndex);
}

forceinline void Deserialize(DeserializeBase& ser, VulkanShaderHeader& shaderHeader)
{
	uint32 stage;
	Deserialize(ser, stage);
	shaderHeader.shaderStage = (VkShaderStageFlags)stage;
	Deserialize(ser, shaderHeader.bytecodeHash);
	Deserialize(ser, shaderHeader.buffers);
	Deserialize(ser, shaderHeader.samplers);
	Deserialize(ser, shaderHeader.entryPoint);
}

