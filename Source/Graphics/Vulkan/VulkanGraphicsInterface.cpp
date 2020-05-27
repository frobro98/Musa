// Copyright 2020, Nathan Blane

#include "fmt/format.h"
#include "Utilities/Array.hpp"

#include "ResourceBlob.hpp"
#include "Serialization/MemoryDeserializer.hpp"
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
#include "VulkanSampler.hpp"
#include "VulkanRenderContext.hpp"
#include "VulkanTexture.h"
#include "VulkanDescriptorLayoutManager.h"
#include "VulkanShaderHeader.hpp"
#include "VulkanShaders.h"

constexpr const tchar* validationLayers[] = {
	"VK_LAYER_GOOGLE_threading",
	"VK_LAYER_LUNARG_parameter_validation",
	"VK_LAYER_LUNARG_object_tracker",
	"VK_LAYER_LUNARG_image",
	"VK_LAYER_LUNARG_core_validation",
	"VK_LAYER_LUNARG_swapchain",
	"VK_LAYER_GOOGLE_unique_objects"
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
//#if M_DEBUG

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		fmt::print("WARNING: ");
		fmt::print("@[{}]: {}\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		fmt::print("PERFORMANCE: ");
		fmt::print("@[{}]: {}\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		fmt::print("ERROR: ");
		fmt::print("@[{}]: {}\n", layerPrefix, msg);
	}
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
	{
		fmt::print("DEBUG: ");
		fmt::print("@[{}]: {}\n", layerPrefix, msg);
	}

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
	{
		fmt::print("INFO: ");
		fmt::print("@[{}]: {}\n", layerPrefix, msg);
	}

//#endif // M_DEBUG

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

//////////////////////////////////////////////////////////////////////////

void VulkanGraphicsInterface::InitializeGraphics()
{
	CreateInstance();

	logicalDevice = new VulkanDevice();
	logicalDevice->Initialize(instance);

	renderContext = new VulkanRenderContext(*logicalDevice);
}

void VulkanGraphicsInterface::DeinitializeGraphics()
{
	GetDescriptorLayoutManager().Deinitialize();

	renderContext.Reset();
	logicalDevice.Reset();
	// TODO - add clean up for vulkan classes
	vkDestroyDebugReportCallbackEXT(instance, debugReportHandle, nullptr);
	vkDestroyInstance(instance, nullptr);
}

UniquePtr<NativeVertexShader> VulkanGraphicsInterface::CreateVertexShader(const MemoryBuffer& vertexCode)
{
	MemoryDeserializer memoryBuffer(vertexCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;

	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_VERTEX_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanVertexShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeFragmentShader> VulkanGraphicsInterface::CreateFragmentShader(const MemoryBuffer& fragmentCode)
{
	MemoryDeserializer memoryBuffer(fragmentCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;

	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_FRAGMENT_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanFragmentShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeGeometryShader> VulkanGraphicsInterface::CreateGeometryShader(const MemoryBuffer& geometryCode)
{
	MemoryDeserializer memoryBuffer(geometryCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;

	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_GEOMETRY_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanGeometryShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeTessEvaluationShader> VulkanGraphicsInterface::CreateTessEvaluationShader(const MemoryBuffer& tessEvalCode)
{
	MemoryDeserializer memoryBuffer(tessEvalCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;

	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanTessEvaluationShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeTessControlShader> VulkanGraphicsInterface::CreateTessControlShader(const MemoryBuffer& tessCtrlCode)
{
	MemoryDeserializer memoryBuffer(tessCtrlCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;

	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanTessControlShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeComputeShader> VulkanGraphicsInterface::CreateComputeShader(const MemoryBuffer& computeCode)
{
	MemoryDeserializer memoryBuffer(computeCode);

	VulkanShaderHeader header;
	DynamicArray<u32> byteCode;
	
	Deserialize(memoryBuffer, header);
	Assert(header.shaderStage == VK_SHADER_STAGE_COMPUTE_BIT);
	Deserialize(memoryBuffer, byteCode);

	return MakeUnique<VulkanComputeShader>(*logicalDevice, header, byteCode);
}

UniquePtr<NativeTexture> VulkanGraphicsInterface::CreateEmptyTexture2D(
	u32 width, 
	u32 height, 
	ImageFormat textureFormat, 
	u32 mipLevels,
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

	return new VulkanTexture(*logicalDevice, *image);
}

UniquePtr<NativeTexture> VulkanGraphicsInterface::CreateInitializedTexture2D(
	const ResourceBlob& textureBlob, 
	u32 width, u32 height, 
	ImageFormat textureFormat, 
	u32 mipLevels, 
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

	VulkanTexture* tex = new VulkanTexture(*logicalDevice, *image);

	UploadTextureBlob(*logicalDevice, textureBlob, *image);

	return tex;
}

void VulkanGraphicsInterface::PushTextureData(NativeTexture& texture, const ResourceBlob& textureBlob)
{
	VulkanTexture* tex = static_cast<VulkanTexture*>(&texture);
	UploadTextureBlob(*logicalDevice, textureBlob, *tex->image);
}

NativeSampler* VulkanGraphicsInterface::CreateTextureSampler(const SamplerDescription& params)
{
	return new VulkanSampler(*logicalDevice, params);
}

UniquePtr<NativeViewport> VulkanGraphicsInterface::CreateViewport(void * windowHandle, u32 viewWidth, u32 viewHeight)
{
	return new VulkanViewport(*logicalDevice, instance, windowHandle, viewWidth, viewHeight);
}

UniquePtr<NativeVertexBuffer> VulkanGraphicsInterface::CreateVertexBuffer(const DynamicArray<Vertex>& vertices) const
{
	return new VulkanVertexBuffer(*logicalDevice, vertices);
}

UniquePtr<NativeIndexBuffer> VulkanGraphicsInterface::CreateIndexBuffer(const DynamicArray<Face>& faces) const
{
	return new VulkanIndexBuffer(*logicalDevice, faces);
}

UniquePtr<NativeUniformBuffer> VulkanGraphicsInterface::CreateUniformBuffer(u32 bufferSize) const
{
	return new VulkanUniformBuffer(*logicalDevice, bufferSize);
}

void VulkanGraphicsInterface::PushBufferData(NativeUniformBuffer& buffer, const void* data) const
{
	VulkanUniformBuffer* uniformBuffer = static_cast<VulkanUniformBuffer*>(&buffer);
	uniformBuffer->UpdateUniforms(data);
}

void* VulkanGraphicsInterface::GetGraphicsDevice()
{
	return logicalDevice->GetNativeHandle();
}

RenderContext* VulkanGraphicsInterface::GetRenderContext()
{
	return renderContext.Get();
}

void VulkanGraphicsInterface::CreateInstance()
{
	VkDebugReportFlagsEXT debugFlags =
		//VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	u32 layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	DynamicArray<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.GetData());

	VkDebugReportCallbackCreateInfoEXT debugInfo = Vk::DebugReportCallbackInfo(VulkanDebugCallback, debugFlags, this);
	VkInstanceCreateInfo instanceInfo = Vk::InstanceInfo(validationLayers, (u32)ArraySize(validationLayers),
		instanceExtensions, (u32)ArraySize(instanceExtensions), &debugInfo);
	NOT_USED VkResult result = vkCreateInstance(&instanceInfo, nullptr, &instance);
	CHECK_VK(result);

	// Trying to get around warnings
//#if M_DEBUG
	void* createDebugFunc = vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	void* destroyDebugFunc = vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	vkCreateDebugReportCallbackEXT = reinterpret_cast<vk_create_debug_report>(createDebugFunc);
	vkDestroyDebugReportCallbackEXT = reinterpret_cast<vk_destroy_debug_report>(destroyDebugFunc);

	result = vkCreateDebugReportCallbackEXT(instance, &debugInfo, nullptr, &debugReportHandle);
	CHECK_VK(result);
//#endif
}
