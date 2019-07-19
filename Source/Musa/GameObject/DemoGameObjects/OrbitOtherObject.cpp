#include "OrbitOtherObject.hpp"

OrbitOtherObject::OrbitOtherObject(const GameObject& obj, const Vector& orbitAxis)
	: axis(orbitAxis),
	object(obj)
{
	prevObjectPosition = object.GetPosition();
}

void OrbitOtherObject::Update(float tick)
{
	constexpr float angleOffsetDeg = .005f;

	Vector objectPos = object.GetPosition();
	Vector dir = objectPos - prevObjectPosition;

	Matrix trans(TRANS, objectPos);
	Quat rot(ROT_AXIS_ANGLE, axis, angleOffsetDeg * tick);
	Matrix negTrans(TRANS, -objectPos);

	Matrix orbitTrans = negTrans * rot * trans;

	position += dir;
	position *= orbitTrans;

	GameObject::Update(tick);

	prevObjectPosition = objectPos;
}
