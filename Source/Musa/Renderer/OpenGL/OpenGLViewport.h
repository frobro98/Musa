#pragma once

#include "Graphics.h"

class OpenGLViewport
{
public:

	OpenGLViewport(int32 width, int32 height, const Color& clearColor = Color(0.f, 0.f, 0.f, 1.f));
	OpenGLViewport(int32 x, int32 y, int32 width, int32 height, const Color& clearColor = Color(0.f, 0.f, 0.f, 1.f));

	void Update();

private:
	int32 x;
	int32 y;
	int32 width;
	int32 height;
	Color clearColor;
};