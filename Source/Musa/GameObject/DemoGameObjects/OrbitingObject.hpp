#pragma once

#include "Math/Vector.h"
#include "GameObject/GameObject.h"


class OrbitingObject : public GameObject
{
public:
	OrbitingObject(const Vector& orbitAxis, const Vector& orbitPos);

	virtual void Update(float tick) override;

private:
	Vector axis;
	Vector orbitLocation;
};