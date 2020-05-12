
#include "ChunkComponentAccessor.hpp"

namespace Musa
{
ChunkComponentAccessor::ChunkComponentAccessor(ArchetypeChunk& c)
	: chunk(c)
{
}
ArrayView<const ComponentType*> ChunkComponentAccessor::GetChunkTypes() const
{
	return ArrayView<const ComponentType*>(
		chunk.header->types,
		chunk.header->componentTypeCount
	);
}
}