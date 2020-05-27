// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanAbstractions.h"
#include "BasicTypes/Uncopyable.hpp"
#include "Containers/DynamicArray.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

struct Face;
class VulkanDevice;

class VulkanIndexBuffer : public NativeIndexBuffer, private Uncopyable
{
public:
	VulkanIndexBuffer(VulkanDevice& device, const DynamicArray<Face>& indices);
	~VulkanIndexBuffer();

	inline VulkanBuffer& GetBuffer() const { return *indexBuffer; }
	inline u32 GetNumberOfIndicies() const { return numIndicies; }

private:
	VulkanDevice& logicalDevice;
	VulkanBuffer* indexBuffer;
	u32 numIndicies;
};
