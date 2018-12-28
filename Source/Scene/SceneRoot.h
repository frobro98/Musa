#pragma once

#include "GameObject.h"

// TODO - This basically becomes a way to get at the "World" possibly
// The other option would just be actually setting the world when the GameObjectManager creates them

class SceneRoot : public GameObject
{
public:
	virtual void Update(float /*tick*/) override {}
};