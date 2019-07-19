#pragma once

#include "Lighting/Light.hpp"

class PointLight : public Light
{
public:
	virtual LightDescription GetLightDescription() override;

	float32 attenuationRadius;


};