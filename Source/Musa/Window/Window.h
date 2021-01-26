// Copyright 2020, Nathan Blane

#pragma once

#include "Math/IntVector2.hpp"
#include "BasicTypes/Uncopyable.hpp"

class MusaApp;

enum class WindowMode : u32
{
	Windowed = 0,
	Fullscreen = 1,
	Undefined = 2
};

class Window : private Uncopyable
{
public:
	Window(HINSTANCE instance, MusaApp& app, u32 xPos, u32 yPos, u32 width, u32 height);
	~Window();

	void SetWindowMode(WindowMode mode);
	void* GetWindowHandle() const;

	void Show();
	void Hide();
	bool IsActive() const;

	void SetAsActiveWindow();
	void Close();
	void Resize(u32 width, u32 height);

	inline IntVector2 GetPosition() const { return position; }
	inline i32 GetWidth() const { return width; }
	inline i32 GetHeight() const { return height; }

private:
	WINDOWPLACEMENT previousPlacement;
	HWND window;
	MusaApp& application;
	IntVector2 position;
	i32 width;
	i32 height;
	WindowMode windowMode;
	u32 isShown : 1;
	u32 isActive : 1;
};