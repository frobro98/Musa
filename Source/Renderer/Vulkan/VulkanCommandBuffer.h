#pragma once

#include "Graphics.h"

#if GRAPHICS_API_VK

class VulkanDevice;
class VulkanQueue;
class VulkanFramebuffer;
class VulkanRenderPass;

enum class CommandBufferLevel
{
	Primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
};

struct VulkanSyncInfo
{
	VkSemaphore waitSemaphore = VK_NULL_HANDLE;
	VkSemaphore signalSemaphore = VK_NULL_HANDLE;
	VkFence waitFence = VK_NULL_HANDLE;
	VkPipelineStageFlags waitMask = VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
	uint32 pad[1] = { 0 };
};

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(VulkanDevice* device);
	~VulkanCommandBuffer();

	void Initialize(CommandBufferLevel cmdBufferLevel, VkCommandPool cmdPool);
	void DeInitialize(VkCommandPool cmdPool);

	void Begin();
	void BeginRenderpass(VulkanFramebuffer* frameBuffer);
	void EndRenderPass();
	void End();
	void Submit(VulkanQueue* queue, const VulkanSyncInfo* syncInfo = nullptr);
	void Reset();

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

	void DrawIndexed(
		uint32 indexCount,
		uint32 instanceCount,
		uint32 firstIndex,
		int32 vertexOffset,
		uint32 firstInstance
	);

	inline VkCommandBuffer GetNativeHandle() const { return commandBuffer; }

public:
	enum class State
	{
		Uninitialized,
		Initialized,
		Began,
		InRenderPass,
		PendingSubmit,
		Submitted
	};

public:
	State GetBufferState() const { return state; }

private:
	VkCommandBuffer commandBuffer;
	VulkanDevice* logicalDevice;
	State state;
};

class VulkanCommandBufferManager
{
	friend class Renderer;
public:
	static VulkanCommandBuffer* GetActiveGraphicsBuffer();
	static VulkanCommandBuffer* GetActiveTransferBuffer();
	static VulkanCommandBuffer* GetActiveComputeBuffer();

private:
	friend class Renderer;

	void InitializeInternal();

	void InitializeGraphicsCommandBuffer();
	void InitializeTransferCommandBuffer();
	void InitializeComputeCommandBuffer();

	static VulkanCommandBufferManager& Instance();
	static VulkanCommandBufferManager* cbManager;

	VulkanCommandBufferManager(VulkanDevice* device);
	~VulkanCommandBufferManager();

private:
	VkCommandPool graphicsCmdPool = VK_NULL_HANDLE;
	VkCommandPool transferCmdPool = VK_NULL_HANDLE;
	VkCommandPool computeCmdPool = VK_NULL_HANDLE;
	VulkanCommandBuffer* activeGraphicsBuffer = nullptr;
	VulkanCommandBuffer* activeTransferBuffer = nullptr;
	VulkanCommandBuffer* activeComputeBuffer = nullptr;
	VulkanDevice* logicalDevice = nullptr;
	uint32 pad[2] = { 0, 0 };
};
#endif