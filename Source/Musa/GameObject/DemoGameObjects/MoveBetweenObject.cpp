
#include "MoveBetweenObject.hpp"
#include "Math/VectorFunctions.h"

MoveBetweenObject::MoveBetweenObject(const Vector& pointA, const Vector& pointB, float startingDelta)
	: A(pointA), 
	B(pointB),
	srcVec(&A),
	dstVec(&B),
	totalDelta(startingDelta)
{
	Math::Lerp(position, *srcVec, *dstVec, totalDelta);
}

void MoveBetweenObject::Update(float tick)
{
	if (totalDelta >= 1.f)
	{
		srcVec = srcVec == &A ? &B : &A;
		dstVec = dstVec == &B ? &A : &B;
		totalDelta = 0.f;
	}

	Math::Lerp(position, *srcVec, *dstVec, totalDelta);

	totalDelta += .003f;

	GameObject::Update(tick);
}
