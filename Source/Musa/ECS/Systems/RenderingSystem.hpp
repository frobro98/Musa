// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/System.hpp"

// After Culling System...
class RenderingSystem : public Musa::System
{
public:
	
	virtual void Initialize() override;
	virtual void Update() override;

private:
	Musa::Query* renderQuery = nullptr;
};
