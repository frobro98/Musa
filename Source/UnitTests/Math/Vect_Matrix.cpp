//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/Matrix4.hpp"

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Vect_mult_matrix, vector_tests)
{
	Vector4 vA(2.0f, 3.0f, 4.0f, -2.0f);
	Vector4 vOut;

	CHECK_EQ(vA.x, 2.0f);
	CHECK_EQ(vA.y, 3.0f);
	CHECK_EQ(vA.z, 4.0f);
	CHECK_EQ(vA.w, -2.0f);

	Vector4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vector4 V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vector4 V3(9.0f, -7.0f, -2.0f, 5.0f);

	Matrix4 M(V0, V1, V2, V3);

	CHECK_EQ(M[m0], 1.0f);
	CHECK_EQ(M[m1], 2.0f);
	CHECK_EQ(M[m2], 3.0f);
	CHECK_EQ(M[m3], 4.0f);
	CHECK_EQ(M[m4], 7.0f);
	CHECK_EQ(M[m5], 6.0f);
	CHECK_EQ(M[m6], 5.0f);
	CHECK_EQ(M[m7], 3.0f);
	CHECK_EQ(M[m8], -4.0f);
	CHECK_EQ(M[m9], -2.0f);
	CHECK_EQ(M[m10], -1.0f);
	CHECK_EQ(M[m11], -4.0f);
	CHECK_EQ(M[m12], 9.0f);
	CHECK_EQ(M[m13], -7.0f);
	CHECK_EQ(M[m14], -2.0f);
	CHECK_EQ(M[m15], 5.0f);

	vOut = vA * M;

	CHECK_EQ(M[m0], 1.0f);
	CHECK_EQ(M[m1], 2.0f);
	CHECK_EQ(M[m2], 3.0f);
	CHECK_EQ(M[m3], 4.0f);
	CHECK_EQ(M[m4], 7.0f);
	CHECK_EQ(M[m5], 6.0f);
	CHECK_EQ(M[m6], 5.0f);
	CHECK_EQ(M[m7], 3.0f);
	CHECK_EQ(M[m8], -4.0f);
	CHECK_EQ(M[m9], -2.0f);
	CHECK_EQ(M[m10], -1.0f);
	CHECK_EQ(M[m11], -4.0f);
	CHECK_EQ(M[m12], 9.0f);
	CHECK_EQ(M[m13], -7.0f);
	CHECK_EQ(M[m14], -2.0f);
	CHECK_EQ(M[m15], 5.0f);

	CHECK_EQ(vA.x, 2.0f);
	CHECK_EQ(vA.y, 3.0f);
	CHECK_EQ(vA.z, 4.0f);
	CHECK_EQ(vA.w, -2.0f);

	CHECK_EQ(vOut.x, -11.0f);
	CHECK_EQ(vOut.y, 28.0f);
	CHECK_EQ(vOut.z, 21.0f);
	CHECK_EQ(vOut.w, -9.0f);
}

TEST(Vect_multEqual_matrix, vector_tests)
{
	Vector4 vA(2.0f, 3.0f, 4.0f, -2.0f);

	CHECK_EQ(vA.x, 2.0f);
	CHECK_EQ(vA.y, 3.0f);
	CHECK_EQ(vA.z, 4.0f);
	CHECK_EQ(vA.w, -2.0f);


	Vector4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vector4 V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vector4 V3(9.0f, -7.0f, -2.0f, 5.0f);

	Matrix4 M(V0, V1, V2, V3);

	CHECK_EQ(M[m0], 1.0f);
	CHECK_EQ(M[m1], 2.0f);
	CHECK_EQ(M[m2], 3.0f);
	CHECK_EQ(M[m3], 4.0f);
	CHECK_EQ(M[m4], 7.0f);
	CHECK_EQ(M[m5], 6.0f);
	CHECK_EQ(M[m6], 5.0f);
	CHECK_EQ(M[m7], 3.0f);
	CHECK_EQ(M[m8], -4.0f);
	CHECK_EQ(M[m9], -2.0f);
	CHECK_EQ(M[m10], -1.0f);
	CHECK_EQ(M[m11], -4.0f);
	CHECK_EQ(M[m12], 9.0f);
	CHECK_EQ(M[m13], -7.0f);
	CHECK_EQ(M[m14], -2.0f);
	CHECK_EQ(M[m15], 5.0f);

	vA *= M;

	CHECK_EQ(M[m0], 1.0f);
	CHECK_EQ(M[m1], 2.0f);
	CHECK_EQ(M[m2], 3.0f);
	CHECK_EQ(M[m3], 4.0f);
	CHECK_EQ(M[m4], 7.0f);
	CHECK_EQ(M[m5], 6.0f);
	CHECK_EQ(M[m6], 5.0f);
	CHECK_EQ(M[m7], 3.0f);
	CHECK_EQ(M[m8], -4.0f);
	CHECK_EQ(M[m9], -2.0f);
	CHECK_EQ(M[m10], -1.0f);
	CHECK_EQ(M[m11], -4.0f);
	CHECK_EQ(M[m12], 9.0f);
	CHECK_EQ(M[m13], -7.0f);
	CHECK_EQ(M[m14], -2.0f);
	CHECK_EQ(M[m15], 5.0f);

	CHECK_EQ(vA.x, -11.0f);
	CHECK_EQ(vA.y, 28.0f);
	CHECK_EQ(vA.z, 21.0f);
	CHECK_EQ(vA.w, -9.0f);
}

// ---  End of File ---------------
