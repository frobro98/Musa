// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Component.hpp"

struct RenderComponent : public Musa::Component
{
	//DECLARE_COMPONENT(RenderComponent);


	u32 pad[2] = { 0, 0 };
};
