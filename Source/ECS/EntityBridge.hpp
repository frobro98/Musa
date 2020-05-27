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
	ArchetypeChunk chunk;
	u32 version;
	u32 chunkIndex;
};

ECS_API Entity CreateEntityWith(Archetype& archetype);
}

