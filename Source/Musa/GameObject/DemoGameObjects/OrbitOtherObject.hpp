#pragma once


#include "GameObject/GameObject.h"

class OrbitOtherObject : public GameObject
{
public:
	OrbitOtherObject(const GameObject& obj, const Vector& axis);

	virtual void Update(float tick) override;

private:
	Vector axis;
	Vector prevObjectPosition;
	const GameObject& object;
};