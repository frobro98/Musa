#pragma once

#include "Math/Vector4.hpp"
#include "GameObject/GameObject.h"

class ScaleChangingObject : public GameObject
{
public:
	ScaleChangingObject(GameWorld& world, float32 scaleA, float32 scaleB, float32 delta);

	virtual void Update(float32 tick) override;

private:
	Vector4 A, B;
	Vector4* srcVec;
	Vector4* dstVec;
	float32 delta;
	float32 totalDelta = 0.f;
};