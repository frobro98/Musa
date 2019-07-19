#pragma once

#include "ECS/Component.hpp"

struct RenderComponent : public Musa::Component
{
	DECLARE_COMPONENT(RenderComponent);

	RenderComponent();

	uint32 pad[2] = { 0, 0 };
};
