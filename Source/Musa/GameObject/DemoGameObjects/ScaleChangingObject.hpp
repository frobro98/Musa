#pragma once

#include "Math/Vector4.hpp"
#include "GameObject/GameObject.h"

class ScaleChangingObject : public GameObject
{
public:
	ScaleChangingObject(GameWorld& world, float scaleA, float scaleB);

	virtual void Update(float tick) override;

private:
	Vector4 A, B;
	Vector4* srcVec;
	Vector4* dstVec;
	float totalDelta = 0.f;
};