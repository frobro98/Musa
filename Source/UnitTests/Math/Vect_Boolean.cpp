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

TEST(IsEqualFalse, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(9.0f, -3.0f, 5.0f);
	bool bvalue1;

	bvalue1 = v1.IsEqual(v2, 0.1f);

	CHECK(bvalue1 == false);
}

TEST(IsEqualTrue, vector_tests)
{
	bool bvalue1;
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(1.0f, 2.0f, 3.05f);

	bvalue1 = v1.IsEqual(v2, 0.1f);

	CHECK(bvalue1 == true);
}

TEST(IsZeroFalse, vector_tests)
{
	bool bvalue1;
	Vector v1(0.023f, 0.0f, 0.0f, 1.05f);

	bvalue1 = v1.IsZero(0.0f);

	CHECK(bvalue1 == false);
}

TEST(IsZeroTrue, vector_tests)
{
	bool bvalue1;
	Vector v1(0.0f, 0.0f, 0.0f, 1.05f);

	bvalue1 = v1.IsZero(0.1f);

	CHECK(bvalue1 == true);
}

TEST(IsEqualFalse_default, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(9.0f, -3.0f, 5.0f);
	bool bvalue1;

	bvalue1 = v1.IsEqual(v2);

	CHECK(bvalue1 == false);
}

TEST(IsEqualTrue_default, vector_tests)
{
	bool bvalue1;
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(1.0f, 2.0f, 3.05f);

	bvalue1 = v1.IsEqual(v2);

	CHECK(bvalue1 == false);
}

TEST(IsZeroFalse_default, vector_tests)
{
	bool bvalue1;
	Vector v1(0.023f, 0.0f, 0.0f, 1.05f);

	bvalue1 = v1.IsZero();

	CHECK(bvalue1 == false);
}

TEST(IsZeroTrue_default, vector_tests)
{
	bool bvalue1;
	Vector v1(0.0f, 0.0f, 0.0f, 1.05f);

	bvalue1 = v1.IsZero();

	CHECK(bvalue1 == false);
}

// ---  End of File ---------------
