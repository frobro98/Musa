
#include "UnitTest.h"

UnitTest::UnitTest(const char* testName)
{
	// initialized data
	name = testName;

	UnitTestRegistry::AddTest(*this);
}
