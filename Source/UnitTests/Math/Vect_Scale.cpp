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

TEST(scale_Vect, vector_tests)
{
	Vector A(2.0f, 3.0f, 4.0f, 5.0f);
	float s = 5.0f;
	Vector C;

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	C = s * A;

	CHECK(C.x == 10.0f);
	CHECK(C.y == 15.0f);
	CHECK(C.z == 20.0f);
	CHECK(C.w == 1.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);
}

TEST(Vect_times_Scale, vector_tests)
{
	Vector A(2.0f, 3.0f, 4.0f, 5.0f);
	float s = 5.0f;
	Vector C;

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	C = A*s;

	CHECK(C.x == 10.0f);
	CHECK(C.y == 15.0f);
	CHECK(C.z == 20.0f);
	CHECK(C.w == 1.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);
}

TEST(Vect_ScaleEquals, vector_tests)
{
	Vector A(2.0f, 3.0f, 4.0f, 5.0f);
	float s = 5.0f;

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	A *= s;

	CHECK(A.x == 10.0f);
	CHECK(A.y == 15.0f);
	CHECK(A.z == 20.0f);
	CHECK(A.w == 1.0f);
}

// ---  End of File ---------------
