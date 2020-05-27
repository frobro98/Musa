// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "BasicTypes/UniquePtr.hpp"
#include "Graphics/GraphicsResourceDefinitions.hpp"

class Viewport final
{
public:
	Viewport(void* windowHandle, u32 width, u32 height);

	inline NativeViewport& GetNativeViewport() const { return *graphicsViewport; }
	inline u32 GetWidth() const { return viewWidth; }
	inline u32 GetHeight() const { return viewHeight; }

private:
	UniquePtr<NativeViewport> graphicsViewport;
	i32 viewWidth;
	i32 viewHeight;
	//Color clearColor;
};
