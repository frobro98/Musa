#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanFence.hpp"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanBufferAllocation.hpp"

#include "Assertion.h"


VulkanCommandBuffer::VulkanCommandBuffer(const VulkanDevice& device, VulkanCommandBufferManager& manager)
	:logicalDevice(&device),
	cmdBufferManager(&manager),
	state(CommandBufferState::Uninitialized)
{
}

void VulkanCommandBuffer::Initialize(VkCommandBufferLevel cmdBufferLevel, VkCommandPool cmdPool)
{
	bufferLevel = cmdBufferLevel;

	VkCommandBufferAllocateInfo cmdAllocInfo = {};
	cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdAllocInfo.commandBufferCount = 1;
	cmdAllocInfo.commandPool = cmdPool;
	cmdAllocInfo.level = cmdBufferLevel;

	CHECK_VK(vkAllocateCommandBuffers(logicalDevice->GetNativeHandle(), &cmdAllocInfo, &commandBuffer));

	state = CommandBufferState::Initialized;

	fence = logicalDevice->GetFenceManager().CreateFence();
}

void VulkanCommandBuffer::Initialize(VkCommandBuffer cmdBuffer, VkCommandBufferLevel cmdBufferLevel)
{
	Assert(cmdBuffer != VK_NULL_HANDLE);

	bufferLevel = cmdBufferLevel;

	commandBuffer = cmdBuffer;
	state = CommandBufferState::Initialized;
	fence = logicalDevice->GetFenceManager().CreateFence();
}

void VulkanCommandBuffer::DeInitialize(VkCommandPool cmdPool)
{
	Assert(commandBuffer != VK_NULL_HANDLE);

	vkFreeCommandBuffers(logicalDevice->GetNativeHandle(), cmdPool, 1, &commandBuffer);
	commandBuffer = VK_NULL_HANDLE;
	state = CommandBufferState::Uninitialized;
}

void VulkanCommandBuffer::Begin(VkCommandBufferUsageFlags cbUsageFlags, const VkCommandBufferInheritanceInfo* pInheritanceInfo)
{
	Assert(state == CommandBufferState::Initialized || state == CommandBufferState::Submitted);
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = cbUsageFlags;
	// TODO - If command buffer usage flags are specific values, parts of the inheritance info must be valid. Therefore, these values must be checked...
	beginInfo.pInheritanceInfo = pInheritanceInfo;
	CHECK_VK(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	state = CommandBufferState::Began;
}

void VulkanCommandBuffer::BeginRenderpass(VulkanFramebuffer* frameBuffer, const DynamicArray<Color32>& clearColors, bool inlinedContents)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	Assert(state == CommandBufferState::Began);

	DynamicArray<VkClearValue> vkClearColors(frameBuffer->GetAttachmentCount());
	uint32 numColorAttachments = frameBuffer->HasDepthAttachment() ? vkClearColors.Size() - 1 : vkClearColors.Size();
	for (uint32 i = 0; i < numColorAttachments; ++i)
	{
		vkClearColors[i].color = { 
			clearColors[i].r,
			clearColors[i].g,
			clearColors[i].b,
			clearColors[i].a
		};
	}

	if (frameBuffer->HasDepthAttachment())
	{
		vkClearColors[vkClearColors.Size() - 1].depthStencil = { 1.f, 0 };
	}

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = frameBuffer->GetRenderPass()->GetNativeHandle();
	renderPassBeginInfo.framebuffer = frameBuffer->GetNativeHandle();
	renderPassBeginInfo.renderArea.offset = { 0,0 };
	renderPassBeginInfo.renderArea.extent.width = frameBuffer->GetWidth();
	renderPassBeginInfo.renderArea.extent.height = frameBuffer->GetHeight();
	renderPassBeginInfo.clearValueCount = vkClearColors.Size();
	renderPassBeginInfo.pClearValues = vkClearColors.GetData();

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, inlinedContents ? VK_SUBPASS_CONTENTS_INLINE : VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

	state = CommandBufferState::InRenderPass;
}

void VulkanCommandBuffer::EndRenderPass()
{
	if (state == CommandBufferState::InRenderPass)
	{
		vkCmdEndRenderPass(commandBuffer);
		state = CommandBufferState::Began;
	}
}

void VulkanCommandBuffer::End()
{
	Assert(state == CommandBufferState::Began);
	CHECK_VK(vkEndCommandBuffer(commandBuffer));
	state = CommandBufferState::PendingSubmit;
}

void VulkanCommandBuffer::ImageMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 imageMemoryBarrierCount, const VkImageMemoryBarrier * pImageMemoryBarriers)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	Assert(state == CommandBufferState::Began ||
		   state == CommandBufferState::InRenderPass);

	vkCmdPipelineBarrier(
		commandBuffer,
		srcStageMask, dstStageMask,
		dependencyFlags, 
		0, nullptr,
		0, nullptr,
		imageMemoryBarrierCount, pImageMemoryBarriers
	);
}

void VulkanCommandBuffer::BufferMemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 bufferMemoryBarrierCount, const VkBufferMemoryBarrier * pBufferMemoryBarriers)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	Assert(state == CommandBufferState::Began ||
		state == CommandBufferState::InRenderPass);

	vkCmdPipelineBarrier(commandBuffer,
		srcStageMask, dstStageMask, 
		dependencyFlags, 
		0, nullptr,
		bufferMemoryBarrierCount, pBufferMemoryBarriers,
		0, nullptr);
}

void VulkanCommandBuffer::MemoryBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32 memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	Assert(state == CommandBufferState::Began ||
		state == CommandBufferState::InRenderPass);

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
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}

	Assert(state == CommandBufferState::Began ||
		state == CommandBufferState::InRenderPass);

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void VulkanCommandBuffer::CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout dstLayout, uint32 regionCount, const VkBufferImageCopy * pRegions)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	// Can't be called during a render pass!
	Assert(state == CommandBufferState::Began);

	vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstLayout, regionCount, pRegions);
}

void VulkanCommandBuffer::CopyImage(VkImage srcImage, VkImageLayout srcLayout, VkImage dstImage, VkImageLayout dstLayout, uint32 regionCount, const VkImageCopy* pRegions)
{
	if (state == CommandBufferState::Submitted ||
		state == CommandBufferState::Initialized)
	{
		Begin();
	}
	// Can't be called during a render pass!
	Assert(state == CommandBufferState::Began);

	vkCmdCopyImage(commandBuffer, srcImage, srcLayout, dstImage, dstLayout, regionCount, pRegions);
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

void VulkanCommandBuffer::BindVertexBuffers(const VulkanVertexBuffer* const* vertexBuffers, const uint32* vertexBufferOffsets, uint32 bufferCount)
{
	if (bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
	{
		Assert(IsInRenderPass());
	}

	DynamicArray<VkBuffer> nativeHandles(bufferCount);
	DynamicArray<VkDeviceSize> offsets(bufferCount);
	for (uint32 i = 0; i < bufferCount; ++i)
	{
		nativeHandles[i] = vertexBuffers[i]->GetBuffer().handle;
		offsets[i] = vertexBufferOffsets[i];
	}

	vkCmdBindVertexBuffers(commandBuffer, 0, bufferCount, nativeHandles.GetData(), offsets.GetData());
}

void VulkanCommandBuffer::BindIndexBuffer(const VulkanIndexBuffer& indexBuffer)
{
	if (bufferLevel == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
	{
		Assert(IsInRenderPass());
	}
	vkCmdBindIndexBuffer(
		commandBuffer,
		indexBuffer.GetBuffer().handle,
		indexBuffer.GetBuffer().memory->alignedOffset,
		VK_INDEX_TYPE_UINT32
	);
}

void VulkanCommandBuffer::DrawIndexed(
	uint32 indexCount, 
	uint32 instanceCount, 
	uint32 firstIndex, 
	int32 vertexOffset,
	uint32 firstInstance
)
{
	vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanCommandBuffer::Draw(
	uint32 vertexCount, 
	uint32 instanceCount, 
	uint32 firstVertex, 
	uint32 firstInstance
)
{
	vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanCommandBuffer::ExecuteCommandBuffers(const DynamicArray<VulkanCommandBuffer*>& cmdBuffers)
{
	secondaryWithinCmdBuffer = cmdBuffers;
	DynamicArray<VkCommandBuffer> nativeBuffers(cmdBuffers.Size());
	for (uint32 i = 0; i < cmdBuffers.Size(); ++i)
	{
		Assert(cmdBuffers[i]->GetLevel() == VK_COMMAND_BUFFER_LEVEL_SECONDARY);
		Assert(cmdBuffers[i]->HasEnded());
		nativeBuffers[i] = cmdBuffers[i]->GetNativeHandle();
	}

	vkCmdExecuteCommands(commandBuffer, nativeBuffers.Size(), nativeBuffers.GetData());
}

void VulkanCommandBuffer::Submit(
	const VulkanQueue& queue, 
	VkSemaphore waitSemaphore,
	VkPipelineStageFlags waitMask,
	VkSemaphore signalSemaphore
)
{
	Assert(state == CommandBufferState::PendingSubmit);

	// TODO - Figure out how to check that this submission can go through. Maybe...
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	if (waitSemaphore != VK_NULL_HANDLE)
	{
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &waitSemaphore;
		submitInfo.pWaitDstStageMask = &waitMask;
	}
	if (signalSemaphore != VK_NULL_HANDLE)
	{
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &signalSemaphore;
	}

	VkResult result = vkQueueSubmit(queue.GetNativeHandle(), 1, &submitInfo, fence->GetNativeHandle());
	CHECK_VK(result);

	state = CommandBufferState::Submitted;
}

bool VulkanCommandBuffer::IsFreeForUse()
{
	FenceState fenceState = fence->CheckFenceStatus();
	bool freeForUse = fenceState == FenceState::Signaled;
	if (freeForUse)
	{
		for (auto& secondaryBuffer : secondaryWithinCmdBuffer)
		{
			secondaryBuffer->Reset(true);
		}
	}

	return freeForUse;
}

void VulkanCommandBuffer::Reset(bool forceReset)
{
	fence->ResetFence(forceReset);
	vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	state = CommandBufferState::Initialized;
	secondaryWithinCmdBuffer.Clear();
}

//////////////////////////////////////////////////////////////////////////

void VulkanCommandBufferManager::Initialize()
{
	Assert(logicalDevice != nullptr);

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

}

void VulkanCommandBufferManager::SubmitGraphicsBuffer(
	bool waitForFence,
	VkSemaphore waitSemaphore,
	VkPipelineStageFlags waitMask,
	VkSemaphore signalSemaphore
)
{
	Assert(activeGraphicsBuffer);
	Assert(!activeTransferBuffer);

	SubmitCommandBuffer(*activeGraphicsBuffer, *logicalDevice->GetGraphicsQueue(), waitForFence, waitSemaphore, waitMask, signalSemaphore);

	activeGraphicsBuffer = nullptr;
}

void VulkanCommandBufferManager::SubmitTransferBuffer(
	bool waitForFence,
	VkSemaphore waitSemaphore,
	VkPipelineStageFlags waitMask,
	VkSemaphore signalSemaphore
)
{
	Assert(activeTransferBuffer);
	SubmitCommandBuffer(*activeTransferBuffer, *logicalDevice->GetTransferQueue(), waitForFence, waitSemaphore, waitMask, signalSemaphore);
	activeTransferBuffer = nullptr;
}

void VulkanCommandBufferManager::SubmitCommandBuffer(
	VulkanCommandBuffer& buffer,
	const VulkanQueue& queue,
	bool waitForFence,
	VkSemaphore waitSemaphore,
	VkPipelineStageFlags waitMask,
	VkSemaphore signalSemaphore
) const
{
	Assert(buffer.ContainsCommands());
	
	if (buffer.HasStarted())
	{
		if (buffer.IsInRenderPass())
		{
			buffer.EndRenderPass();
		}
		buffer.End();
	}

	buffer.Submit(queue, waitSemaphore, waitMask, signalSemaphore);
	if (waitForFence)
	{
		WaitFor(buffer);
	}
}

bool VulkanCommandBufferManager::WaitForGraphicsBuffer()
{
	return false;
}

bool VulkanCommandBufferManager::WaitForTransferBuffer()
{
	return false;
}

bool VulkanCommandBufferManager::HasValidGraphicsBuffer() const
{
	return activeGraphicsBuffer != nullptr;
}

bool VulkanCommandBufferManager::HasValidTransferBuffer() const
{
	return activeTransferBuffer != nullptr;
}

VulkanCommandBuffer* VulkanCommandBufferManager::CreateGraphicsCommandBuffer()
{
	Assert(graphicsCmdPool != VK_NULL_HANDLE);
	VulkanCommandBuffer* cBuffer = new VulkanCommandBuffer(*logicalDevice, *this);
	cBuffer->Initialize(VK_COMMAND_BUFFER_LEVEL_PRIMARY, graphicsCmdPool);
	return cBuffer;
}

VulkanCommandBuffer* VulkanCommandBufferManager::CreateTransferCommandBuffer()
{
	Assert(transferCmdPool != VK_NULL_HANDLE);
	VulkanCommandBuffer* cBuffer = new VulkanCommandBuffer(*logicalDevice, *this);
	cBuffer->Initialize(VK_COMMAND_BUFFER_LEVEL_PRIMARY, transferCmdPool);
	return cBuffer;
}

void VulkanCommandBufferManager::AllocateMissingCommandBuffers(uint32 remainingCmdBufs, DynamicArray<VulkanCommandBuffer*>& outCommandBuffers)
{
	DynamicArray<VkCommandBuffer> buffers(remainingCmdBufs);

	VkCommandBufferAllocateInfo cmdAllocInfo = {};
	cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdAllocInfo.commandBufferCount = remainingCmdBufs;
	cmdAllocInfo.commandPool = graphicsCmdPool;
	cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;

	CHECK_VK(vkAllocateCommandBuffers(logicalDevice->GetNativeHandle(), &cmdAllocInfo, buffers.GetData()));

	for (uint32 i = 0; i < remainingCmdBufs; ++i)
	{
		VulkanCommandBuffer* cmdBuffer = new VulkanCommandBuffer(*logicalDevice, *this);
		cmdBuffer->Initialize(buffers[i], VK_COMMAND_BUFFER_LEVEL_SECONDARY);
		graphicsSecondaryCommandBuffers.Add(cmdBuffer);
		outCommandBuffers.Add(cmdBuffer);
	}
}

VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveGraphicsBuffer()
{
	if (activeTransferBuffer != nullptr)
	{
		SubmitTransferBuffer();
	}

	if (activeGraphicsBuffer == nullptr)
	{
		activeGraphicsBuffer = FindOrCreateGraphicsBuffer();
	}

	Assert(activeGraphicsBuffer);
	return activeGraphicsBuffer;
}

VulkanCommandBuffer* VulkanCommandBufferManager::GetActiveTransferBuffer()
{
	if (activeTransferBuffer == nullptr)
	{
		activeTransferBuffer = FindOrCreateTransferBuffer();
	}

	Assert(activeTransferBuffer);
	return activeTransferBuffer;
}

DynamicArray<VulkanCommandBuffer*> VulkanCommandBufferManager::GetSecondaryCommandBuffers(uint32 numCmdBuffers)
{
	return FindOrCreateSecondaryBuffers(numCmdBuffers);
}

bool VulkanCommandBufferManager::WaitFor(VulkanCommandBuffer& buffer) const
{
	const uint64 nanoSeconds = 10 * 1000000;
	buffer.GetFence()->WaitFor(nanoSeconds);
	return false;
}

VulkanCommandBuffer* VulkanCommandBufferManager::FindOrCreateGraphicsBuffer()
{
	VulkanCommandBuffer* found = nullptr;
	for (auto buffer : graphicsCommandBuffers)
	{
		if (buffer->IsSubmitted() &&
			buffer->IsFreeForUse())
		{
			found = buffer;
			found->Reset();
			break;
		}
	}

	if (!found)
	{
		found = CreateGraphicsCommandBuffer();
		graphicsCommandBuffers.Add(found);
		printf("# graphics command buffers: %d\n", graphicsCommandBuffers.Size());
	}

	return found;
}

VulkanCommandBuffer* VulkanCommandBufferManager::FindOrCreateTransferBuffer()
{
	VulkanCommandBuffer* found = nullptr;
	for (auto buffer : transferCommandBuffers)
	{
		if (buffer->IsFreeForUse())
		{
			found = buffer;
			found->Reset();
			break;
		}
	}

	if (!found)
	{
		found = CreateTransferCommandBuffer();
		transferCommandBuffers.Add(found);
		printf("# transfer command buffers: %d\n", transferCommandBuffers.Size());
	}

	return found;
}

DynamicArray<VulkanCommandBuffer*> VulkanCommandBufferManager::FindOrCreateSecondaryBuffers(uint32 numCmdBuffers)
{
	DynamicArray<VulkanCommandBuffer*> commandBuffers;
	commandBuffers.Reserve(numCmdBuffers);

	for (auto buffer : graphicsSecondaryCommandBuffers)
	{
		// TODO - HACK: Need to figure out a way to treat secondary CBs a little differently...
		//if (buffer->IsFreeForUse())
		{
			buffer->Reset();
			commandBuffers.Add(buffer);
			if (commandBuffers.Size() == numCmdBuffers)
			{
				break;
			}
		}
	}

	if (commandBuffers.Size() < numCmdBuffers)
	{
		uint32 cmdBuffersToAlloc = numCmdBuffers - commandBuffers.Size();
		AllocateMissingCommandBuffers(cmdBuffersToAlloc, commandBuffers);
	}

	return commandBuffers;
	
}

VulkanCommandBufferManager::VulkanCommandBufferManager(const VulkanDevice& device)
	: logicalDevice(&device)
{
}

VulkanCommandBufferManager::~VulkanCommandBufferManager()
{
	vkDestroyCommandPool(logicalDevice->GetNativeHandle(), graphicsCmdPool, nullptr);
	vkDestroyCommandPool(logicalDevice->GetNativeHandle(), transferCmdPool, nullptr);
}
