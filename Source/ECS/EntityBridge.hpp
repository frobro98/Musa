#pragma once

#include "ECS/DLLDef.h"
#include "ECS/ArchetypeChunk.hpp"

namespace Musa
{

struct Archetype;
struct ArchetypeChunk;

struct EntityBridge
{
	ArchetypeChunk chunk;
	uint32 version;
	uint32 chunkIndex;
};

ECS_API Entity CreateEntityWith(Archetype& archetype);
}

