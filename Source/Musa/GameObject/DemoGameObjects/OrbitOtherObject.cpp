#include "OrbitOtherObject.hpp"
#include "Math/Quat.hpp"

OrbitOtherObject::OrbitOtherObject(GameWorld& world, const GameObject& obj, const Vector4& orbitAxis)
	: GameObject(world),
	axis(orbitAxis),
	object(obj)
{
	prevObjectPosition = object.GetPosition();
}

void OrbitOtherObject::Update(float tick)
{
	constexpr float angleOffsetDeg = .005f;

	Vector4 objectPos = object.GetPosition();
	Vector4 dir = objectPos - prevObjectPosition;

	Matrix4 trans(TRANS, objectPos);
	Quat rot(ROT_AXIS_ANGLE, axis, angleOffsetDeg * tick);
	Matrix4 negTrans(TRANS, -objectPos);

	Matrix4 orbitTrans = negTrans * rot * trans;

	position += dir;
	position *= orbitTrans;

	GameObject::Update(tick);

	prevObjectPosition = objectPos;
}
