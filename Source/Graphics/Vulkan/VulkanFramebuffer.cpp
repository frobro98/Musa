#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "RenderTargetDescription.hpp"
#include "VulkanTexture.h"
#include "Utilities/CoreUtilities.hpp"

VulkanFramebuffer::VulkanFramebuffer(const VulkanDevice& device)
	: logicalDevice(&device)
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

void VulkanFramebuffer::Initialize(const RenderTargetDescription& targetDesc, const NativeRenderTargets& renderTextures, VulkanRenderPass* renderPass_)
{
	Assert(targetDesc.numColorAttachments == renderTextures.numColorTargets);
	nativeTargets = renderTextures;

	uint32 targetCount = targetDesc.hasDepth ? nativeTargets.numColorTargets + 1 : nativeTargets.numColorTargets;
	extents = { (uint32)targetDesc.targetExtents.width, (uint32)targetDesc.targetExtents.height };

	viewAttachments.Resize(targetCount);
	for (uint32 i = 0; i < nativeTargets.numColorTargets; ++i)
	{
		const VulkanTexture* colorTex = static_cast<const VulkanTexture*>(renderTextures.colorTargets[i]);
		viewAttachments[i] = colorTex->image->CreateView();
		// TODO - Initialize extents in a better place. Possibly in the structure that is passed in?? (RenderTargetDescription)
		extents.width = Min(extents.width, colorTex->image->width);
		extents.height = Min(extents.height, colorTex->image->height);
	}

	if (targetDesc.hasDepth)
	{
		const VulkanTexture* depthTex = static_cast<const VulkanTexture*>(renderTextures.depthTarget);
		viewAttachments[renderTextures.numColorTargets] = depthTex->image->CreateView();
		// TODO - Initialize extents in a better place. Possibly in the structure that is passed in?? (RenderTargetDescription)
		extents.width = Min(extents.width, depthTex->image->width);
		extents.height = Min(extents.height, depthTex->image->height);
	}

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.attachmentCount = viewAttachments.Size();
	framebufferInfo.pAttachments = viewAttachments.GetData();
	framebufferInfo.renderPass = renderPass_->GetNativeHandle();
	framebufferInfo.width = extents.width;
	framebufferInfo.height = extents.height;
	framebufferInfo.layers = 1;
	[[maybe_unused]] VkResult result = vkCreateFramebuffer(logicalDevice->GetNativeHandle(), &framebufferInfo, nullptr, &frameBuffer);
	CHECK_VK(result);

	renderPass = renderPass_;
}

bool VulkanFramebuffer::ContainsRT(const VulkanTexture& texture)
{
	for (const auto target : nativeTargets.colorTargets)
	{
		if (target == &texture)
		{
			return true;
		}
	}
	return nativeTargets.depthTarget == &texture;
}

bool VulkanFramebuffer::ContainsRTs(const NativeRenderTargets& renderTextures)
{
	if (nativeTargets.numColorTargets == renderTextures.numColorTargets)
	{
		if (nativeTargets.depthTarget == renderTextures.depthTarget)
		{
			bool result = true;
			uint32 targetCount = nativeTargets.numColorTargets;
			for (uint32 i = 0; i < targetCount; ++i)
			{
				result &= nativeTargets.colorTargets[i] == renderTextures.colorTargets[i];
			}

			return result;
		}
	}

	return false;
}
