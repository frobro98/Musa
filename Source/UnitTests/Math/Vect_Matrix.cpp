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

TEST(Vect_mult_matrix, vector_tests)
{
	Vector vA(2.0f, 3.0f, 4.0f, -2.0f);
	Vector vOut;

	CHECK(vA.x == 2.0f);
	CHECK(vA.y == 3.0f);
	CHECK(vA.z == 4.0f);
	CHECK(vA.w == -2.0f);

	Vector V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vector V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vector V3(9.0f, -7.0f, -2.0f, 5.0f);

	Matrix M(V0, V1, V2, V3);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	vOut = vA * M;

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	CHECK(vA.x == 2.0f);
	CHECK(vA.y == 3.0f);
	CHECK(vA.z == 4.0f);
	CHECK(vA.w == -2.0f);

	CHECK(eq(vOut.x, -11.0f, Math::InternalTolerence));
	CHECK(eq(vOut.y, 28.0f, Math::InternalTolerence));
	CHECK(eq(vOut.z, 21.0f, Math::InternalTolerence));
	CHECK(eq(vOut.w, -9.0f, Math::InternalTolerence));
}

TEST(Vect_multEqual_matrix, vector_tests)
{
	Vector vA(2.0f, 3.0f, 4.0f, -2.0f);

	CHECK(vA.x == 2.0f);
	CHECK(vA.y == 3.0f);
	CHECK(vA.z == 4.0f);
	CHECK(vA.w == -2.0f);


	Vector V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vector V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vector V3(9.0f, -7.0f, -2.0f, 5.0f);

	Matrix M(V0, V1, V2, V3);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	vA *= M;

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	CHECK(eq(vA.x, -11.0f, Math::InternalTolerence));
	CHECK(eq(vA.y, 28.0f, Math::InternalTolerence));
	CHECK(eq(vA.z, 21.0f, Math::InternalTolerence));
	CHECK(eq(vA.w, -9.0f, Math::InternalTolerence));
}

// ---  End of File ---------------
