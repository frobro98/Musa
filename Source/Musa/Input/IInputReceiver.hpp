#pragma once

class IInputReceiver
{
public:
	virtual void OnMouseMove() = 0;
	virtual void OnKeyDown() = 0;
	virtual void OnKeyUp() = 0;
	virtual void OnChar() = 0;
};
