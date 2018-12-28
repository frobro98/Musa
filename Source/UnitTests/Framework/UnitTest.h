//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 
// UnitTest v.2.1.0
//          v.2.1.1  - fixed C5038 warning
//----------------------------------------------------------------------------- 

#ifndef CPP_UNIT_H
#define CPP_UNIT_H

#include "EngineCore/Platform.h"

//---------------------------------------------------------------------------------------
// UnitTrace - output to debug window
//             isolated from other systems
//
// NOTE: You need to set your project settings
//       Character Set -> Use Multi-Byte Character Set
//
//---------------------------------------------------------------------------------------

// Windows.h include
// many warnings - need to wrap for Wall warnings
	#pragma warning( push )
	#pragma warning( disable : 4820 )
	#pragma warning( disable : 4668 )
	#include <Windows.h>
	#pragma warning( pop ) 


#define UnitTraceBuffSize 256

// Singleton helper class
class UnitTrace
{
public:
	// displays a printf to the output window
	static void out(char* fmt, ...)
	{
		UnitTrace *pTrace = UnitTrace::privGetInstance();
		assert(pTrace);

		va_list args;
		va_start(args, fmt);

		vsprintf_s(pTrace->privBuff, UnitTraceBuffSize, fmt, args);
		OutputDebugString(pTrace->privBuff);

		va_end(args);
	}

	// Big four
	UnitTrace() = default;
	UnitTrace(const UnitTrace &) = default;
	UnitTrace & operator = (const UnitTrace &) = default;
	~UnitTrace() = default;

private:
	static UnitTrace *privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static UnitTrace helper;
		return &helper;
	}
	char privBuff[UnitTraceBuffSize];
};

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
	UnitData()
		: pMemberName(0),
		pSourceFilePath(0),
		sourceLineNumber(0),
		result(false),
		pad0(0),
		pad1(0),
		pad2(0)
	{
	}

	// Big four
	UnitData(const UnitData &) = default;
	UnitData & operator = (const UnitData &) = default;
	~UnitData() = default;

	// data: -----------------
	const char *pMemberName;
	char *pSourceFilePath;
	int sourceLineNumber;
	bool result;
	char pad0;
	char pad1;
	char pad2;
};

class UnitSLink
{
public:
	UnitSLink()
	{
		this->Clear();
	}
	// Big four
	UnitSLink(const UnitSLink &) = default;
	UnitSLink & operator = (const UnitSLink &) = default;
	~UnitSLink() = default;

	void Clear()
	{
		this->pNext = 0;
	}

	static void AddToFront(UnitSLink *&pRoot, UnitSLink *pNode )
	{
		if (pRoot == 0)
		{		
			pRoot = pNode;
			assert(pNode->pNext == 0);
		}
		else
		{
			UnitSLink *pTmp = pRoot;
			pRoot = pNode;
			pNode->pNext = pTmp;
		}
	}

public:
	// Data
	UnitSLink *pNext;
};

class Test : public UnitSLink
{
public:
	Test(const char * pTestName);

	// Big four
	Test() = delete;
	Test(const Test &) = default;
	Test & operator = (const Test &) = default;
	~Test() = default;


	virtual void run(UnitData &, UnitStats &) = 0;
	virtual void teardown() {};

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


	static void AddTest(UnitSLink *pTest)
	{
		//UnitTrace::out("TestRegistry:AddTest(%s) \n", pTest->pName);
		assert(pTest != 0);

		TestRegistry *pRegistry = TestRegistry::privGetInstance();

		// add to front
		UnitSLink::AddToFront((UnitSLink *&)pRegistry->pRoot, pTest);
	}
	static void RunTests()
	{
		//UnitTrace::out("\nTestRegistry:RunTests()\n");
		UnitTrace::out("\n");
		UnitTrace::out("---- Testing ----\n");
		TestRegistry *pRegistry = TestRegistry::privGetInstance();
		UnitSLink *pTmp = (UnitSLink *)pRegistry->pRoot;

		while (pTmp != 0)
		{
			pRegistry->_UnitStats.testCount++;

			// downcast to the test
			Test *pTest = (Test *)pTmp;

			//UnitTrace::out("\n%s - start\n", pTest->pName);

			// run the test
			pRegistry->_UnitData.result = true;
			
			pTest->testFunc->run(pRegistry->_UnitData, pRegistry->_UnitStats);
		
			

			if (pRegistry->_UnitData.result == true)
			{
				pRegistry->_UnitStats.testPass++;
			}
			else
			{
				pRegistry->_UnitStats.testFail++;
			}

			//UnitTrace::out("%s - end\n", pTest->pName);

			// next test
			pTmp = pTmp->pNext;
		}

		UnitTrace::out("\n");
		UnitTrace::out(" testCount: %d\n", pRegistry->_UnitStats.testCount);
		UnitTrace::out("  testPass: %d\n", pRegistry->_UnitStats.testPass);
		UnitTrace::out("  testFail: %d\n", pRegistry->_UnitStats.testFail);
		UnitTrace::out("indvChecks: %d\n", pRegistry->_UnitStats.indvAsserts);
		UnitTrace::out("\n");
		UnitTrace::out("-----------------\n");
	}

private:
	TestRegistry() = default;

	static TestRegistry* privGetInstance()
	{
		static TestRegistry tRegistry;
		return &tRegistry;
	}

// Data: ------------------------
	UnitData _UnitData;
	UnitStats _UnitStats;
	UnitSLink *pRoot = nullptr;
};


// a trick to create a c-string
#define STRING_ME(s) #s

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
		UnitTrace::out("%s(%d): %s \n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName ); \
		this->teardown(); \
		return; \
	} \
	else\
	{\
	}\
}


#define CHECK_EQUAL( value1, value2 ) \
{ \
	_UnitStats.indvAsserts++;\
	if( !( value1 == value2 ) ) \
	{ \
		_UnitData.result = false;  \
		_UnitData.pMemberName = this->pName; \
		_UnitData.pSourceFilePath = __FILE__; \
		_UnitData.sourceLineNumber = __LINE__; \
		UnitTrace::out("%s(%d): %s\n", _UnitData.pSourceFilePath, _UnitData.sourceLineNumber, _UnitData.pMemberName  ); \
		return; \
	} \
	else\
	{\
	}\
}

// Here is the magic
//
// 1) it creates a class, <TestName>_Test
//    it is derived from Test class
// 2) it defines the constructor and calls the base constructor with the name
//    it registers the name of the test to a single class (TestRegistry)
// 3) It also overloads the Test::run() method with the body of the macro

#define TEST( TestName, GroupName ) \
class TestName##GroupName##_Test : public Test \
{ \
	public: \
		TestName##GroupName##_Test(): \
		Test( STRING_ME(TestName##GroupName##_Test)) \
		{ \
		}; \
	\
	void run(UnitData &_UnitData, UnitStats &_UnitStats);\
} TestName##GroupName##_instance; \
\
void TestName##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) 


#define TEST_WITH_TEARDOWN( TestName, GroupName ) \
class TestName##GroupName##_Test : public Test \
{ \
	public: \
		TestName##GroupName##_Test(): \
		Test( STRING_ME(TestName##GroupName##_Test)) \
		{ \
		}; \
	\
	void run(UnitData &_UnitData, UnitStats &_UnitStats);\
	virtual void teardown();\
} TestName##GroupName##_instance; \
\
void TestName##GroupName##_Test::run( UnitData &_UnitData, UnitStats &_UnitStats ) 



#define TEST_TEARDOWN( TestName, GroupName ) \
void TestName##GroupName##_Test::teardown() 


#endif

// ---  End of File ---------------
