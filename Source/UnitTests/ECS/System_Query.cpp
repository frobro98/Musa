// Copyright 2020, Nathan Blane

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
// Query System
//////////////////////////////////////////////////////////////////////////

class QuerySystem : public System
{
public:
	QuerySystem(uint32 expectedEntityCount, const char* n, UnitData& data, UnitStats& stats)
		: name(n), _UnitData(data), _UnitStats(stats), numEntitiesExpected(expectedEntityCount)
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

	Query& q0 = GetQueryFor<Position>();
	uint32 entities = 0;
	for (auto& archetype : q0.queriedArchetypes)
	{
		CHECK_PTR(archetype);
		CHECK_EQ(archetype->chunks.Size(), 1);
		entities += archetype->totalEntityCount;
	}
	CHECK_EQ(entities, numEntitiesExpected);

	Query& q1 = GetQueryFor<Position>();

	CHECK_EQ(&q0, &q1);

	updatedSuccess = true;
}

void QuerySystem::Deinitialize()
{
	updatedSuccess = false;
}

//////////////////////////////////////////////////////////////////////////
// Query Description System
//////////////////////////////////////////////////////////////////////////

class QueryDescSystem : public System
{
public:
	QueryDescSystem(uint32 expectedEntityCount, const char* n, UnitData& data, UnitStats& stats)
		: name(n), _UnitData(data), _UnitStats(stats), numEntitiesExpected(expectedEntityCount)
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

void QueryDescSystem::Initialize()
{
	updatedSuccess = false;
}

void QueryDescSystem::Update()
{
	World& w = GetWorld();
	CHECK_REF(w);

	uint32 entities = 0;
	Query& q = GetQueryFor(
		DescribeQuery()
		.Require<Position>());

	for (auto& archetype : q.queriedArchetypes)
	{
		CHECK_PTR(archetype);
		CHECK_EQ(archetype->chunks.Size(), 1);
		entities += archetype->totalEntityCount;
	}
	CHECK_EQ(entities, numEntitiesExpected);

	updatedSuccess = true;
}

void QueryDescSystem::Deinitialize()
{
	updatedSuccess = false;
}

//////////////////////////////////////////////////////////////////////////
// QueryWithNoneSystem
//////////////////////////////////////////////////////////////////////////

class QueryWithNoneSystem : public System
{
public:
	QueryWithNoneSystem(uint32 expectedEntityCount, const char* n, UnitData& data, UnitStats& stats)
		: name(n), _UnitData(data), _UnitStats(stats), numEntitiesExpected(expectedEntityCount)
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

void QueryWithNoneSystem::Initialize()
{
	updatedSuccess = false;
}

void QueryWithNoneSystem::Update()
{
	World& w = GetWorld();
	CHECK_REF(w);

	uint32 entities = 0;
	Query& q = GetQueryFor(
		DescribeQuery()
		.Require<Position>()
		.None<Movement>()
	);

	for (auto& archetype : q.queriedArchetypes)
	{
		CHECK_PTR(archetype);
		CHECK_EQ(archetype->chunks.Size(), 1);
		entities += archetype->totalEntityCount;
	}
	CHECK_EQ(entities, numEntitiesExpected);

	updatedSuccess = true;
}

void QueryWithNoneSystem::Deinitialize()
{
	updatedSuccess = false;
}

//////////////////////////////////////////////////////////////////////////
// Unit Tests
//////////////////////////////////////////////////////////////////////////

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

	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
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

	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
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

	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
}

TEST(SystemQueryUsingDescriptionThreeEntities, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();
	e = w.CreateEntity<Position, Rotation>();
	e = w.CreateEntity<Position, Movement>();

	constexpr uint32 numEntitiesExpected = 3;
	QueryDescSystem& s = w.CreateSystem<QueryDescSystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QueryDescSystem>());

	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
}

TEST(SystemQueryWithNoneQueryThreeEntities, SystemQuery)
{
	updatedSuccess = true;

	World w;
	CHECK_ZERO(w.systems.Size());

	Entity e = w.CreateEntity<Position>();
	e = w.CreateEntity<Position, Rotation>();
	e = w.CreateEntity<Position, Movement>();

	constexpr uint32 numEntitiesExpected = 2;
	QueryWithNoneSystem& s = w.CreateSystem<QueryWithNoneSystem>(numEntitiesExpected, name, _UnitData, _UnitStats);
	CHECK_REF(s);
	CHECK_EQ(w.systems.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<QueryWithNoneSystem>());

	CHECK_FALSE(updatedSuccess);
	CHECK_TRUE(s.UnitTestUpdate());
}
