#pragma once

#include "Containers/DynamicArray.hpp"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Rect.hpp"
#include "Graphics/RenderTargetDescription.hpp"

// TODO - Remove all traces of vulkan from here
#include "Graphics/Vulkan/VulkanUniformBuffer.h"

class Camera;
struct ShaderResource;

struct ViewDescription
{
	Vector origin;
	Vector direction;
	Rect viewport;
	float32 fieldOfView;
	float32 aspectRatio;
};

struct ViewTransforms
{
	Matrix viewMatrix;
	Matrix projectionMatrix;
};

// TODO - The view should be produced by the main Viewport somehow
struct View
{
	ViewDescription description;
	ViewTransforms transforms;
	UniformBuffer* viewBuffer = nullptr;
};

// Represents the entire screen's view into the scene.
// Can be broken up into multiple views
class ScreenView
{
public:
	ScreenView(int32 screenWidth, int32 screenHeight);

	int32 GetScreenWidth() const;
	int32 GetScreenHeight() const;
	ShaderResource* GetScreenVertexShader() const;
	ShaderResource* GetScreenFragmentShader() const;

	void AssociateCameraWithView(const Camera& cam);

public:
	View view;

private:
	ShaderResource* screenVertexShader = nullptr;
	ShaderResource* screenFragmentShader = nullptr;
	int32 width;
	int32 height;
};