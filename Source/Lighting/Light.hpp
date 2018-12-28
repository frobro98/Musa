#pragma once

#include "GameObject/GameObject.h"


template <typename LightInfo>
class Light : public GameObject
{
public:
	Light() = default;

	virtual LightInfo GetLightInfo() = 0;
};