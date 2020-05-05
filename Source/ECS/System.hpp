#pragma once

#include "Types/Intrinsics.hpp"
#include "Assertion.h"
#include "ECS/DLLDef.h"
#include "ECS/QueryDescription.hpp"
#include "ECS/Query.hpp"

namespace Musa
{
struct World;

class ECS_API System
{
public:
	System(World& w);
	~System() = default;

	virtual void Initialize() {}
	virtual void Deinitialize() {}

	virtual void Update() = 0;
	virtual void CleanupComponents() {}

	forceinline World& GetWorld()
	{
		Assert(world);
		return *world;
	}

	NODISCARD Query GetQueryFor(const QueryDescription& qb);
	template <typename... CompTypes>
	NODISCARD Query GetQueryFor();

protected:
// 	QueryDescription BeginQuery();
// 	NODISCARD Query EndQuery(const QueryDescription& qb);

private:
	// TODO - This is good to have in all systems, but if you want to access entity/component details, you probably don't want to load 
	// all of the world object into memory. Could be a lot to load. Need to split up some behaviors in the world into other structures
	World* world = nullptr;
};

template<typename ...CompTypes>
inline Query System::GetQueryFor()
{
	QueryDescription queryDesc;
	queryDesc.Require<CompTypes...>();
	return GetQueryFor(queryDesc);
}
}
