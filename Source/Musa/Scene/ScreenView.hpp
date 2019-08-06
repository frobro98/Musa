#pragma once

#include "Containers/DynamicArray.hpp"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Rect.hpp"
#include "Graphics/RenderTargetDescription.hpp"

class Camera;
struct ShaderResource;
struct NativeUniformBuffer;

struct ViewDescription
{
	Vector origin;
	Vector direction;
	Rect viewport;
	float32 fieldOfView = 0;
	float32 aspectRatio = 0;
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
	NativeUniformBuffer* viewBuffer = nullptr;
};

// TODO - Determine if this and the Viewport class are similar and try and consolidate if they are
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