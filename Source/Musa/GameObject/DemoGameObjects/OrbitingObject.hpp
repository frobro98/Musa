#pragma once

#include "Math/Vector4.hpp"
#include "GameObject/GameObject.h"


class OrbitingObject : public GameObject
{
public:
	OrbitingObject(GameWorld& world, const Vector4& orbitAxis, const Vector4& orbitPos);

	virtual void Update(float tick) override;

private:
	Vector4 axis;
	Vector4 orbitLocation;
};