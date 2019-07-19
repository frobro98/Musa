#pragma once

#include "Graphics.h"

struct View;
class Window;
class Scene;
class SceneRendering;
class VulkanRenderPass;
class VulkanSwapchain;
class VulkanRenderingCloset;

// This will be the entry point for vulkan and  OpenGL
// TODO - Make this an actual cross-rendering platform renderer
class Renderer
{
public:
	Renderer() = default;
	virtual ~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	virtual void SetupRendering();

	void SetCurrentScene(Scene& scene);
	Scene& GetCurrentScene() const { return *scene; }

	void RenderFrame(const View& view);

private:
	bool PrepareFrame();
	void SubmitFrame();

	// Vulkan specific stuff
private:
	SceneRendering* sceneRendering = nullptr;
	Scene* scene = nullptr;
};
