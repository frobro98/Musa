//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/MathEngine.h"

#define eq	Math::IsEqual 

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(SampleTest, vector_tests)
{
	float x(5.0f);

	CHECK(x == 5.0f);
}

TEST(ConstantCheck, vector_tests)
{
	CHECK(Math::IsEqual(Math::PiOver2, 1.570796f, Math::InternalTolerence) == true);
}

// ---  End of File ---------------
