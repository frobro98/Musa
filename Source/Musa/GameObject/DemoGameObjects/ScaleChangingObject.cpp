#include "ScaleChangingObject.hpp"
#include "Math/MathFunctions.hpp"

ScaleChangingObject::ScaleChangingObject(GameWorld& world, float32 scaleA, float32 scaleB, float32 delta_)
	: GameObject(world),
	A(scaleA, scaleA, scaleA),
	B(scaleB, scaleB, scaleB),
	srcVec(&A),
	dstVec(&B),
	delta(delta_)
{
	scale = A;
}

void ScaleChangingObject::Update(float32 tick)
{
	if (totalDelta >= 1.f)
	{
		srcVec = srcVec == &A ? &B : &A;
		dstVec = dstVec == &B ? &A : &B;
		totalDelta = 0.f;
	}

	scale = Math::Lerp(*srcVec, *dstVec, totalDelta);

	totalDelta += tick;

	GameObject::Update(tick);
}
