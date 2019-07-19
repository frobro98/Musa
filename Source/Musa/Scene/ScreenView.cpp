#include "ScreenView.hpp"
#include "Assertion.h"
#include "Math/MatrixUtilities.hpp"
#include "Camera/Camera.h"
#include "Shader/ShaderObjects/ShaderResource.hpp"
#include "Shader/ShaderObjects/ShaderDefinition.hpp"
#include "Shader/ShaderObjects/ScreenRendering.hpp"

// TODO - Remove all traces of vulkan from here
#include "Graphics/Vulkan/VulkanDescriptorLayoutManager.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include "Graphics/Vulkan/VulkanShaderManager.h"
#include "Graphics/Vulkan/VulkanMemory.h"
#include "Graphics/UniformBuffer.h"

ScreenView::ScreenView(int32 screenWidth, int32 screenHeight)
	: width(screenWidth), height(screenHeight)
{
	Assert(screenWidth > 0);
	Assert(screenHeight > 0);

	VulkanDescriptorSetLayout* descriptorSetLayout = GetDescriptorLayoutManager().CreateSetLayout();
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 2);
	//descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 3);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 4);
	descriptorSetLayout->AddDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 5);
	descriptorSetLayout->BindLayout();

	screenVertexShader = &GetShader<ScreenRenderVert>()->GetNativeShader();
	screenFragmentShader = &GetShader<ScreenRenderFrag>()->GetNativeShader();
}

int32 ScreenView::GetScreenWidth() const
{
	return width;
}

int32 ScreenView::GetScreenHeight() const
{
	return height;
}

ShaderResource* ScreenView::GetScreenVertexShader() const
{
	return screenVertexShader;
}

ShaderResource* ScreenView::GetScreenFragmentShader() const
{
	return screenFragmentShader;
}

void ScreenView::AssociateCameraWithView(const Camera& camera)
{
	view.transforms.projectionMatrix = Math::ConstructPerspectiveMatrix(camera.GetFov(), camera.GetAspectRatio(), camera.GetNearPlane(), camera.GetFarPlane());
	view.transforms.viewMatrix = Math::ConstructViewMatrix(camera.GetPosition(), camera.GetForward(), camera.GetRight(), camera.GetUp());

	view.description.direction = camera.GetForward();
	view.description.origin = camera.GetPosition();
	view.description.viewport = ToRect(camera.GetCameraViewport());
	view.description.fieldOfView = camera.GetFov();
	view.description.aspectRatio = camera.GetAspectRatio();

	if (view.viewBuffer == nullptr)
	{
		view.viewBuffer = VulkanMemory::CreateUniformBuffer(sizeof(ViewProperties));
	}

	ViewProperties props;
	props.viewPosition = view.description.origin;
	view.viewBuffer->UpdateUniforms(&props);
}
