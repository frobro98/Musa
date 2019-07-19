
#include "RenderingEntity.h"
#include "Graphics/UniformBuffer.h"

#include "Renderer.h"
#include "Model/MeshSceneInfo.hpp"
#include "Model/Mesh.h"
#include "Shader/Material.h"
#include "Texture2D/Texture.h"

// TODO - Remove Vulkan references
#include "Graphics/Vulkan/VulkanMemoryManager.hpp"
#include "Graphics/Vulkan/VulkanMemory.h"
#include "Graphics/Vulkan/VulkanUtilities.h"
#include "Graphics/Vulkan/VulkanCreateInfos.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanStagingBufferManager.hpp"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"


RenderingEntity::RenderingEntity(Renderer* renderer_, MeshRenderInfo& info)
	: descriptorSet(nullptr),
	vertBuffer(info.vertexBuffer),
	weightsBuffer(nullptr),
	indexBuffer(info.indexBuffer),
	renderer(renderer_),
	meshInfo(&info)
{
	transformUBO = VulkanMemory::CreateUniformBuffer(sizeof(TransformationUniformBuffer));
	materialPropsUBO = VulkanMemory::CreateUniformBuffer(sizeof(MaterialProperties));
	viewPropsUBO = VulkanMemory::CreateUniformBuffer(sizeof(ViewProperties));
	poseInversesUBO = VulkanMemory::CreateUniformBuffer(sizeof(PoseInverseUniformBuffer));
	boneDeltasUBO = VulkanMemory::CreateUniformBuffer(sizeof(BoneDeltasUniformBuffer));

	SetupTextures();
}

RenderingEntity::~RenderingEntity()
{
}

// TODO - Each texture that is used like this is in fact a separate allocation
// This needs to be fixed. Each *unique texture should be an allocation
void RenderingEntity::SetupTextures()
{
	MaterialRenderInfo* mat = meshInfo->meshMaterial;
	nativeTexture = mat->baseTexture;
	normalMap = mat->normalMap;

	/*


	if(tex != nullptr)
	{
		// DEBUGGING
		uint32 mipMapLevels = tex->mipLevels.Size();
		uint32 mipMapStartLevel = 0;
		// END DEBUGGING


		Memcpy(stagingBuffer->mappedData, (size_t)stagingBuffer->stagingSize, completeTextureData.GetData(), completeTextureData.Size());

		VkFormat imageFormat = MusaFormatToVkFormat(tex->format);

		nativeTexture.image = logicalDevice.GetMemoryManager().AllocateImage(
			tex->mipLevels[mipMapStartLevel].width, tex->mipLevels[mipMapStartLevel].height, imageFormat, mipMapLevels, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = mipMapLevels;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		VulkanCommandBuffer* cmdBuffer = renderer->GetDevice().GetCmdBufferManager().GetActiveGraphicsBuffer();
		*nativeTexture.image = ImageLayoutTransition(*cmdBuffer, *nativeTexture.image, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyStagingBufferToImage(*cmdBuffer, *stagingBuffer, *nativeTexture.image);
		*nativeTexture.image = ImageLayoutTransition(*cmdBuffer, *nativeTexture.image, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		renderer->GetDevice().GetStagingBufferManager().ReleaseStagingBuffer(*cmdBuffer, *stagingBuffer);

		// Image view creation
		VkImageViewCreateInfo imageViewInfo = Vk::ImageViewInfo(nativeTexture.image->handle, mipMapLevels, nativeTexture.image->format, VK_IMAGE_ASPECT_COLOR_BIT);
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		CHECK_VK(vkCreateImageView(renderer->GetDevice().GetNativeHandle(), &imageViewInfo, nullptr, &nativeTexture.imageView));

		int32 minFilter, magFilter, wrapS, wrapT;
		minFilter = tex->minFilter;
		magFilter = tex->magFilter;
		wrapS = tex->wrapS;
		wrapT = tex->wrapT;

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = static_cast<VkFilter>(magFilter);
		samplerInfo.minFilter = static_cast<VkFilter>(minFilter);
		samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(wrapS);
		samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(wrapT);
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = (float)mipMapLevels;

		CHECK_VK(vkCreateSampler(renderer->GetDevice().GetNativeHandle(), &samplerInfo, nullptr, &nativeTexture.sampler));
	}

	if (normMap != nullptr)
	{
		uint32 mipMapLevels = tex->mipLevels.Size();
		VulkanStagingBuffer* stagingBuffer = renderer->GetDevice().GetStagingBufferManager().AllocateStagingBuffer(normMap->TotalSize());

		DynamicArray<uint8> completeTextureData(normMap->TotalSize());
		uint32 offset = 0;
		for (uint32 i = 0; i < mipMapLevels; ++i)
		{
			Memcpy(completeTextureData.GetData() + offset, normMap->mipLevels[i].Size(), normMap->mipLevels[i].Data(), normMap->mipLevels[i].Size());
			offset += normMap->mipLevels[i].Size();
		}
		Memcpy(stagingBuffer->mappedData, (size_t)stagingBuffer->stagingSize, completeTextureData.GetData(), completeTextureData.Size());

		VkFormat imageFormat = MusaFormatToVkFormat(normMap->format);

		normalMap.image = logicalDevice.GetMemoryManager().AllocateImage(
			normMap->mipLevels[0].width, normMap->mipLevels[0].height, imageFormat, mipMapLevels, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = mipMapLevels;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		VulkanCommandBuffer* cmdBuffer = renderer->GetDevice().GetCmdBufferManager().GetActiveGraphicsBuffer();
		*normalMap.image = ImageLayoutTransition(*cmdBuffer, *normalMap.image, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyStagingBufferToImage(*cmdBuffer, *stagingBuffer, *normalMap.image);
		*normalMap.image = ImageLayoutTransition(*cmdBuffer, *normalMap.image, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		renderer->GetDevice().GetStagingBufferManager().ReleaseStagingBuffer(*cmdBuffer, *stagingBuffer);

		// Image view creation
		VkImageViewCreateInfo imageViewInfo = Vk::ImageViewInfo(normalMap.image->handle, mipMapLevels, normalMap.image->format, VK_IMAGE_ASPECT_COLOR_BIT);
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		CHECK_VK(vkCreateImageView(renderer->GetDevice().GetNativeHandle(), &imageViewInfo, nullptr, &normalMap.imageView));

		int32 minFilter, magFilter, wrapS, wrapT;
		minFilter = normMap->minFilter;
		magFilter = normMap->magFilter;
		wrapS = normMap->wrapS;
		wrapT = normMap->wrapT;

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = static_cast<VkFilter>(magFilter);
		samplerInfo.minFilter = static_cast<VkFilter>(minFilter);
		samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(wrapS);
		samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(wrapT);
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = (float)mipMapLevels;

		CHECK_VK(vkCreateSampler(renderer->GetDevice().GetNativeHandle(), &samplerInfo, nullptr, &normalMap.sampler));
	}

	// TODO - get rid of hack!!!
	if (tex1 != nullptr)
	{
		VulkanStagingBuffer* stagingBuffer = renderer->GetDevice().GetStagingBufferManager().AllocateStagingBuffer(tex1->TotalSize());

		Memcpy(stagingBuffer->mappedData, tex1->mipLevels[0].Size(), tex1->mipLevels[0].imageData.GetData(), tex1->mipLevels[0].imageData.Size());

		VkFormat imageFormat = MusaFormatToVkFormat(tex1->format);

		nativeTexture1.image = logicalDevice.GetMemoryManager().AllocateImage(
			tex1->mipLevels[0].width, tex1->mipLevels[0].height, imageFormat, 1, VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = tex1->mipLevels.Size();

		VulkanCommandBuffer* cmdBuffer = renderer->GetDevice().GetCmdBufferManager().GetActiveGraphicsBuffer();

		*nativeTexture1.image = ImageLayoutTransition(*cmdBuffer, *nativeTexture1.image, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyStagingBufferToImage(*cmdBuffer, *stagingBuffer, *nativeTexture1.image);
		*nativeTexture1.image = ImageLayoutTransition(*cmdBuffer, *nativeTexture1.image, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		renderer->GetDevice().GetStagingBufferManager().ReleaseStagingBuffer(*cmdBuffer, *stagingBuffer);

		// Image view creation
		VkImageViewCreateInfo imageViewInfo = Vk::ImageViewInfo(nativeTexture1.image->handle, tex1->mipLevels.Size(), nativeTexture1.image->format, VK_IMAGE_ASPECT_COLOR_BIT);
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		CHECK_VK(vkCreateImageView(renderer->GetDevice().GetNativeHandle(), &imageViewInfo, nullptr, &nativeTexture1.imageView));

		int32 minFilter, magFilter, wrapS, wrapT;
		minFilter = tex1->minFilter;
		magFilter = tex1->magFilter;
		wrapS = tex1->wrapS;
		wrapT = tex1->wrapT;

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = static_cast<VkFilter>(magFilter);
		samplerInfo.minFilter = static_cast<VkFilter>(minFilter);
		samplerInfo.addressModeU = static_cast<VkSamplerAddressMode>(wrapS);
		samplerInfo.addressModeV = static_cast<VkSamplerAddressMode>(wrapT);
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		CHECK_VK(vkCreateSampler(renderer->GetDevice().GetNativeHandle(), &samplerInfo, nullptr, &nativeTexture1.sampler));
	}

	*/
}
