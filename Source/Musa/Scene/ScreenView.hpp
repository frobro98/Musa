// Copyright 2020, Nathan Blane

#pragma once

#include "Types/UniquePtr.hpp"
#include "Containers/DynamicArray.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Rect.hpp"
#include "Graphics/RenderTargetDescription.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Camera;
struct ShaderResource;

struct ViewDescription
{
	Vector4 origin;
	Vector4 direction;
	Rect viewport;
	float32 fieldOfView = 0;
	float32 aspectRatio = 0;
};

struct ViewTransforms
{
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
};

// TODO - The view should be produced by the main Viewport somehow
struct View
{
	ViewDescription description;
	ViewTransforms transforms;
	UniquePtr<NativeUniformBuffer> viewBuffer;
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