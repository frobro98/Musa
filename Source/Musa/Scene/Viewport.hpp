// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"
#include "Types/UniquePtr.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Viewport final
{
public:
	Viewport(void* windowHandle, uint32 width, uint32 height);

	inline NativeViewport& GetNativeViewport() const { return *graphicsViewport; }
	inline uint32 GetWidth() const { return viewWidth; }
	inline uint32 GetHeight() const { return viewHeight; }

private:
	UniquePtr<NativeViewport> graphicsViewport;
	int32 viewWidth;
	int32 viewHeight;
	//Color clearColor;
};
