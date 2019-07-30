#include "VulkanIndexBuffer.h"
#include "VulkanUtilities.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanMemoryManager.hpp"
#include "VulkanBufferAllocation.hpp"

// TODO - The magic number "3" must be defined somewhere...
VulkanIndexBuffer::VulkanIndexBuffer(VulkanDevice& device, const DynamicArray<Face>& indices)
	: logicalDevice(device),
	numIndicies(indices.Size() * 3)
{
	uint32 indexBufferSize = sizeof(Face) * indices.Size();

	// TODO - Get staging buffers working. Need to cache them until they're used
	indexBuffer = logicalDevice.GetMemoryManager().AllocateBuffer(
		indexBufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	{
		indexBuffer->memory->LockForWrite();
		Memcpy(indexBuffer->memory->GetMappedPtr(), indexBufferSize, indices.GetData(), indexBufferSize);
		indexBuffer->memory->Unlock();
	}
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
	logicalDevice.GetMemoryManager().DeallocateBuffer(*indexBuffer);
}
