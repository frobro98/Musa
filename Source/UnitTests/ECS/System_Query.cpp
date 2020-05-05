
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

class QuerySystem : public System
{
public:
	QuerySystem(World& w, uint32 expectedEntityCount, const char* n, UnitData& data, UnitStats& stats)
		: System(w), name(n), _UnitData(data), _UnitStats(stats), numEntitiesExpected(expectedEntityCount)
	{}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Deinitialize() override;

	bool UnitTestUpdate()
	{
		Update();
		return updatedSuccess;
	}

	// This is here just to allow the UnitTestSystem be hooked into the unit test framework
	void teardown() {};

private:
	const char* name;
	UnitData& _UnitData;
	UnitStats& _UnitStats;
	uint32 numEntitiesExpected;
};

void QuerySystem::Initialize()
{
	updatedSuccess = false;
}

void QuerySystem::Update()
{
	World& w = GetWorld();
	CHECK_REF(w);

	Query q = GetQueryFor<Position>();
	uint32 entities = 0;
	for (auto& archetype : q.queriedArchetypes)
	{
		CHECK_PTR(archetype);
		CHECK_EQ(archetype->chunks.Size(), 1);
		entities += archetype->chunks[0]->footer.numEntities;
	}
	CHECK_EQ(entities, numEntitiesExpected);

	updatedSuccess = true;
}

void QuerySystem::Deinitialize()
{
	updatedSuccess = false;
}

TEST(SystemQueryOneEntity, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();

	constexpr uint32 numEntitiesExpected = 1;
	QuerySystem& s = w.CreateSystem<QuerySystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QuerySystem>());

	s.Initialize();
	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
	s.Deinitialize();
	CHECK_FALSE(updatedSuccess);
}

TEST(SystemQueryTwoEntitiesDiffArchetype, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();
	e = w.CreateEntity<Position, Rotation>();

	constexpr uint32 numEntitiesExpected = 2;
	QuerySystem& s = w.CreateSystem<QuerySystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QuerySystem>());

	s.Initialize();
	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
	s.Deinitialize();
	CHECK_FALSE(updatedSuccess);
}

TEST(SystemQueryThreeEntitiesDiffArchetype, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();
	e = w.CreateEntity<Position, Rotation>();
	e = w.CreateEntity<Position, Movement>();

	constexpr uint32 numEntitiesExpected = 3;
	QuerySystem& s = w.CreateSystem<QuerySystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QuerySystem>());

	s.Initialize();
	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
	s.Deinitialize();
	CHECK_FALSE(updatedSuccess);
}

TEST(SystemQueryThreeEntitiesTwoMatching, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();
	e = w.CreateEntity<Position, Rotation>();
	e = w.CreateEntity<Movement>();

	constexpr uint32 numEntitiesExpected = 2;
	QuerySystem& s = w.CreateSystem<QuerySystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QuerySystem>());

	s.Initialize();
	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
	s.Deinitialize();
	CHECK_FALSE(updatedSuccess);
}
