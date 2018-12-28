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

TEST(Dot, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(4.0f, 5.0f, 6.0f);

	float value1;

	value1 = v1.Dot(v2);
	CHECK(value1 == 32.0f);
}

TEST(Cross, vector_tests)
{
	Vector v1(1.0f, 0.0f, 0.0f);
	Vector v2(0.0f, 1.0f, 0.0f);
	Vector out1;

	out1 = v1.Cross(v2);

	CHECK((out1.x == 0.0f));
	CHECK((out1.y == 0.0f));
	CHECK((out1.z == 1.0f));
	CHECK((out1.w == 1.0f));
}

TEST(Normalizealize, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);

	v1.Normalize();

	CHECK(eq(v1.x, 0.2672f, Math::InternalTolerence));
	CHECK(eq(v1.y, 0.5345f, Math::InternalTolerence));
	CHECK(eq(v1.z, 0.80174f, Math::InternalTolerence));
	CHECK((v1.w == 1.0f));
}

TEST(GetNormalize, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2;

	v2 = v1.GetNormalized();

	CHECK(eq(v2.x, 0.2672f, Math::InternalTolerence));
	CHECK(eq(v2.y, 0.5345f, Math::InternalTolerence));
	CHECK(eq(v2.z, .80174f, Math::InternalTolerence));
	CHECK((v2.w == 1.0f));
	CHECK((v1.x == 1.0f));
	CHECK((v1.y == 2.0f));
	CHECK((v1.z == 3.0f));
	CHECK((v1.w == 1.0f));
}

TEST(Magnitude, VectFunctionTests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	float value;

	value = v1.Magnitude();

	CHECK(eq(value, 3.7416f, Math::InternalTolerence));
}

TEST(MagnitudeSquared, vector_tests)
{
	Vector v1(1.0f, 2.0f, 3.0f);
	float value;
	value = v1.MagnitudeSqr();
	CHECK(eq(value, 14, Math::InternalTolerence));
}

TEST(GetAngle, vector_tests)
{
	float value;
	Vector v1(1.0f, 2.0f, 3.0f);
	Vector v2(1.80f, -2.0f, 2.0f);

	value = v1.GetAngle(v2);

	CHECK(eq(value, 1.263f, Math::InternalTolerence));
}

// ---  End of File ---------------
