#include "VulkanVertexBuffer.h"
#include "VulkanMemory.h"
#include "VulkanUtilites.h"
#include "VulkanCommandBuffer.h"

VulkanVertexBuffer::VulkanVertexBuffer(const Array<Vertex>& vertices)
{
	uint32 vertexBufferSize = sizeof(Vertex) * vertices.Size();

	// TODO - Get staging buffers working. Need to cache them until they're used
	vertexBuffer = VulkanMemory::AllocateBuffer(
		vertexBufferSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	void* data = VulkanMemory::MapMemory(vertexBuffer.allocatedMemory, vertexBuffer.size);
	Memcpy(data, vertexBufferSize, vertices.GetData(), vertexBufferSize);
	VulkanMemory::UnmapMemory(vertexBuffer.allocatedMemory);

// 	VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(
// 		vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
// 		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
// 	);

// 	void* data = VulkanMemory::MapMemory(stagingBuffer.allocatedMemory, stagingBuffer.size);
// 	Memcpy(data, vertexBufferSize, vertices.GetData(), vertexBufferSize);
// 	VulkanMemory::UnmapMemory(stagingBuffer.allocatedMemory);

// 	vertexBuffer = VulkanMemory::AllocateBuffer(
// 		vertexBufferSize,
// 		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
// 		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
// 	);

	//CopyToDeviceBuffer(stagingBuffer, vertexBuffer);

	//VulkanMemory::DeallocateBuffer(stagingBuffer);
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
	VulkanMemory::DeallocateBuffer(vertexBuffer);
}

void VulkanVertexBuffer::Bind(VulkanCommandBuffer * cmdBuffer) const
{
	assert(cmdBuffer->GetBufferState() == VulkanCommandBuffer::State::InRenderPass);
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdBuffer->GetNativeHandle(), 0, 1, &vertexBuffer.handle, offsets);
}

VulkanWeightsBuffer::VulkanWeightsBuffer(const Array<VertexBoneWeights>& weights)
{
	if (weights.Size() > 0)
	{
		uint32 vertexBufferSize = sizeof(VertexBoneWeights) * weights.Size();

		// TODO - Get staging buffers working. Need to cache them until they're used
		weightsBuffer = VulkanMemory::AllocateBuffer(
			vertexBufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		void* data = VulkanMemory::MapMemory(weightsBuffer.allocatedMemory, weightsBuffer.size);
		Memcpy(data, vertexBufferSize, weights.GetData(), vertexBufferSize);
		VulkanMemory::UnmapMemory(weightsBuffer.allocatedMemory);
	}

	// 	VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(
	// 		vertexBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	// 		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	// 	);

	// 	void* data = VulkanMemory::MapMemory(stagingBuffer.allocatedMemory, stagingBuffer.size);
	// 	Memcpy(data, vertexBufferSize, vertices.GetData(), vertexBufferSize);
	// 	VulkanMemory::UnmapMemory(stagingBuffer.allocatedMemory);

	// 	vertexBuffer = VulkanMemory::AllocateBuffer(
	// 		vertexBufferSize,
	// 		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
	// 		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	// 	);

	//CopyToDeviceBuffer(stagingBuffer, vertexBuffer);

	//VulkanMemory::DeallocateBuffer(stagingBuffer);
}

VulkanWeightsBuffer::~VulkanWeightsBuffer()
{
	VulkanMemory::DeallocateBuffer(weightsBuffer);
}

void VulkanWeightsBuffer::Bind(VulkanCommandBuffer * cmdBuffer) const
{
	assert(cmdBuffer->GetBufferState() == VulkanCommandBuffer::State::InRenderPass);
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(cmdBuffer->GetNativeHandle(), 1, 1, &weightsBuffer.handle, offsets);
}
