#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "RenderTargetDescription.hpp"

VulkanFramebuffer::VulkanFramebuffer(const VulkanDevice& device)
	:logicalDevice(&device)
{
}

VulkanFramebuffer::~VulkanFramebuffer()
{
	for (auto view : viewAttachments)
	{
		vkDestroyImageView(logicalDevice->GetNativeHandle(), view, nullptr);
	}
	vkDestroyFramebuffer(logicalDevice->GetNativeHandle(), frameBuffer, nullptr);
}

void VulkanFramebuffer::Initialize(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures, VulkanRenderPass* renderPass_)
{
	Assert(targetDesc.targetCount == renderTextures.targetCount);
	nativeTargets = renderTextures;

	viewAttachments.Resize(nativeTargets.targetCount + 1);
	for (uint32 i = 0; i < nativeTargets.targetCount; ++i)
	{
		viewAttachments[i] = renderTextures.colorTargets[i]->image->CreateView();
	}

	viewAttachments[renderTextures.targetCount] = renderTextures.depthTarget->image->CreateView();

	extents = { (uint32)targetDesc.targetExtents.width, (uint32)targetDesc.targetExtents.height };
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.attachmentCount = viewAttachments.Size();
	framebufferInfo.pAttachments = viewAttachments.GetData();
	framebufferInfo.renderPass = renderPass_->GetNativeHandle();
	framebufferInfo.width = extents.width;
	framebufferInfo.height = extents.height;
	framebufferInfo.layers = 1;

	CHECK_VK(vkCreateFramebuffer(logicalDevice->GetNativeHandle(), &framebufferInfo, nullptr, &frameBuffer));

	renderPass = renderPass_;
}

bool VulkanFramebuffer::ContainsRTs(const RenderTargetTextures& renderTextures)
{
	if (nativeTargets.targetCount == renderTextures.targetCount)
	{
		if (nativeTargets.depthTarget == renderTextures.depthTarget)
		{
			bool result = true;
			uint32 targetCount = nativeTargets.targetCount;
			for (uint32 i = 0; i < targetCount; ++i)
			{
				result &= nativeTargets.colorTargets[i] == renderTextures.colorTargets[i];
			}

			return result;
		}
	}

	return false;
}
