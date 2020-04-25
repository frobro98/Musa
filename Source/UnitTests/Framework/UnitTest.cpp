
#include "UnitTest.h"

Test::Test(const char* testName)
{
	// initialized data
	name = testName;

	TestRegistry::AddTest(*this);
}
