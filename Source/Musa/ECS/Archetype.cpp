
#include "Archetype.hpp"
#include "Entity.hpp"
#include "ComponentType.hpp"

namespace Musa
{
static constexpr ArchetypeHashID BuildArchetypeHash(const ComponentType** types, size_t typeCount)
{
	ArchetypeHashID hashId = 0;
	for (size_t i = 0; i < typeCount; ++i)
	{
		hashId |= types[i]->archetypeBit;
	}
	return hashId;
}

Archetype* CreateArchetypeFrom(const ComponentType** compTypes, size_t typeCount)
{
	constexpr uint32 MaxComponentsPerArchetype = 32;
	Assert(typeCount <= MaxComponentsPerArchetype);

	Archetype* archetype = new Archetype
	{
		DynamicArray<ArchetypeChunk*>(),
		ConstructOffsetList(compTypes, typeCount),
		BuildArchetypeHash(compTypes, typeCount),
		0
	};

	return archetype;
}
}
