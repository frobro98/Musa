
#include "Framework/UnitTest.h"
#include "ECS/Archetype.hpp"
#include "ECS/World.hpp"
#include "ECS/System.hpp"

#include "TestComponents/FloatArray.hpp"
#include "TestComponents/Movement.hpp"
#include "TestComponents/Position.hpp"
#include "TestComponents/Rotation.hpp"

using namespace Musa;

static bool updatedSuccess = true;

//////////////////////////////////////////////////////////////////////////
// Iterate Entity System
//////////////////////////////////////////////////////////////////////////

class IterateChunkSystem : public System
{
public:
	IterateChunkSystem(uint32 expectedEntityCount, const char* n, UnitData& data, UnitStats& stats)
		: name(n), _UnitData(data), _UnitStats(stats), numEntitiesExpected(expectedEntityCount)
	{}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Deinitialize() override;

	// This is here just to allow the UnitTestSystem be hooked into the unit test framework
	void teardown() {};

private:
	const char* name;
	UnitData& _UnitData;
	UnitStats& _UnitStats;
	uint32 numEntitiesExpected;
};

void IterateChunkSystem::Initialize()
{
	updatedSuccess = false;
}

void IterateChunkSystem::Update()
{
	World& w = GetWorld();
	CHECK_REF(w);

	// Query gets all of the archetypes that match types asked for
	QueryDescription desc = DescribeQuery().OneOrMore<Position, Rotation>();
	Query& q0 = GetQueryFor(desc);

	// Need to get all entities from Query object
	ArrayView<Entity> entities = GetQueriedEntities(q0);
	CHECK_EQ(entities.Size(), numEntitiesExpected);

	DynamicArray<ChunkComponentAccessor> chunks = GetQueryChunks(q0);
	CHECK_GT(chunks.Size(), 0);

	uint32 numEntitiesInChunks = 0;
	for (auto& chunk : chunks)
	{
		numEntitiesInChunks += chunk.GetEntityCount();
	}
	CHECK_EQ(numEntitiesInChunks, numEntitiesExpected);

	updatedSuccess = true;
}

void IterateChunkSystem::Deinitialize()
{
	updatedSuccess = false;
}

//////////////////////////////////////////////////////////////////////////
// Unit Tests
//////////////////////////////////////////////////////////////////////////

TEST(IterateQueriedChunks, SystemChunkBehaviors)
{
	updatedSuccess = true;

	World w;
	CHECK_REF(w);

	constexpr uint32 numEntitiesExpected = 50;
	for (uint32 i = 0; i < numEntitiesExpected / 2; ++i)
	{
		NOT_USED Entity e = w.CreateEntity<Position>();
	}

	for (uint32 i = 0; i < numEntitiesExpected / 2; ++i)
	{
		NOT_USED Entity e = w.CreateEntity<Rotation>();
	}

	IterateChunkSystem& s = w.CreateSystem<IterateChunkSystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<IterateChunkSystem>());

	CHECK_FALSE(updatedSuccess);
	w.Update();
	CHECK_TRUE(updatedSuccess);

}

