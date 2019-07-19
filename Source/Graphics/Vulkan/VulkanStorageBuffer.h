#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"

class VulkanStorageBuffer
{
public:
	VulkanStorageBuffer() = default;
	//~VulkanStorageBuffer();

	VulkanStorageBuffer(const VulkanStorageBuffer&) = delete;
	VulkanStorageBuffer& operator=(const VulkanStorageBuffer&) = delete;

	void Initialize(const VulkanDevice& device, uint32 bufferSize);
	void UpdateStorage(const void* bufferData);

	inline VulkanBuffer& GetBuffer() { return *storageBuffer; }

private:
	VulkanBuffer* storageBuffer;
};

using StorageBuffer = VulkanStorageBuffer;