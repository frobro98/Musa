
#include "Framework/UnitTest.h"
#include "ECS/Archetype.hpp"
#include "ECS/World.hpp"
#include "ECS/System.hpp"

#include "TestComponents/FloatArray.hpp"
#include "TestComponents/Movement.hpp"
#include "TestComponents/Position.hpp"
#include "TestComponents/Rotation.hpp"

using namespace Musa;

class UnitTestSystem : public System
{
public:
	UnitTestSystem(World& w, const char* n, UnitData& data, UnitStats& stats)
		: System(w), name(n), _UnitData(data), _UnitStats(stats)
	{}

	virtual void Update() override;

	// This is here just to allow the UnitTestSystem be hooked into the unit test framework
	void teardown() {};

private:
	const char* name;
	UnitData& _UnitData;
	UnitStats& _UnitStats;
};

void UnitTestSystem::Update()
{
	World& w = GetWorld();
	CHECK_REF(w);
}

TEST(SystemCreateOne, SystemCreate)
{
 	World w;
 	CHECK_ZERO(w.systems.Size());
 
 	System& s = w.CreateSystem<UnitTestSystem>(name, _UnitData, _UnitStats);
	CHECK_REF(s);
//  	CHECK_EQ(w.systems.Size(), 1);
//  	CHECK_EQ(w.systemTypesInWorld.Size(), 1);
//  	CHECK_EQ(w.systemTypesInWorld[0], GetSystemTypeFor<UnitTestSystem>());
// 
// 	s.Initialize();
// 	s.Update();
// 	s.Deinitialize();
// 
// 	w.DestroySystem<UnitTestSystem>();
// 	CHECK_EQ(w.systems.Size(), 0);
// 	CHECK_EQ(w.systemTypesInWorld.Size(), 0);
}