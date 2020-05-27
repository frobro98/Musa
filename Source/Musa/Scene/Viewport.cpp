// Copyright 2020, Nathan Blane

#include "Viewport.hpp"
#include "Graphics/GraphicsInterface.hpp"


Viewport::Viewport(void* windowHandle, u32 width, u32 height)
	: viewWidth(width),
	viewHeight(height)
{
	graphicsViewport = GetGraphicsInterface().CreateViewport(windowHandle, viewWidth, viewHeight);
}
