#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector.h"

class MoveBetweenObject : public GameObject
{
public:
	MoveBetweenObject(const Vector& pointA, const Vector& pointB, float startingDelta = 0.f);

	virtual void Update(float tick) override;

private:
	Vector A, B;
	Vector* srcVec;
	Vector* dstVec;
	float totalDelta;
};