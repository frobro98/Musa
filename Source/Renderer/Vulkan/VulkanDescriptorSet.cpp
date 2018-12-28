#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"

#if GRAPHICS_API_VK

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice * device)
	: logicalDevice(device)
{
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
	assert(descriptorSetLayout != VK_NULL_HANDLE);
	vkDestroyDescriptorSetLayout(logicalDevice->GetNativeHandle(), descriptorSetLayout, nullptr);
}

void VulkanDescriptorSetLayout::AddDescriptorBinding(VkDescriptorType descriptorType, VkShaderStageFlags shaderStages, uint32 binding)
{
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = descriptorType;
	layoutBinding.stageFlags = shaderStages;
	layoutBinding.binding = binding;
	layoutBinding.descriptorCount = 1;

	layoutBindings.Add(layoutBinding);
}

void VulkanDescriptorSetLayout::BindLayout()
{
	assert(descriptorSetLayout == VK_NULL_HANDLE);

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = layoutBindings.Size();
	layoutInfo.pBindings = layoutBindings.GetData();

	CHECK_VK(vkCreateDescriptorSetLayout(logicalDevice->GetNativeHandle(), &layoutInfo, nullptr, &descriptorSetLayout));
}

#endif

VulkanDescriptorSet::VulkanDescriptorSet(VulkanDevice* device, VkDescriptorSet ds, VulkanDescriptorSetLayout* layout)
	: descriptorSet(ds),
	logicalDevice(device),
	associatedLayout(layout)
{
	uint32 uniformDescriptors = 0;
	uint32 samplerDescriptors = 0;
	uint32 storageDescriptors = 0;
	Array<VkDescriptorSetLayoutBinding> layoutBindings = associatedLayout->GetLayoutBindings();
	for(auto& binding : layoutBindings)
	{
		// TODO - some of this information is set when updating. Compare what's set and then get rid of that info from here
		VkWriteDescriptorSet descriptorSetWriter = {};
		descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWriter.descriptorType = binding.descriptorType;
		if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			++uniformDescriptors;
		}
		else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			++storageDescriptors;
		}
		else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			++samplerDescriptors;
		}
		descriptorSetWriter.dstSet = descriptorSet;
		descriptorSetWriter.descriptorCount = binding.descriptorCount;
		descriptorSetWriter.dstBinding = binding.binding;
		descriptorWrites.Add(std::move(descriptorSetWriter));	
	}

	bufferInfos.Reserve(uniformDescriptors + storageDescriptors);
	imageInfos.Reserve(samplerDescriptors);
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
}

void VulkanDescriptorSet::SetUniformBufferInfo(const VulkanBuffer& uniformBuffer, uint32 bufferBinding)
{
	for(auto& writer : descriptorWrites)
	{
		// TODO - Figure out how to test a failed binding
		if (writer.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER &&
			writer.dstBinding == bufferBinding)
		{
			if (writer.pBufferInfo == nullptr)
			{
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = uniformBuffer.handle;
				bufferInfo.offset = 0;
				bufferInfo.range = uniformBuffer.size;
				bufferInfos.Add(std::move(bufferInfo));
				writer.pBufferInfo = &bufferInfos.Last();
				break;
			}
			else
			{
				VkDescriptorBufferInfo* bufferInfo = nullptr;
				for(auto& info : bufferInfos)
				{
					// TODO - This is sort of gross. I would like to figure out a way to not have to do this loop
					if (&info == writer.pBufferInfo)
					{
						bufferInfo = &info;
						break;
					}
				}
				bufferInfo->buffer = uniformBuffer.handle;
				bufferInfo->offset = 0;
				bufferInfo->range = uniformBuffer.size;
				break;
			}
		}
	}
}

// TODO - this needs to be refactored. This is duplicate code when compared to the 
void VulkanDescriptorSet::SetStorageBufferInfo(const VulkanBuffer& storageBuffer, uint32 bufferBinding)
{
	for (auto& writer : descriptorWrites)
	{
		// TODO - Figure out how to test a failed binding
		if (writer.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER &&
			writer.dstBinding == bufferBinding)
		{
			if (writer.pBufferInfo == nullptr)
			{
				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = storageBuffer.handle;
				bufferInfo.offset = 0;
				bufferInfo.range = storageBuffer.size;
				bufferInfos.Add(std::move(bufferInfo));
				writer.pBufferInfo = &bufferInfos.Last();
				break;
			}
			else
			{
				VkDescriptorBufferInfo* bufferInfo = nullptr;
				for (auto& info : bufferInfos)
				{
					// TODO - This is sort of gross. I would like to figure out a way to not have to do this loop
					if (&info == writer.pBufferInfo)
					{
						bufferInfo = &info;
						break;
					}
				}
				bufferInfo->buffer = storageBuffer.handle;
				bufferInfo->offset = 0;
				bufferInfo->range = storageBuffer.size;
				break;
			}
		}
	}
}

void VulkanDescriptorSet::SetSampledTextureInfo(const VulkanTexture& texture, uint32 textureBinding)
{
	for (auto& writer : descriptorWrites)
	{
		// TODO - Figure out how to test a failed binding
		if (writer.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER &&
			writer.dstBinding == textureBinding)
		{
			if (writer.pImageInfo == nullptr)
			{
				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = texture.imageView;
				imageInfo.sampler = texture.sampler;
				imageInfos.Add(std::move(imageInfo));
				writer.pImageInfo = &imageInfos.Last();
			}
			else
			{
				VkDescriptorImageInfo* imageInfo = nullptr;
				for (auto& info : imageInfos)
				{
					// TODO - This is sort of gross. I would like to figure out a way to not have to do this loop
					if (&info == writer.pImageInfo)
					{
						imageInfo = &info;
						break;
					}
				}
				imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo->imageView = texture.imageView;
				imageInfo->sampler = texture.sampler;
			}
		}
	}
}

void VulkanDescriptorSet::UpdateDescriptorSet()
{
	vkUpdateDescriptorSets(logicalDevice->GetNativeHandle(), descriptorWrites.Size(), descriptorWrites.GetData(), 0, nullptr);
}
