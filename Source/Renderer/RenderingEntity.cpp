
#include "RenderingEntity.h"
#include "UniformBuffer.h"
#include "Vulkan/VulkanMemory.h"
#include "Vulkan/VulkanUtilites.h"
#include "Vulkan/VulkanCreateInfos.h"
#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanStageBuffers.h"

#include "Renderer.h"
#include "UniformBuffer.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Shader/Material.h"
#include "Texture2D/Texture.h"

// TODO - move this to a place that makes sense, like a utility place
static VkFormat MusaToVkFormat(ImageFormat format)
{
	switch (format)
	{
		case ImageFormat::RGB_8:
			return VK_FORMAT_R8G8B8_UNORM;
		case ImageFormat::BGR_8:
			return VK_FORMAT_B8G8R8_UNORM;
		case ImageFormat::RGBA_8:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case ImageFormat::BGRA_8:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case ImageFormat::Gray_8:
			return VK_FORMAT_R8_UNORM;
		case ImageFormat::BC1:
			return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
		case ImageFormat::BC3:
			return VK_FORMAT_BC3_UNORM_BLOCK;
		case ImageFormat::BC7:
			return VK_FORMAT_BC7_UNORM_BLOCK;
		case ImageFormat::Invalid:
		default:
			Assert(false);
	}

	return VK_FORMAT_UNDEFINED;
}

RenderingEntity::RenderingEntity(Renderer* renderer_, Model* model)
	: vertBuffer(model->GetMesh()->GetVertices()),
	weightsBuffer(model->GetMesh()->GetSkinWeights()),
	indexBuffer(model->GetMesh()->GetFaces()),
	descriptorSet(nullptr),
	renderer(renderer_),
	referencedModel(model)
{
	constexpr const uint32 numIndicesPerFace = 3;
	numMeshIndices = model->GetMesh()->GetFaces().Size() * numIndicesPerFace;
	transformUBO.Initialize(sizeof(TransformationUniformBuffer));
	materialPropsUBO.Initialize(sizeof(MaterialProperties));
	viewPropsUBO.Initialize(sizeof(ViewProperties));
	poseInversesUBO.Initialize(sizeof(PoseInverseUniformBuffer));
	boneDeltasUBO.Initialize(sizeof(BoneDeltasUniformBuffer));

	SetupTextures();
}

RenderingEntity::~RenderingEntity()
{
}

void RenderingEntity::SetDescriptorSet(VulkanDescriptorSet * ds)
{
	descriptorSet = ds;
}

void RenderingEntity::SetupTextures()
{
	Material* mat = referencedModel->GetMaterial();
	Texture* tex = mat->GetTexture0();
	Texture* tex1 = mat->GetTexture1();

	if(tex != nullptr)
	{
		// DEBUGGING
		uint32 mipMapLevels = tex->mipLevels.Size();
		uint32 mipMapStartLevel = 0;
		// END DEBUGGING

		//*
		VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(tex->TotalSize(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		Array<uint8> completeTextureData(tex->TotalSize());
		uint32 offset = 0;
		for (uint32 i = 0; i < mipMapLevels; ++i)
		{
			Memcpy(completeTextureData.GetData() + offset, tex->mipLevels[i].Size(), tex->mipLevels[i].Data(), tex->mipLevels[i].Size());
			offset += tex->mipLevels[i].Size();
		}
		//*/

		/*
		VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(tex->mipLevels[mipMapStartLevel].Size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		Array<uint8> completeTextureData(tex->mipLevels[mipMapStartLevel].Size());
		Memcpy(completeTextureData.GetData(), tex->mipLevels[mipMapStartLevel].Size(), tex->mipLevels[mipMapStartLevel].Data(), tex->mipLevels[mipMapStartLevel].Size());
		//*/

		void* data = VulkanMemory::MapMemory(stagingBuffer.allocatedMemory, stagingBuffer.size);
		Memcpy(data, (size_t)stagingBuffer.size, completeTextureData.GetData(), completeTextureData.Size());
		VulkanMemory::UnmapMemory(stagingBuffer.allocatedMemory);

		VkFormat imageFormat = MusaToVkFormat(tex->format);

		nativeTexture.image = VulkanMemory::AllocateImage(
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

		nativeTexture.image = ImageLayoutTransition(nativeTexture.image, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer, nativeTexture.image);
		nativeTexture.image = ImageLayoutTransition(nativeTexture.image, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		VulkanMemory::DeallocateBuffer(stagingBuffer);

		// Image view creation
		VkImageViewCreateInfo imageViewInfo = Vk::ImageViewInfo(nativeTexture.image.handle, mipMapLevels, nativeTexture.image.format, VK_IMAGE_ASPECT_COLOR_BIT);
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		CHECK_VK(vkCreateImageView(renderer->GetDevice()->GetNativeHandle(), &imageViewInfo, nullptr, &nativeTexture.imageView));

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
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = (float)mipMapLevels;

		CHECK_VK(vkCreateSampler(renderer->GetDevice()->GetNativeHandle(), &samplerInfo, nullptr, &nativeTexture.sampler));
	}

	// TODO - get rid of hack!!!
	if (tex1 != nullptr)
	{
		VulkanBuffer stagingBuffer = VulkanMemory::AllocateBuffer(tex1->mipLevels[0].Size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data = VulkanMemory::MapMemory(stagingBuffer.allocatedMemory, tex1->mipLevels[0].Size());
		Memcpy(data, tex1->mipLevels[0].Size(), tex1->mipLevels[0].imageData.GetData(), tex1->mipLevels[0].imageData.Size());
		VulkanMemory::UnmapMemory(stagingBuffer.allocatedMemory);

		VkFormat imageFormat = MusaToVkFormat(tex1->format);

		nativeTexture1.image = VulkanMemory::AllocateImage(
			tex1->mipLevels[0].width, tex1->mipLevels[0].height, imageFormat, 1, VK_IMAGE_TILING_LINEAR,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = tex1->mipLevels.Size();

		nativeTexture1.image = ImageLayoutTransition(nativeTexture1.image, subresourceRange, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		CopyBufferToImage(stagingBuffer, nativeTexture1.image);
		nativeTexture1.image = ImageLayoutTransition(nativeTexture1.image, subresourceRange, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		VulkanMemory::DeallocateBuffer(stagingBuffer);

		// Image view creation
		VkImageViewCreateInfo imageViewInfo = Vk::ImageViewInfo(nativeTexture1.image.handle, tex1->mipLevels.Size(), nativeTexture1.image.format, VK_IMAGE_ASPECT_COLOR_BIT);
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		CHECK_VK(vkCreateImageView(renderer->GetDevice()->GetNativeHandle(), &imageViewInfo, nullptr, &nativeTexture1.imageView));

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
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		CHECK_VK(vkCreateSampler(renderer->GetDevice()->GetNativeHandle(), &samplerInfo, nullptr, &nativeTexture1.sampler));
	}
}
