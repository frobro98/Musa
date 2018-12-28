#include "VulkanIndexBuffer.h"
#include "VulkanMemory.h"
#include "VulkanUtilites.h"
#include "VulkanCommandBuffer.h"

VulkanIndexBuffer::VulkanIndexBuffer(const Array<Face>& indices)
{
	uint32 indexBufferSize = sizeof(Face) * indices.Size();

	// TODO - Get staging buffers working. Need to cache them until they're used
	indexBuffer = VulkanMemory::AllocateBuffer(
		indexBufferSize,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	void* data = VulkanMemory::MapMemory(indexBuffer.allocatedMemory, indexBuffer.size);
	Memcpy(data, indexBufferSize, indices.GetData(), indexBufferSize);
	VulkanMemory::UnmapMemory(indexBuffer.allocatedMemory);


// 	VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(
// 		indexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
// 		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
// 	);
// 
// 	void* data = VulkanMemory::MapMemory(stagingBuffer.allocatedMemory, stagingBuffer.size);
// 	Memcpy(data, indexBufferSize, indices.GetData(), indexBufferSize);
// 	VulkanMemory::UnmapMemory(stagingBuffer.allocatedMemory);
// 
// 	indexBuffer = VulkanMemory::AllocateBuffer(
// 		indexBufferSize,
// 		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
// 		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
// 	);
// 
// 	CopyToDeviceBuffer(stagingBuffer, indexBuffer);
// 
// 	VulkanMemory::DeallocateBuffer(stagingBuffer);
}

VulkanIndexBuffer::~VulkanIndexBuffer()
{
	VulkanMemory::DeallocateBuffer(indexBuffer);
}

void VulkanIndexBuffer::Bind(VulkanCommandBuffer * cmdBuffer) const
{
	assert(cmdBuffer->GetBufferState() == VulkanCommandBuffer::State::InRenderPass);
	vkCmdBindIndexBuffer(cmdBuffer->GetNativeHandle(), indexBuffer.handle, 0, VK_INDEX_TYPE_UINT32);
}
