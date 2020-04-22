
#include "Archetype.hpp"
#include "ComponentType.hpp"
#include "Entity.hpp"

namespace Musa
{
ComponentTypeOffsetList ConstructOffsetList(const ComponentType** compTypes, size_t count)
{
	uint32 componentSetSize = sizeof(Entity);
	for (size_t i = 0; i < count; ++i)
	{
		componentSetSize += compTypes[i]->size;
	}


	constexpr uint32 usableSpace = Musa::UsableChunkSize;
	// slack just pads chunk for safety.
	// TODO - Investigate if this is actually necessary...
	constexpr uint32 slack = 2;
	const uint32 archetypeEntityCap = usableSpace / componentSetSize - slack;

	DynamicArray<ComponentTypeOffset> offsetList;
	offsetList.Reserve(archetypeEntityCap);
	size_t offset = sizeof(Entity) * archetypeEntityCap;
	for (size_t i = 0; i < count; ++i)
	{
		const ComponentType* type = compTypes[i];
		Assert(type->alignment != 0);

		size_t remainder = offset % type->alignment;
		offset += type->alignment - remainder;
		offsetList.Add(ComponentTypeOffset{ type, type->typenameHash, type->archetypeBit, offset });

		offset += type->size * archetypeEntityCap;
	}

	Assert(offset <= Musa::ArchetypeBlockSize);

	return ComponentTypeOffsetList{
		std::move(offsetList),
		archetypeEntityCap
	};
}
}
