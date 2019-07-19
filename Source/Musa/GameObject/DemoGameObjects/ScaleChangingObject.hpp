#pragma once

#include "Math/Vector.h"
#include "GameObject/GameObject.h"

class ScaleChangingObject : public GameObject
{
public:
	ScaleChangingObject(float scaleA, float scaleB);

	virtual void Update(float tick) override;

private:
	Vector A, B;
	Vector* srcVec;
	Vector* dstVec;
	float totalDelta = 0.f;
};