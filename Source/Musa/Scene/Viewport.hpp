#pragma once

#include "Types/Intrinsics.hpp"

struct NativeViewport;

class Viewport final
{
public:
	Viewport(void* windowHandle, uint32 width, uint32 height);
	~Viewport();

	inline NativeViewport& GetNativeViewport() const { return *graphicsViewport; }
	inline uint32 GetWidth() const { return viewWidth; }
	inline uint32 GetHeight() const { return viewHeight; }

private:
	NativeViewport* graphicsViewport;
	int32 viewWidth;
	int32 viewHeight;
	//Color clearColor;
};
