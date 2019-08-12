#include "ScreenView.hpp"
#include "Assertion.h"
#include "Math/MatrixUtilities.hpp"
#include "Camera/Camera.h"
#include "Shader/ShaderDefinition.hpp"
#include "Shader/ShaderResource.hpp"
#include "Shader/ShaderObjects/ScreenRendering.hpp"
#include "Graphics/GraphicsInterface.hpp"
#include "Graphics/UniformBuffers.h"

ScreenView::ScreenView(int32 screenWidth, int32 screenHeight)
	: width(screenWidth), height(screenHeight)
{
	Assert(screenWidth > 0);
	Assert(screenHeight > 0);

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
		view.viewBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(ViewPropertiesBuffer));
	}

	ViewPropertiesBuffer props;
	props.viewTransform = view.transforms.viewMatrix;
	props.projectionTransform = view.transforms.projectionMatrix;
	props.viewPosition = view.description.origin;
	GetGraphicsInterface().PushBufferData(*view.viewBuffer, &props);
}
