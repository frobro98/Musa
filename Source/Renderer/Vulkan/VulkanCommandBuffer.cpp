#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"

#if GRAPHICS_API_VK

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device)
	:logicalDevice(device),
	state(State::Uninitialized)
{
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
	assert(commandBuffer == VK_NULL_HANDLE);
}

void VulkanCommandBuffer::Initialize(CommandBufferLevel cmdBufferLevel, VkCommandPool cmdPool)
{
	VkCommandBufferAllocateInfo cmdAllocInfo = {};
	cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdAllocInfo.commandBufferCount = 1;
	cmdAllocInfo.commandPool = cmdPool;
	cmdAllocInfo.level = static_cast<VkCommandBufferLevel>(cmdBufferLevel);

	CHECK_VK(vkAllocateCommandBuffers(logicalDevice->GetNativeHandle(), &cmdAllocInfo, &commandBuffer));

	state = State::Initialized;
}

void VulkanCommandBuffer::DeInitialize(VkCommandPool cmdPool)
{
	assert(commandBuffer != VK_NULL_HANDLE);
	vkFreeCommandBuffers(logicalDevice->GetNativeHandle(), cmdPool, 1, &commandBuffer);
	commandBuffer = VK_NULL_HANDLE;

	state = State::Uninitialized;
}

void VulkanCommandBuffer::Begin()
{
	assert(state == State::Initialized || state == State::Submitted);
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	CHECK_VK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	state = State::Began;
}

void VulkanCommandBuffer::BeginRenderpass(VulkanFramebuffer* frameBuffer)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}
	assert(state == State::Began);

	VkClearValue clearColors[2] = {};
	clearColors[0].color = { .7f, .7f, .8f, 1.f };
	clearColors[1].depthStencil = { 1.f, 0 };

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = frameBuffer->GetRenderPass()->GetNativeHandle();
	renderPassBeginInfo.framebuffer = frameBuffer->GetNativeHandle();
	renderPassBeginInfo.renderArea.offset = { 0,0 };
	renderPassBeginInfo.renderArea.extent = frameBuffer->GetExtent();
	renderPassBeginInfo.clearValueCount = ArraySize(clearColors);
	renderPassBeginInfo.pClearValues = clearColors;

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	state = State::InRenderPass;
}

void VulkanCommandBuffer::EndRenderPass()
{
	vkCmdEndRenderPass(commandBuffer);
	state = State::Began;
}

void VulkanCommandBuffer::End()
{
	assert(state == State::Began);
	CHECK_VK(vkEndCommandBuffer(commandBuffer));
	state = State::PendingSubmit;
}

void VulkanCommandBuffer::ImageMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 imageMemoryBarrierCount, const VkImageMemoryBarrier * pImageMemoryBarriers)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}
	assert(state == State::Began ||
		   state == State::InRenderPass);

	vkCmdPipelineBarrier(
		commandBuffer,
		srcStageMask, dstStageMask,
		dependencyFlags, 
		0, nullptr,
		0, nullptr,
		imageMemoryBarrierCount, pImageMemoryBarriers
	);

	if (state == State::Began)
	{
		End();
		if (!!(dstStageMask & (VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)))
		{
			Submit(logicalDevice->GetGraphicsQueue());
		}
		else
		{
			Submit(logicalDevice->GetTransferQueue());
		}
		Reset();
	}
}

void VulkanCommandBuffer::BufferMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 bufferMemoryBarrierCount, const VkBufferMemoryBarrier * pBufferMemoryBarriers)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}
	assert(state == State::Began ||
		state == State::InRenderPass);

	vkCmdPipelineBarrier(commandBuffer,
		srcStageMask, dstStageMask, 
		dependencyFlags, 
		0, nullptr,
		bufferMemoryBarrierCount, pBufferMemoryBarriers,
		0, nullptr);
}

void VulkanCommandBuffer::MemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}
	assert(state == State::Began ||
		state == State::InRenderPass);

	vkCmdPipelineBarrier(commandBuffer,
		srcStageMask, dstStageMask,
		dependencyFlags,
		memoryBarrierCount, pMemoryBarriers,
		0, nullptr,
		0, nullptr
	);
}

void VulkanCommandBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, uint32 regionCount, const VkBufferCopy * pRegions)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}

	assert(state == State::Began);

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
	End();
	Submit(logicalDevice->GetTransferQueue());
	Reset();
}

void VulkanCommandBuffer::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstLayout, uint32 regionCount, const VkBufferImageCopy * pRegions)
{
	if (state == State::Submitted ||
		state == State::Initialized)
	{
		Begin();
	}
	assert(state == State::Began);

	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstLayout, regionCount, pRegions);

	End();
	Submit(logicalDevice->GetTransferQueue());
	Reset();
}

void VulkanCommandBuffer::SetViewport(uint32 firstViewport, uint32 viewportCount, const VkViewport * pViewports)
{
	vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void VulkanCommandBuffer::SetScissor(uint32 firstScissor, uint32 scissorCount, const VkRect2D * pScissors)
{
	vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void VulkanCommandBuffer::Dispatch(uint32 workgroupX, uint32 workgroupY, uint32 workgroupZ)
{
	vkCmdDispatch(commandBuffer, workgroupX, workgroupY, workgroupZ);
}

void VulkanCommandBuffer::Submit(VulkanQueue* queue, const VulkanSyncInfo* syncInfo)
{
	assert(state == State::PendingSubmit);

	// TODO - Figure out how to check that this submission can go through. Maybe...
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	VkFence waitFence = VK_NULL_HANDLE;
	if (syncInfo != nullptr)
	{
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &syncInfo->waitSemaphore;
		submitInfo.pWaitDstStageMask = &syncInfo->waitMask;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &syncInfo->signalSemaphore;
		waitFence = syncInfo->waitFence;
	}

	VkResult result = vkQueueSubmit(queue->GetNativeHandle(), 1, &submitInfo, waitFence);
	CHECK_VK(result);

	state = State::Submitted;
	vkQueueWaitIdle(queue->GetNativeHandle());
}

void VulkanCommandBuffer::Reset()
{
	vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	state = State::Initialized;
}

//////////////////////////////////////////////////////////////////////////


VulkanCommandBufferManager* VulkanCommandBufferManager::cbManager = nullptr;

void VulkanCommandBufferManager::InitializeInternal()
{
	assert(logicalDevice != nullptr);

	VkCommandPoolCreateInfo cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = logicalDevice->GetGraphicsQueue()->GetFamilyIndex();
	// TODO - Find out if there are any flags for creating command pools
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	CHECK_VK(vkCreateCommandPool(logicalDevice->GetNativeHandle(), &cmdPoolInfo, nullptr, &graphicsCmdPool));

	cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = logicalDevice->GetTransferQueue()->GetFamilyIndex();
	// TODO - Find out if there are any flags for creating command pools
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	CHECK_VK(vkCreateCommandPool(logicalDevice->GetNativeHandle(), &cmdPoolInfo, nullptr, &transferCmdPool));

	cmdPoolInfo = {};
	cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolInfo.queueFamilyIndex = logicalDevice->GetComputeQueue()->GetFamilyIndex();
	// TODO - Find out if there are any flags for creating command pools
	cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	CHECK_VK(vkCreateCommandPool(logicalDevice->GetNativeHandle(), &cmdPoolInfo, nullptr, &computeCmdPool));
}

void VulkanCommandBufferManager::InitializeTransferCommandBuffer()
{
	assert(transferCmdPool != VK_NULL_HANDLE);
	VulkanCommandBuffer* cBuffer = new VulkanCommandBuffer(logicalDevice);
	cBuffer->Initialize(CommandBufferLevel::Primary, transferCmdPool);
	activeTransferBuffer = cBuffer;
}

void VulkanCommandBufferManager::InitializeComputeCommandBuffer()
{
	assert(graphicsCmdPool != VK_NULL_HANDLE);
	VulkanCommandBuffer* cBuffer = new VulkanCommandBuffer(logicalDevice);
	cBuffer->Initialize(CommandBufferLevel::Primary, computeCmdPool);
	activeComputeBuffer = cBuffer;
}

void VulkanCommandBufferManager::InitializeGraphicsCommandBuffer()
{
	assert(graphicsCmdPool != VK_NULL_HANDLE);
	VulkanCommandBuffer* cBuffer = new VulkanCommandBuffer(logicalDevice);
	cBuffer->Initialize(CommandBufferLevel::Primary, graphicsCmdPool);
	activeGraphicsBuffer = cBuffer;
}

VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveGraphicsBuffer()
{
	if (Instance().activeGraphicsBuffer == nullptr)
	{
		Instance().InitializeGraphicsCommandBuffer();
	}

	return Instance().activeGraphicsBuffer;
}

VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveTransferBuffer()
{
	if (Instance().activeTransferBuffer == nullptr)
	{
		Instance().InitializeTransferCommandBuffer();
	}

	return Instance().activeTransferBuffer;
}

VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveComputeBuffer()
{
	if (Instance().activeComputeBuffer == nullptr)
	{
		Instance().InitializeComputeCommandBuffer();
	}

	return Instance().activeComputeBuffer;
}

VulkanCommandBufferManager& VulkanCommandBufferManager::Instance()
{
	assert(cbManager != nullptr);
	return *cbManager;
}

VulkanCommandBufferManager::VulkanCommandBufferManager(VulkanDevice * device)
	: logicalDevice(device)
{
}

VulkanCommandBufferManager::~VulkanCommandBufferManager()
{
	vkDestroyCommandPool(logicalDevice->GetNativeHandle(), graphicsCmdPool, nullptr);
}

#endif