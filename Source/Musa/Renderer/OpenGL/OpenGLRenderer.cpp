
#include "Platform.h"
#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer()
{
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::Initialize()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::SetupRendering(const Window* window)
{
	assert(window != nullptr);
}
