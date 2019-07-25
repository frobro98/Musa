#include "Scene.hpp"
#include "Platform.h"
#include "ScreenView.hpp"
#include "Camera/Camera.h"
#include "Camera/CameraManager.h"
#include "Camera/ViewFrustum.hpp"
#include "GameObject/GameObject.h"
#include "Thread/JobSystem/JobUtilities.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "GameObject/RenderObjectManager.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "RenderPipeline/SceneRendering.h"

// TODO - Get rid of these vulkan includes
#include "Graphics/Vulkan/VulkanViewport.hpp"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanStagingBufferManager.hpp"


namespace
{

void BuildPositionGBuffer(ColorDescription& posGbuffer)
{
	posGbuffer.format = ImageFormat::RGBA_16f;
	posGbuffer.load = LoadOperation::Clear;
	posGbuffer.store = StoreOperation::Store;
	posGbuffer.stencilLoad = LoadOperation::DontCare;
	posGbuffer.stencilStore = StoreOperation::DontCare;
	posGbuffer.sampleCount = 1;
}

void BuildNormalGBuffer(ColorDescription& normalGBuffer)
{
	normalGBuffer.format = ImageFormat::RGBA_16f;
	normalGBuffer.load = LoadOperation::Clear;
	normalGBuffer.store = StoreOperation::Store;
	normalGBuffer.stencilLoad = LoadOperation::DontCare;
	normalGBuffer.stencilStore = StoreOperation::DontCare;
	normalGBuffer.sampleCount = 1;
}

void BuildDiffuseGBuffer(ColorDescription& diffuseGBuffer)
{
	diffuseGBuffer.format = ImageFormat::RGBA_8norm;
	diffuseGBuffer.load = LoadOperation::Clear;
	diffuseGBuffer.store = StoreOperation::Store;
	diffuseGBuffer.stencilLoad = LoadOperation::DontCare;
	diffuseGBuffer.stencilStore = StoreOperation::DontCare;
	diffuseGBuffer.sampleCount = 1;
}

}

// TODO - REMOVE THESE HEADERS!!!
#include "Graphics/Vulkan/VulkanMemory.h"
#include "Graphics/Vulkan/VulkanUtilities.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanCommandBuffer.h"
#include "Graphics/Vulkan/VulkanMemoryManager.hpp"

VulkanTexture* CreateColorRenderTexture(const ColorDescription& desc, const ScreenView& view)
{
	VkFormat format = MusaFormatToVkFormat(desc.format);
	VulkanImage* image = GetGraphicsInterface().GetGraphicsDevice()->GetMemoryManager().AllocateImage(
		(uint32)view.GetScreenWidth(), (uint32)view.GetScreenHeight(), 
		format, 1, VK_IMAGE_TILING_OPTIMAL, 
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	image->aspectFlags = MusaFormatToVkAspect(desc.format);

	VulkanDevice& device = *GetGraphicsInterface().GetGraphicsDevice();
	VulkanCommandBuffer* cmdBuffer = device.GetCmdBufferManager().GetActiveGraphicsBuffer();

	VkImageSubresourceRange range = {};
	range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	range.baseArrayLayer = 0;
	range.layerCount = 1;
	range.baseMipLevel = 0;
	range.levelCount = 1;

	ImageLayoutTransition(*cmdBuffer, range, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, *image);
	VulkanTexture* tex = new VulkanTexture(*image);

	return tex;
}

VulkanTexture* CreateDepthRenderTexture(const DepthStencilDescription& desc, const ScreenView& view)
{
	VkFormat format = MusaFormatToVkFormat(desc.format);
	VulkanImage* image = GetGraphicsInterface().GetGraphicsDevice()->GetMemoryManager().AllocateImage(
		(uint32)view.GetScreenWidth(), (uint32)view.GetScreenHeight(),
		format, 1, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	image->aspectFlags = MusaFormatToVkAspect(desc.format);

	VulkanDevice& device = *GetGraphicsInterface().GetGraphicsDevice();
	VulkanCommandBuffer* cmdBuffer = device.GetCmdBufferManager().GetActiveGraphicsBuffer();

	VkImageSubresourceRange range = {};
	range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	range.baseArrayLayer = 0;
	range.layerCount = 1;
	range.baseMipLevel = 0;
	range.levelCount = 1;

	ImageLayoutTransition(*cmdBuffer, range, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, *image);

	VulkanTexture* tex = new VulkanTexture(*image);

	return tex;
}


void Scene::InitializeScene()
{
	BuildPositionGBuffer(gbufferTargets.colorDescs[0]);
	BuildNormalGBuffer(gbufferTargets.colorDescs[1]);
	BuildDiffuseGBuffer(gbufferTargets.colorDescs[2]);

	gbufferTargets.depthDesc.format = ImageFormat::DS_32f_8u;
	gbufferTargets.depthDesc.load = LoadOperation::Clear;
	gbufferTargets.depthDesc.store = StoreOperation::Store;
	gbufferTargets.depthDesc.stencilLoad = LoadOperation::DontCare;
	gbufferTargets.depthDesc.stencilStore = StoreOperation::DontCare;
	gbufferTargets.depthDesc.sampleCount = 1;

	gbufferTargets.targetCount = GBufferCount;

	sceneRendering = new SceneRendering;
}

void Scene::AddGameObjectToScene(GameObject& object)
{
	activeGameObjects.Add(&object);
}

void Scene::RemoveGameObjectFromScene(GameObject& object)
{
	activeGameObjects.RemoveAll(&object);
}

void Scene::AddMeshInfoToScene(MeshRenderInfo& obj)
{
	renderingInfo.Add(&obj);
}

void Scene::RemoveMeshInfoFromScene(MeshRenderInfo& obj)
{
	renderingInfo.RemoveAll(&obj);
}

void Scene::AddLightToScene(Light& light)
{
	bool emptySlotFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == nullptr)
		{
			lights[i] = &light;
			emptySlotFound = true;
			break;
		}
	}

	Assert(emptySlotFound);
}

void Scene::RemoveLightFromScene(Light& light)
{
	bool lightFound = false;
	for (uint32 i = 0; i < ArraySize(lights); ++i)
	{
		if (lights[i] == &light)
		{
			lights[i] = nullptr;
			lightFound = true;
			break;
		}
	}

	Assert(lightFound);
}

void Scene::Tick(float deltaTime)
{
	for (auto& go : activeGameObjects)
	{
		//PushConcurrentJobUnder(
			//GetActiveJobParent(),
			//[=]
			//{
				go->Update(deltaTime);
			//}
		//);
	}
}

void Scene::PushStateToGpu()
{
	Camera* cam = GetCameraManager().GetActiveCamera();
	view->AssociateCameraWithView(*cam);

	GetRenderObjectManager().SequenciallyPull(view->view);
}

void Scene::RenderScene(Viewport& viewport)
{
	if (!gbuffersInitialized)
	{
		CreateGBuffer();
		gbuffersInitialized = true;
	}

	if (viewport.graphicsViewport->CanProceedWithRender())
	{
		GetGraphicsInterface().GetGraphicsDevice()->GetStagingBufferManager().ProcessDeferredReleases();

		sceneRendering->RenderScene(*this, viewport, view->view);

		viewport.graphicsViewport->SubmitFrame();
	}
}

void Scene::SetView(ScreenView& view_)
{
	view = &view_;

	gbufferTargets.targetExtents = { (float32)view->GetScreenWidth(), (float32)view->GetScreenHeight() };
}

void Scene::CreateGBuffer()
{
	gbufferTextures.targetCount = GBufferCount;

	for (uint32 i = 0; i < GBufferCount; ++i)
	{
		ColorDescription& desc = gbufferTargets.colorDescs[i];
		gbufferTextures.colorTargets[i] = GetGraphicsInterface().CreateEmptyTexture2D(
			view->GetScreenWidth(), view->GetScreenHeight(), 
			desc.format, 1, TextureUsage::RenderTarget
		);
	}

	gbufferTextures.depthTarget = GetGraphicsInterface().CreateEmptyTexture2D(
		view->GetScreenWidth(), view->GetScreenHeight(),
		gbufferTargets.depthDesc.format, 1, 
		TextureUsage::DepthStencilTarget
	);
}
