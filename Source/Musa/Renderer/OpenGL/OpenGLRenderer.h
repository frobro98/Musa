#pragma once

#include "Renderer/Renderer.h"

#if !GRAPHICS_API_VK

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	OpenGLRenderer(const OpenGLRenderer&) = delete;
	OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;

	virtual void Initialize() override;
	virtual void SetupRendering(const Window* window) override;
};

typedef OpenGLRenderer RenderingEngine;

#endif