#pragma once

#include "PlatformDefinitions.h"
#include "Graphics.h"

class WindowInputHandler;

enum class WindowMode : uint32
{
	Windowed = 0,
	Fullscreen = 1,
	Undefined = 2
};

class Window
{
public:
	Window(HINSTANCE instance, WindowInputHandler& inputHandler, uint32 xPos, uint32 yPos, uint32 width, uint32 height);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetWindowMode(WindowMode mode);
	int32 GetWidth() const;
	int32 GetHeight() const;
	void* GetWindowHandle() const;

	void Show();
	void Hide();
	bool IsActive() const;
	void SwapBuffers();

	void SetAsActiveWindow();
	void Close();
	void Resize();

private:
	WINDOWPLACEMENT previousPlacement;
	HWND window;

	int32 width;
	int32 height;
	WindowMode windowMode;
	uint32 isShown : 1;
	uint32 isActive : 1;
};