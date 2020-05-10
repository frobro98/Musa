
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
	Query& q0 = GetQueryFor<Position>();

	// Need to get all entities from Query object
	ArrayView<Entity> entities = GetQueriedEntities(q0);
	CHECK_EQ(entities.Size(), numEntitiesExpected);

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
	CHECK_ZERO(w.systems.Size());

}

