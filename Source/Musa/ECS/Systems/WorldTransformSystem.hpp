// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/System.hpp"

namespace Musa
{
struct Query;
}

class WorldTransformSystem : public Musa::System
{
public:
	virtual void Initialize() override;

	virtual void Update() override;

private:
	void UpdateEntitiesWithNoTransform();
	void UpdateEntitiesWithTransform();

private:
	Musa::Query* noWorldEntities;
	Musa::Query* worldEntities;
};
