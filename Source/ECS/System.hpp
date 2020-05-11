#pragma once

#include "Types/Intrinsics.hpp"
#include "Assertion.h"
#include "ECS/DLLDef.h"
#include "ECS/QueryDescription.hpp"
#include "ECS/Query.hpp"
// TODO - There should be another type that is user facing...
#include "ECS/ArchetypeChunk.hpp"

namespace Musa
{
struct World;
class QueryCache;

class ECS_API System
{
public:
	virtual ~System() = default;

	void InitializeInternals(World& w);

	virtual void Initialize() {}
	virtual void Deinitialize() {}

	virtual void Update() = 0;
	virtual void CleanupComponents() {}

	forceinline World& GetWorld()
	{
		Assert(world);
		return *world;
	}

	NODISCARD Query& GetQueryFor(const QueryDescription& qb);
	template <typename... CompTypes>
	NODISCARD Query& GetQueryFor();
	NODISCARD forceinline QueryDescription DescribeQuery()
	{
		return QueryDescription{};
	}

	NODISCARD DynamicArray<ArchetypeChunk> GetQueryChunks(const Query& query);

protected:
// 	QueryDescription BeginQuery();
// 	NODISCARD Query EndQuery(const QueryDescription& qb);

private:
	// TODO - This is good to have in all systems, but if you want to access entity/component details, you probably don't want to load 
	// all of the world object into memory. Could be a lot to load. Need to split up some behaviors in the world into other structures
	World* world = nullptr;
	QueryCache* queryCache = nullptr;
};

template<typename ...CompTypes>
inline Query& System::GetQueryFor()
{
	QueryDescription queryDesc;
	queryDesc.Require<CompTypes...>();
	return GetQueryFor(queryDesc);
}
}
