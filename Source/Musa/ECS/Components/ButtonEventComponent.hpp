// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/Component.hpp"
#include "Input/InputEvents.hpp"

struct ButtonEventComponent : Musa::Component
{
	ButtonEvent event;
};
