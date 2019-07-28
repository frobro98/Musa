
#include "VulkanDescriptorSetUtilities.hpp"
#include "VulkanDescriptorSet.h"
#include "VulkanBufferAllocation.hpp"

void WriteDescriptorSet::SetupWriteDescriptors(const VulkanDescriptorSetLayout& layout)
{
	DynamicArray<VkDescriptorSetLayoutBinding> layoutBindings = layout.GetLayoutBindings();
	for (auto& binding : layoutBindings)
	{
		if (binding.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
			binding.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			++numBuffers;
		}
		else if (binding.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			++numImages;
		}
	}

	bufferDescriptors.Reserve(numBuffers);
	imageDescriptors.Reserve(numImages);
	writeDescriptors.Resize(numImages + numBuffers);
	Assert(layoutBindings.Size() == writeDescriptors.Size());

	uint32 imageIndex = 0;
	uint32 bufferIndex = 0;
	for (uint32 i = 0; i < writeDescriptors.Size(); ++i)
	{
		WriteDescriptor desc = {};
		if (layoutBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
			layoutBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			desc.bufferDescriptor = &bufferDescriptors[bufferIndex++];
			desc.bufferDescriptor->descType = layoutBindings[i].descriptorType;
		}
		else if (layoutBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			desc.imageDescriptor = &imageDescriptors[imageIndex++];
			desc.imageDescriptor->descType = layoutBindings[i].descriptorType;
		}
		writeDescriptors[i] = desc;
	}
}

void WriteDescriptorSet::SetBuffer(VkDescriptorType type, const VulkanBuffer& buffer, uint32 bindIndex)
{
	Assert(bindIndex < writeDescriptors.Size());
	WriteDescriptor& descriptor = writeDescriptors[bindIndex];
	Assert(descriptor.bufferDescriptor != nullptr);
	WriteBufferDescriptor* bufferInfo = descriptor.bufferDescriptor;
	Assert(bufferInfo->descType == type);
	bufferInfo->bindingIndex = bindIndex;
	bufferInfo->bufferInfo.buffer = buffer.handle;
	bufferInfo->bufferInfo.offset = buffer.memory->alignedOffset;
	bufferInfo->bufferInfo.range = buffer.memory->size;
}

void WriteDescriptorSet::SetTexture(VkDescriptorType type, const VulkanTexture& texture, uint32 bindIndex)
{
	Assert(bindIndex < writeDescriptors.Size());
	WriteDescriptor& descriptor = writeDescriptors[bindIndex];
	Assert(descriptor.imageDescriptor != nullptr);
	WriteImageDescriptor* imageInfo = descriptor.imageDescriptor;
	Assert(imageInfo->descType == type);
	imageInfo->bindingIndex = bindIndex;
	imageInfo->imageInfo.imageView = texture.imageView;
	imageInfo->imageInfo.sampler = texture.sampler.nativeSampler;
	imageInfo->imageInfo.imageLayout = texture.image->layout;
}