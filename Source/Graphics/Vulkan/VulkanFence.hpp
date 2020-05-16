// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "BasicTypes/Uncopyable.hpp"

class VulkanDevice;

enum class FenceState : uint32
{
	NotSignaled,
	Signaled
};

class VulkanFenceManager;

class VulkanFence
{
public:
	VulkanFence(const VulkanDevice& device, VulkanFenceManager& manager);
	~VulkanFence();

	inline VkFence GetNativeHandle() const
	{
		return fenceHandle;
	}

	inline VulkanFenceManager& GetFenceManager()
	{
		return *fenceManager;
	}

	inline uint64 GetFenceSignaledCount()
	{
		return fenceSignaledCount;
	}

	FenceState CheckFenceStatus();
	bool WaitFor(uint64 nanoSeconds);
	bool IsSignaled();

	// TODO - HACK: This is horrible practice and must essentially take secondary CBs into account...
	void ResetFence(bool forceReset = false);

private:
	VkFence fenceHandle;
	uint64 fenceSignaledCount = 0;
	FenceState state;
	VulkanFenceManager* fenceManager;
	const VulkanDevice* logicalDevice;
	uint32 pad[1] = { 0 };

};

class VulkanFenceManager : private Uncopyable
{
public:
	VulkanFenceManager(const VulkanDevice& device);
	~VulkanFenceManager();

	VulkanFence* CreateFence();
	void DestroyFence(const VulkanFence& fence);

private:
	DynamicArray<VulkanFence*> fences;
	const VulkanDevice* logicalDevice;
};
