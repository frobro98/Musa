
#include "Platform.h"
#include "ResourceBlob.hpp"
#include "VulkanGraphicsInterface.hpp"
#include "VulkanCreateInfos.h"
#include "VulkanDevice.h"
#include "VulkanShaderManager.h"
#include "VulkanSurface.h"
#include "VulkanSwapchain.h"
#include "VulkanStagingBufferManager.hpp"
#include "VulkanUtilities.h"
#include "VulkanMemoryManager.hpp"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderingCloset.hpp"
#include "VulkanViewport.hpp"
#include "VulkanUtilities.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"

constexpr const tchar* validationLayers[] = {
	"VK_LAYER_LUNARG_standard_validation"
};

constexpr const tchar* instanceExtensions[] = {
	VK_KHR_SURFACE_EXTENSION_NAME,
	VK_PLATFORM_SURFACE_EXTENSION,
	VK_EXT_DEBUG_REPORT_EXTENSION_NAME
};

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t srcObj, size_t location, int32_t msgCode,
	const char* layerPrefix, const char* msg,
	void* userData)
{
	UNUSED(flags, objType, srcObj, location, msgCode, layerPrefix, msg, userData);
#ifdef _DEBUG

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		printf("WARNING: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		printf("PERFORMANCE: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		printf("ERROR: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		printf("DEBUG: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		printf("INFO: ");
		printf("@[%s]: %s\n", layerPrefix, msg);
	}

#endif // _DEBUG

	return false;
}

static void UploadTextureBlob(VulkanDevice& logicalDevice, const ResourceBlob& blob, VulkanImage& image)
{
	VulkanCommandBuffer* cmdBuffer = logicalDevice.GetCmdBufferManager().GetActiveGraphicsBuffer();
	VulkanStagingBuffer* stagingBuffer = logicalDevice.GetStagingBufferManager().AllocateStagingBuffer(blob.GetSize());
	Memcpy(stagingBuffer->mappedData, (size_t)stagingBuffer->stagingSize, blob.GetData(), blob.GetSize());

	VkImageSubresourceRange subresourceRange = {};
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresourceRange.baseMipLevel = 0;
	subresourceRange.levelCount = image.mipmapLevels;
	subresourceRange.baseArrayLayer = 0;
	subresourceRange.layerCount = 1;

	ImageLayoutTransition(*cmdBuffer, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, { &image });
	CopyStagingBufferToImage(*cmdBuffer, *stagingBuffer, image);
	ImageLayoutTransition(*cmdBuffer, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, { &image });

	logicalDevice.GetStagingBufferManager().ReleaseStagingBuffer(*cmdBuffer, *stagingBuffer);
}



VulkanGraphicsInterface::~VulkanGraphicsInterface()
{
	delete logicalDevice;
	// TODO - add clean up for vulkan classes
	vkDestroyDebugReportCallbackEXT(instance, debugReportHandle, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void VulkanGraphicsInterface::InitializeGraphics()
{
	CreateInstance();

	logicalDevice = new VulkanDevice();
	logicalDevice->Initialize(instance);

	GetShaderManager().logicalDevice = logicalDevice;
}

VulkanTexture* VulkanGraphicsInterface::CreateEmptyTexture2D(
	uint32 width, 
	uint32 height, 
	ImageFormat textureFormat, 
	uint32 mipLevels,
	TextureUsage::Type usage
)
{
	VkFormat imageFormat = MusaFormatToVkFormat(textureFormat);

	VkImageUsageFlags usageFlags = 0;
 	usageFlags |= (VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

	if (usage & TextureUsage::RenderTarget)
	{
		usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}
	else if (usage & TextureUsage::DepthStencilTarget)
	{
		usageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}

	VulkanImage* image = logicalDevice->GetMemoryManager().AllocateImage(
		width, height, imageFormat, mipLevels, VK_IMAGE_TILING_OPTIMAL,
		usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// TODO - Move this somewhere when refactoring vulkan image creation...
	image->aspectFlags = MusaFormatToVkAspect(textureFormat);

	return new VulkanTexture(*image);
}

VulkanTexture* VulkanGraphicsInterface::CreateInitializedTexture2D(
	const ResourceBlob& textureBlob, 
	uint32 width, uint32 height, 
	ImageFormat textureFormat, 
	uint32 mipLevels, 
	TextureUsage::Type usage)
{
	VkFormat imageFormat = MusaFormatToVkFormat(textureFormat);

	VkImageUsageFlags usageFlags = 0;
	usageFlags |= (VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

	if (usage & TextureUsage::RenderTarget)
	{
		usageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}
	else if (usage & TextureUsage::DepthStencilTarget)
	{
		usageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	}

	VulkanImage* image = logicalDevice->GetMemoryManager().AllocateImage(
		width, height, imageFormat, mipLevels, VK_IMAGE_TILING_OPTIMAL,
		usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	// TODO - Move this somewhere when refactoring vulkan image creation...
	image->aspectFlags = MusaFormatToVkAspect(textureFormat);

	VulkanTexture* tex = new VulkanTexture(*image);

	UploadTextureBlob(*logicalDevice, textureBlob, *image);

	return tex;
}

void VulkanGraphicsInterface::PushTextureData(VulkanTexture& texture, const ResourceBlob& textureBlob)
{
	UploadTextureBlob(*logicalDevice, textureBlob, *texture.image);
}

VulkanFramebuffer* VulkanGraphicsInterface::CreateOrFindFramebuffer(const RenderTargetDescription& targetDesc, const RenderTargetTextures& renderTextures)
{
	return logicalDevice->GetRenderingStorage()->FindOrCreateFramebuffer(targetDesc, renderTextures);
}

TextureSampler VulkanGraphicsInterface::CreateTextureSampler(const TextureSamplerCreateParams& params)
{
	VkSampler sampler = logicalDevice->GetRenderingStorage()->FindOrCreateSampler(params);
	return TextureSampler{ sampler };
}

VulkanDevice* VulkanGraphicsInterface::GetGraphicsDevice()
{
	return logicalDevice;
}

VulkanViewport* VulkanGraphicsInterface::CreateViewport(void * windowHandle, uint32 viewWidth, uint32 viewHeight)
{
	return new VulkanViewport(*logicalDevice, instance, windowHandle, viewWidth, viewHeight);
}

VulkanVertexBuffer* VulkanGraphicsInterface::CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const
{
	return new VulkanVertexBuffer(*logicalDevice, vertices);
}

VulkanIndexBuffer* VulkanGraphicsInterface::CreateIndexBuffer(const DynamicArray<Face>& faces) const
{
	return new VulkanIndexBuffer(*logicalDevice, faces);
}

VulkanUniformBuffer* VulkanGraphicsInterface::CreateUniformBuffer(uint32 bufferSize) const
{
	return new VulkanUniformBuffer(*logicalDevice, bufferSize);
}

void VulkanGraphicsInterface::CreateInstance()
{
	VkDebugReportFlagsEXT debugFlags =
		//VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	VkDebugReportCallbackCreateInfoEXT debugInfo = Vk::DebugReportCallbackInfo(VulkanDebugCallback, debugFlags, this);
	VkInstanceCreateInfo instanceInfo = Vk::InstanceInfo(validationLayers, ArraySize(validationLayers),
		instanceExtensions, ArraySize(instanceExtensions), &debugInfo);
	vkCreateInstance(&instanceInfo, nullptr, &instance);

	// Trying to get around warnings
#ifdef _DEBUG
	void* createDebugFunc = vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	void* destroyDebugFunc = vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	vkCreateDebugReportCallbackEXT = reinterpret_cast<vk_create_debug_report>(createDebugFunc);
	vkDestroyDebugReportCallbackEXT = reinterpret_cast<vk_destroy_debug_report>(destroyDebugFunc);
#endif

	vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportHandle);
}
