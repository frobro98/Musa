// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/System.hpp"
#include "ECS/Component.hpp"

// This system exists to determine, based on a given frustum bounds
// How the frustum bounds are actually found would be 2 different queries:
// One query being used to locate culling bounds within entities, and the other
// being all entities that would be existing in the world at a position. If
// an object needs to be culled, it gets tagged with a "Culled" component. The
// systems that require only non-culled entities will just filter by the "Culled"
// component.
//
// This system poses a potential issue when it comes to performance. If an entity
// is culled, it will be tagged accordingly. Because of this tagging, the entity
// data store will be copied into a different archetype. After the presentation,
// This component needs to be removed from all entities within the world. This,
// in theory, could be a high volume of entities being culled from the world.
//
// Alternatives to this option, because of the potential performance cost, would
// be checking every entity in the world to see if it needs to be culled. This
// way, an entity that is culled would not be copied between archetypes constantly.
// This would only work if all entities are tested, filtering by both tagged and untagged
// component queries. 

struct CulledTag : Musa::Component
{
};

class CullingSystem : public Musa::System
{
public:
	virtual void Initialize() override;
	virtual void Update() override;

private:
	Musa::Query* culledQuery = nullptr;
	Musa::Query* shownQuery = nullptr;
	Musa::Query* frustumBoundsQuery = nullptr;
};
