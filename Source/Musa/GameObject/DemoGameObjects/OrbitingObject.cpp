#include "OrbitingObject.hpp"
#include "Math/Matrix.h"
#include "Math/Quat.h"

OrbitingObject::OrbitingObject(const Vector& orbitAxis, const Vector& orbitPos)
	: axis(orbitAxis),
	orbitLocation(orbitPos)
{
}

void OrbitingObject::Update(float tick)
{
	constexpr float angleOffsetDeg = .005f;
	Matrix trans(TRANS, orbitLocation);
	Quat rot(ROT_AXIS_ANGLE, axis, angleOffsetDeg * tick);
	Matrix negTrans(TRANS, -orbitLocation);

	Matrix orbitTrans = negTrans * rot * trans;

	position *= orbitTrans;

	GameObject::Update(tick);
}
