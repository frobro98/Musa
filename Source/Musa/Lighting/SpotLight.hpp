#pragma once

#include "Lighting/Light.hpp"

class SpotLight : public Light
{
public:

	SpotLight(GameWorld& world) : Light(world) {}

	virtual LightDescription GetLightDescription() override;

	float32 innerAngleDegrees;
	float32 outerAngleDegrees;
	float32 attenuationRadius;

private:
	// Eventually, these will be out of here and only calculated when needed. 
	float cosInnerAngle = 0;
	float cosOuterAngle = 0;
};
