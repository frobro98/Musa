#include "Viewport.hpp"
#include "Graphics/GraphicsInterface.hpp"


Viewport::Viewport(void* windowHandle, uint32 width, uint32 height)
	: viewWidth(width),
	viewHeight(height)
{
	graphicsViewport = GetGraphicsInterface().CreateViewport(windowHandle, viewWidth, viewHeight);
}

Viewport::~Viewport()
{
}
