#pragma once


#include "GameObject/GameObject.h"

class OrbitOtherObject : public GameObject
{
public:
	OrbitOtherObject(GameWorld& world, const GameObject& obj, const Vector4& axis);

	virtual void Update(float tick) override;

private:
	Vector4 axis;
	Vector4 prevObjectPosition;
	const GameObject& object;
};