// Copyright 2020, Nathan Blane

#include "ScreenView.hpp"
#include "Assertion.h"
#include "Math/MatrixUtilities.hpp"
#include "Camera/Camera.h"
#include "Shader/ShaderDefinition.hpp"
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

NativeVertexShader* ScreenView::GetScreenVertexShader() const
{
	return screenVertexShader;
}

NativeFragmentShader* ScreenView::GetScreenFragmentShader() const
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

	if (!view.viewBuffer.IsValid())
	{
		view.viewBuffer = GetGraphicsInterface().CreateUniformBuffer(sizeof(ViewPropertiesBuffer));
	}

	ViewPropertiesBuffer props;
	props.viewTransform = view.transforms.viewMatrix;
	props.projectionTransform = view.transforms.projectionMatrix;
	props.viewPosition = view.description.origin;
	GetGraphicsInterface().PushBufferData(*view.viewBuffer, &props);
}
