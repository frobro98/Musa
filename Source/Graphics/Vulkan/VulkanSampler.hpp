#pragma once

#include "Graphics.h"
#include "Graphics/ResourceInitializationDescriptions.hpp"

class VulkanDevice;

struct VulkanSampler : public NativeSampler
{
	VulkanSampler(const VulkanDevice& device, const SamplerDescription& params);
	~VulkanSampler();

	VkSampler sampler;
private:
	const VulkanDevice& logicalDevice;
};