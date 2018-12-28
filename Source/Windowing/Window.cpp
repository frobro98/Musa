#include "Window.h"
#include "Graphics.h"
#if !GRAPHICS_API_VK
#include "Memory/MemoryUtilities.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3native.h"

Window::Window(int32 w, int32 h)
	:width(w),
	height(h)
{
}

Window::~Window()
{
	assert(window);
	glfwDestroyWindow(window);
}

void Window::Initialize()
{
	
	window = glfwCreateWindow(width, height, "OpenGL Engine", nullptr, nullptr);
	assert(window);

	glfwSwapInterval(1);
}

void Window::SetAsActiveWindow()
{
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::SetUserPointer(void * userPtr)
{
	glfwSetWindowUserPointer(window, userPtr);
}

void Window::SetKeyboardCallback(GLFWkeyfun callback)
{
	glfwSetKeyCallback(window, callback);
}

bool32 Window::PresentationSupported(GPUHandle /*gpu*/, uint32 /*queueIndex*/) const
{
	return false;
}

bool Window::IsActive() const
{
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_RELEASE) ||
			(glfwWindowShouldClose(window) == GL_TRUE);
}

void Window::Close()
{

}

void Window::Resize()
{

}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

int32 Window::GetWidth() const
{
	return width;
}

int32 Window::GetHeight() const
{
	return height;
}

void * Window::GetWindowHandle() const
{
	return glfwGetWin32Window(window);
}

#endif