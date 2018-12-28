#pragma once

#include "Graphics.h"
#include "VulkanAbstractions.h"

class VulkanStorageBuffer
{
public:
	VulkanStorageBuffer() = default;
	~VulkanStorageBuffer();

	VulkanStorageBuffer(const VulkanStorageBuffer&) = delete;
	VulkanStorageBuffer& operator=(const VulkanStorageBuffer&) = delete;

	void Initialize(uint32 bufferSize);
	void UpdateStorage(const void* bufferData);

	inline VulkanBuffer& GetBuffer() { return storageBuffer; }

	void* GetMappedData();

private:
	VulkanBuffer storageBuffer;
	void* mappedData = nullptr;
	uint32 pad[1] = { 0 };
};

using StorageBuffer = VulkanStorageBuffer;