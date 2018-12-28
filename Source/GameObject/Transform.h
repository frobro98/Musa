#pragma once

#include "Math/MathEngine.h"

class Transform
{
public:
	Transform();
	~Transform() = default;

	Vector GetForward() const;
	Vector GetRight() const;
	Vector GetUp() const;

private:
	Vector forward;
	Vector right;
	Vector up;
	Vector position;
	
};