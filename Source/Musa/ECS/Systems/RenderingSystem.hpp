// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/System.hpp"

// After Culling System...
// Move mesh and material data from the MeshRenderComponent over to the RenderDataComponent.
// This also 
class RenderingSystem : public Musa::System
{
public:
	virtual void Initialize() override;
	virtual void Update() override;

private:
	void AddMissingRenderData();
	void UpdateRenderData();

private:
	Musa::Query* noRenderDataQuery = nullptr;
	Musa::Query* updateRenderDataQuery = nullptr;
};
