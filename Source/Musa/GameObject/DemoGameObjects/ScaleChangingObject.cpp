#include "ScaleChangingObject.hpp"
#include "Math/MathFunctions.hpp"

ScaleChangingObject::ScaleChangingObject(GameWorld& world, float scaleA, float scaleB)
	: GameObject(world),
	A(scaleA, scaleA, scaleA),
	B(scaleB, scaleB, scaleB),
	srcVec(&A),
	dstVec(&B)
{
	scale = A;
}

void ScaleChangingObject::Update(float tick)
{
	if (totalDelta >= 1.f)
	{
		srcVec = srcVec == &A ? &B : &A;
		dstVec = dstVec == &B ? &A : &B;
		totalDelta = 0.f;
	}

	scale = Math::Lerp(*srcVec, *dstVec, totalDelta);

	totalDelta += .001f * tick;

	GameObject::Update(tick);
}
