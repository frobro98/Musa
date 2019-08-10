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

TEST(default_constuctor, vector_tests)
{
	Vector4 A;

	CHECK(A.x == 0.0f);
	CHECK(A.y == 0.0f);
	CHECK(A.z == 0.0f);
	CHECK(A.w == 1.0f);
}

TEST(individual_defaultW_constuctor, vector_tests)
{
	Vector4 A(2.0f, 3.0f, 4.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 1.0f);
}

TEST(individual_constuctor, vector_tests)
{
	Vector4 A(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);
}

TEST(assignment_operator, vector_tests)
{
	Vector4 A(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	CHECK(B.x == 10.0f);
	CHECK(B.y == 11.0f);
	CHECK(B.z == 12.0f);
	CHECK(B.w == 13.0f);

	B = A;

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	CHECK(B.x == A.x);
	CHECK(B.y == A.y);
	CHECK(B.z == A.z);
	CHECK(B.w == A.w);
}

TEST(Copy_constuctor, vector_tests)
{
	Vector4 A(2.0f, 3.0f, 4.0f, 5.0f);
	Vector4 B(A);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	CHECK(B.x == A.x);
	CHECK(B.y == A.y);
	CHECK(B.z == A.z);
	CHECK(B.w == A.w);
}

TEST(Destructor_constuctor, vector_tests)
{
	Vector4 v2(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 *pV2 = &v2;
	pV2->~Vector4();
	CHECK(1);
}

// ---  End of File ---------------
