
#include "VulkanDescriptorSetUtilities.hpp"
#include "VulkanDescriptorSet.h"
#include "VulkanBufferAllocation.hpp"

void WriteDescriptorSet::SetupWriteDescriptors(const VulkanDescriptorSetLayout& layout)
{
	DynamicArray<VkDescriptorSetLayoutBinding> layoutBindings = layout.GetLayoutBindings();
	// TODO - have the descriptors be actually sorted and placed in the location of their binding index...
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

	// Need to have the correct size set for these containers before setting up the descriptors below
	bufferDescriptors.Resize(numBuffers);
	imageDescriptors.Resize(numImages);
	writeDescriptors.Resize(numBuffers + numImages);
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
			desc.bufferDescriptor->bindingIndex = layoutBindings[i].binding;
		}
		else if (layoutBindings[i].descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			desc.imageDescriptor = &imageDescriptors[imageIndex++];
			desc.imageDescriptor->descType = layoutBindings[i].descriptorType;
			desc.imageDescriptor->bindingIndex = layoutBindings[i].binding;
		}
		writeDescriptors[i] = desc;
	}
}

void WriteDescriptorSet::SetBuffer(VkDescriptorType type, const VulkanBuffer& buffer, uint32 bindIndex)
{
	for (auto& writeDescriptor : writeDescriptors)
	{
		if (writeDescriptor.bufferDescriptor && writeDescriptor.bufferDescriptor->bindingIndex == bindIndex)
		{
			//Assert(writeDescriptor.bufferDescriptor != nullptr);
			WriteBufferDescriptor* bufferInfo = writeDescriptor.bufferDescriptor;
			Assert(bufferInfo->descType == type);
			bufferInfo->bindingIndex = bindIndex;
			bufferInfo->bufferInfo.buffer = buffer.handle;
			bufferInfo->bufferInfo.offset = buffer.memory->alignedOffset;
			bufferInfo->bufferInfo.range = buffer.memory->size;
			break;
		}
	}
}

void WriteDescriptorSet::SetTexture(VkDescriptorType type, const VulkanTexture& texture, uint32 bindIndex)
{
	for (auto& writeDescriptor : writeDescriptors)
	{
		if (writeDescriptor.imageDescriptor && writeDescriptor.imageDescriptor->bindingIndex == bindIndex)
		{
			//Assert(descriptor.imageDescriptor != nullptr);
			WriteImageDescriptor* imageInfo = writeDescriptor.imageDescriptor;
			Assert(imageInfo->descType == type);
			imageInfo->bindingIndex = bindIndex;
			imageInfo->imageInfo.imageView = texture.imageView;
			imageInfo->imageInfo.sampler = texture.sampler.nativeSampler;
			imageInfo->imageInfo.imageLayout = texture.image->layout;
		}
	}
}