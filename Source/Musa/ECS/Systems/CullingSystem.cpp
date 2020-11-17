// Copyright 2020, Nathan Blane

#include "CullingSystem.hpp"
#include "ECS/Components/CulledTag.hpp"

void CullingSystem::Initialize()
{
	culledQuery = &GetQueryFor(
		DescribeQuery()
		.Require<CulledTag>()
		//.Require<SomeKindOfTransformComp>()
	);

	shownQuery = &GetQueryFor(
		DescribeQuery()
		//.Require<SomeKindOfTransformComp>()
		.None<CulledTag>()
	);

	frustumBoundsQuery = &GetQueryFor(
		DescribeQuery()
		//.Require<ViewBounds>()
	);
}

void CullingSystem::Update()
{
	// Get all entities containing frustum view bounds

	// Check if CulledTag entities need to be updated

	// Check if shown entities need to be culled
}
