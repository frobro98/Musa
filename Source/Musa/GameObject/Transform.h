#pragma once

#include "Math/MathEngine.h"

class Transform
{
public:
	Transform();
	~Transform() = default;

	Vector4 GetForward() const;
	Vector4 GetRight() const;
	Vector4 GetUp() const;

private:
	Vector4 forward;
	Vector4 right;
	Vector4 up;
	Vector4 position;
	
};