#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

VulkanFramebuffer::VulkanFramebuffer(VulkanDevice * device)
	:logicalDevice(device)
{
}

VulkanFramebuffer::~VulkanFramebuffer()
{
	vkDestroyFramebuffer(logicalDevice->GetNativeHandle(), frameBuffer, nullptr);
}

void VulkanFramebuffer::Initialize(const Array<VkImageView>& attachments_, const VkExtent2D & extent, VulkanRenderPass * renderPass_)
{
	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.attachmentCount = attachments_.Size();
	framebufferInfo.pAttachments = attachments_.GetData();
	framebufferInfo.renderPass = renderPass_->GetNativeHandle();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	CHECK_VK(vkCreateFramebuffer(logicalDevice->GetNativeHandle(), &framebufferInfo, nullptr, &frameBuffer));

	renderPass = renderPass_;
	attachments = attachments_;
	framebufferExtent = extent;

}
