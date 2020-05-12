// Copyright 2020, Nathan Blane

#pragma once

#include "PlatformDefinitions.h"
#include "Math/IntVector2.hpp"
#include "Types/Uncopyable.hpp"

class WindowInputHandler;

enum class WindowMode : uint32
{
	Windowed = 0,
	Fullscreen = 1,
	Undefined = 2
};

class Window : private Uncopyable
{
public:
	Window(HINSTANCE instance, WindowInputHandler& inputHandler, uint32 xPos, uint32 yPos, uint32 width, uint32 height);
	~Window();

	void SetWindowMode(WindowMode mode);
	void* GetWindowHandle() const;

	void Show();
	void Hide();
	bool IsActive() const;

	void SetAsActiveWindow();
	void Close();
	void Resize(uint32 width, uint32 height);

	inline IntVector2 GetPosition() const { return position; }
	inline int32 GetWidth() const { return width; }
	inline int32 GetHeight() const { return height; }
	inline WindowInputHandler& GetInputHandler() const { return inputHandler; }

private:
	WINDOWPLACEMENT previousPlacement;
	HWND window;
	WindowInputHandler& inputHandler;
	IntVector2 position;
	int32 width;
	int32 height;
	WindowMode windowMode;
	uint32 isShown : 1;
	uint32 isActive : 1;
};