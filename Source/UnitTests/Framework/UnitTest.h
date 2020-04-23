//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 
// UnitTest v.2.1.0
//          v.2.1.1  - fixed C5038 warning
//----------------------------------------------------------------------------- 

#ifndef CPP_UNIT_H
#define CPP_UNIT_H

#include "Platform.h"
#include "Assertion.h"
#include "DebugOutput.h"
#include "Containers/DynamicArray.hpp"
#include "Utilities/MacroHelpers.hpp"
#include "Math/MathFunctions.hpp"

//---------------------------------------------------------------------------------------
// Utility class - for testing floats
//
// Needs <cmath> - fabs()
//
//---------------------------------------------------------------------------------------
#include <cmath>

class UnitUtility
{
public:
	static bool AreEqual(float a, float b, float epsilon = 0.001f)
	{
		return (fabs(a - b) < epsilon);
	}

	static bool AreEqual(double a, double b, double epsilon = 0.001f)
	{
		return (fabs(a - b) < epsilon);
	}
};

//---------------------------------------------------------------------------------------
// Unit Tests
//
// Needs this header
//
//---------------------------------------------------------------------------------------
struct UnitStats
{
	UnitStats()
		:testCount(0),
		testPass(0),
		testFail(0), 
		indvAsserts(0)
	{
	}

	// Big four
	UnitStats(const UnitStats &) = default;
	UnitStats & operator = (const UnitStats &) = default;
	~UnitStats() = default;

	// data: ------------------
	int testCount;
	int testPass;
	int testFail;
	int indvAsserts;
};
struct UnitData
{
	const char *pMemberName = nullptr;
	char *pSourceFilePath = nullptr;
	int sourceLineNumber = 0;
	bool result = false;
};

class Test
{
public:
	Test(const char* pTestName);

	virtual void run(UnitData &, UnitStats &) const = 0;
	virtual void teardown() const {};

public:
	const char * pName;
	Test *testFunc;
};

class TestRegistry
{
public:
	// Big four
	TestRegistry(const TestRegistry &) = delete;
	TestRegistry & operator = (const TestRegistry &) = delete;


	static void AddTest(Test& test)
	{
		TestRegistry *pRegistry = TestRegistry::privGetInstance();

		pRegistry->tests.Add(&test);
	}
	static void RunTests()
	{
		//UnitTrace::out("\nTestRegistry:RunTests()\n");
		Debug::Print("\n");
		Debug::Print("---- Testing ----\n");
		TestRegistry *pRegistry = TestRegistry::privGetInstance();

		UnitData unitData;
		UnitStats unitStats;

		for(const auto& test : pRegistry->tests)
		{
			unitStats.testCount++;

			// run the test
			unitData.result = true;
			test->run(unitData, unitStats);
		
			if (unitData.result == true)
			{
				unitStats.testPass++;
			}
			else
			{
				unitStats.testFail++;
			}
		}

		Debug::Print("\n");
		Debug::Printf(" testCount: {}\n", unitStats.testCount);
		Debug::Printf("  testPass: {}\n", unitStats.testPass);
		Debug::Printf("  testFail: {}\n", unitStats.testFail);
		Debug::Printf("indvChecks: {}\n", unitStats.indvAsserts);
		Debug::Print("\n");
		Debug::Print("-----------------\n");
	}

private:
	TestRegistry() = default;

	static TestRegistry* privGetInstance()
	{
		static TestRegistry tRegistry;
		return &tRegistry;
	}

	DynamicArray<Test*> tests;
};

// Allow conditional expressions that are constant.
#pragma warning( disable : 4127 )

// Increments total check count
// Creates a clickable format in the output window for failure
// Abort test on first fail
#define CHECK( condition ) \
{ \
	_UnitStats.indvAsserts++;\
	if( !( condition ) ) \
	{ \
		_UnitData.result = false;  \
		_UnitData.pMemberName = this->pName; \
		_UnitData.pSourceFilePath = __FILE__; \
		_UnitData.sourceLineNumber = __LINE__; \
		Debug::Printf("{}({}): {} \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); \
		this->teardown(); \
		return; \
	} \
	else\
	{\
	}\
}

#define CHECK_TRUE(value) CHECK(value == true);
#define CHECK_FALSE(value) CHECK(value == false);
#define CHECK_ZERO(value) CHECK(Math::IsZero(value)
#define CHECK_EQ( value1, value2 ) CHECK(Math::IsEqual(value1, value2))


#define TEST(TestName, GroupName)													\
class TestName##GroupName##_Test : public Test										\
{																					\
	public:																			\
		TestName##GroupName##_Test():												\
		Test(STRING(TestName##GroupName##_Test))									\
		{																			\
		};																			\
																					\
	virtual void run(UnitData &_UnitData, UnitStats &_UnitStats) const override;	\
} TestName##GroupName##_instance;													\
																					\
void TestName##GroupName##_Test::run(UnitData& _UnitData, UnitStats& _UnitStats) const


#define TEST_WITH_TEARDOWN(TestName, GroupName)												\
class TestName##GroupName##_Test : public Test												\
{																							\
	public:																					\
		TestName##GroupName##_Test():														\
		Test(STRING(TestName##GroupName##_Test))											\
		{																					\
		};																					\
																							\
	virtual void run(UnitData &_UnitData, UnitStats &_UnitStats) const override;			\
	virtual void teardown() override;														\
} TestName##GroupName##_instance;															\
																							\
void TestName##GroupName##_Test::run(UnitData& _UnitData, UnitStats& _UnitStats) const 



#define TEST_TEARDOWN(TestName, GroupName)	\
void TestName##GroupName##_Test::teardown() const


#endif

// ---  End of File ---------------
