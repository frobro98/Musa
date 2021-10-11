// Copyright 2020, Nathan Blane

#pragma once

#include "ECS/ECSAPI.hpp"
#include "ECS/ArchetypeChunk.hpp"

namespace Musa
{

struct Archetype;
struct ArchetypeChunk;

struct EntityBridge
{
	// Owning chunk
	ArchetypeChunk chunk;
	// Current version of entity that lives in the bridge
	u32 version;
	// Index into the owning chunk
	u32 chunkIndex;
};

ECS_API Entity CreateEntityWith(Archetype& archetype);
}

