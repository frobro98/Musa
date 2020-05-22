// Copyright 2020, Nathan Blane

#pragma once

#include "VulkanDefinitions.h"
#include "Containers/DynamicArray.hpp"
#include "BasicTypes/Color.hpp"

class VulkanDevice;
class VulkanQueue;
class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanFence;
class VulkanVertexBuffer;
class VulkanIndexBuffer;
class VulkanCommandBufferManager;

enum class CommandBufferState
{
	Uninitialized,
	Initialized,
	Began,
	InRenderPass,
	PendingSubmit,
	Submitted
};

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(const VulkanDevice& device, class VulkanCommandBufferManager& manager);

	void Initialize(VkCommandBufferLevel cmdBufferLevel, VkCommandPool cmdPool);
	void Initialize(VkCommandBuffer cmdBuffer, VkCommandBufferLevel cmdBufferLevel);
	void DeInitialize(VkCommandPool cmdPool);

	void Begin(VkCommandBufferUsageFlags cbUsageFlags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, const VkCommandBufferInheritanceInfo* pInheritanceInfo = nullptr);
	void BeginRenderpass(VulkanFramebuffer* frameBuffer, const DynamicArray<Color32>& clearColors, bool inlinedContents = true);
	void EndRenderPass();
	void End();
	void Submit(const VulkanQueue& queue, 
		VkSemaphore waitSemaphore = VK_NULL_HANDLE,
		VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
		VkSemaphore signalSemaphore = VK_NULL_HANDLE);
	bool IsFreeForUse();
	// TODO - HACK: Doesn't accommodate for secondary command buffers. Must rectify this...
	void Reset(bool forceReset = false);

	void ExecuteCommandBuffers(const DynamicArray<VulkanCommandBuffer*>& cmdBuffers);

	// Command wrappers. The point of these is to force the command buffer to state check
	void ImageMemoryBarrier(
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkDependencyFlags dependencyFlags,
		uint32 imageMemoryBarrierCount,
		const VkImageMemoryBarrier* pImageMemoryBarriers
	);

	void BufferMemoryBarrier(
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkDependencyFlags dependencyFlags,
		uint32 bufferMemoryBarrierCount,
		const VkBufferMemoryBarrier* pBufferMemoryBarriers
	);

	void MemoryBarrier(
		VkPipelineStageFlags srcStageMask, 
		VkPipelineStageFlags dstStageMask, 
		VkDependencyFlags dependencyFlags, 
		uint32 memoryBarrierCount, 
		const VkMemoryBarrier* pMemoryBarriers
	);

	void CopyBuffer(
		VkBuffer srcBuffer,
		VkBuffer dstBuffer,
		uint32 regionCount,
		const VkBufferCopy* pRegions
	);
	void CopyBufferToImage(
		VkBuffer srcBuffer,
		VkImage dstImage,
		VkImageLayout dstLayout,
		uint32 regionCount,
		const VkBufferImageCopy* pRegions
	);
	void CopyImage(
		VkImage srcImage, 
		VkImageLayout srcLayout, 
		VkImage dstImage, 
		VkImageLayout dstLayout, 
		uint32 regionCount, 
		const VkImageCopy* pRegions
	);

	void SetViewport(
		uint32 firstViewport,
		uint32 viewportCount,
		const VkViewport* pViewports
	);

	void SetScissor(
		uint32 firstScissor,
		uint32 scissorCount,
		const VkRect2D* pScissors
	);

	void Dispatch(
		uint32 workgroupX, 
		uint32 workgroupY, 
		uint32 workgroupZ
	);

	void BindVertexBuffers(
		const VulkanVertexBuffer* const* vertexBuffers,
		const uint32* vertexBufferOffsets,
		uint32 bufferCount
	);

	void BindVertexBuffers(
		const VkBuffer* vertexBuffers,
		const uint32* vertexBufferOffsets,
		uint32 bufferCount
	);

	void BindIndexBuffer(
		const VulkanIndexBuffer& indexBuffer
	);

	void BindIndexBuffer(
		VkBuffer indexBuffer,
		uint32 bufferOffset
	);

	void DrawIndexed(
		uint32 indexCount,
		uint32 instanceCount,
		uint32 firstIndex,
		int32 vertexOffset,
		uint32 firstInstance
	);

	void Draw(
		uint32 vertexCount,
		uint32 instanceCount,
		uint32 firstVertex,
		uint32 firstInstance
	);

	inline bool ContainsCommands() const { return state != CommandBufferState::Initialized && state != CommandBufferState::Submitted; }
	inline bool HasStarted() const { return state == CommandBufferState::Began || state == CommandBufferState::InRenderPass; }
	inline bool HasEnded() const { return state == CommandBufferState::PendingSubmit; }
	inline bool IsInRenderPass() const { return state == CommandBufferState::InRenderPass; }
	inline bool IsSubmitted() const { return state == CommandBufferState::Submitted; }

	inline VkCommandBuffer GetNativeHandle() const { return commandBuffer; }
	inline VkCommandBufferLevel GetLevel() const { return bufferLevel; }
	inline VulkanFence* GetFence() const { return fence; }
	inline VulkanCommandBufferManager& GetManager() const { return *cmdBufferManager; }
	inline const VulkanDevice& GetDevice() const { return *logicalDevice; }

private:
	DynamicArray<VulkanCommandBuffer*> secondaryWithinCmdBuffer;
	VkCommandBuffer commandBuffer;
	VkCommandBufferLevel bufferLevel;
	const VulkanDevice* logicalDevice;
	VulkanCommandBufferManager* cmdBufferManager;
	VulkanFence* fence = nullptr;
	CommandBufferState state;
};

class VulkanCommandBufferManager
{
public:
	VulkanCommandBufferManager(const VulkanDevice& device);
	~VulkanCommandBufferManager();

	void Initialize();
	void SubmitGraphicsBuffer(
		bool waitForFence = false,
		VkSemaphore waitSemaphore = VK_NULL_HANDLE,
		VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM,
		VkSemaphore signalSemaphore = VK_NULL_HANDLE
	);
	void SubmitTransferBuffer(
		bool waitForFence = false,
		VkSemaphore waitSemaphore = VK_NULL_HANDLE,
		VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM,
		VkSemaphore signalSemaphore = VK_NULL_HANDLE
	);

	bool WaitForGraphicsBuffer();
	bool WaitForTransferBuffer();

	bool HasValidGraphicsBuffer() const;
	bool HasValidTransferBuffer() const;

	VulkanCommandBuffer* GetActiveGraphicsBuffer();
	VulkanCommandBuffer* GetActiveTransferBuffer();

	DynamicArray<VulkanCommandBuffer*> GetSecondaryCommandBuffers(uint32 numCmdBuffers);

private:
	void SubmitCommandBuffer(
		VulkanCommandBuffer& buffer,
		const VulkanQueue& queue,
		bool waitForFence,
		VkSemaphore waitSemaphore,
		VkPipelineStageFlags waitMask,
		VkSemaphore signalSemaphore
	) const;
	bool WaitFor(VulkanCommandBuffer& buffer) const;

	VulkanCommandBuffer* FindOrCreateGraphicsBuffer();
	VulkanCommandBuffer* FindOrCreateTransferBuffer();
	DynamicArray<VulkanCommandBuffer*> FindOrCreateSecondaryBuffers(uint32 numCmdBuffers);
	VulkanCommandBuffer* CreateGraphicsCommandBuffer();
	VulkanCommandBuffer* CreateTransferCommandBuffer();
	void AllocateMissingCommandBuffers(uint32 remainingCmdBufs, DynamicArray<VulkanCommandBuffer*>& outCommandBuffers);

private:
	VkCommandPool graphicsCmdPool = VK_NULL_HANDLE;
	VkCommandPool transferCmdPool = VK_NULL_HANDLE;
	DynamicArray<VulkanCommandBuffer*> graphicsCommandBuffers;
	DynamicArray<VulkanCommandBuffer*> graphicsSecondaryCommandBuffers;
	DynamicArray<VulkanCommandBuffer*> transferCommandBuffers;
	VulkanCommandBuffer* activeGraphicsBuffer = nullptr;
	VulkanCommandBuffer* activeTransferBuffer = nullptr;
	const VulkanDevice& logicalDevice;
};
