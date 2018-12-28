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

TEST(Quat_Set_everyElement, QuatSetGetTests)
{
	Quat q1;
	// Set every element
	q1.Set(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(2.0f == q1.x);
	CHECK(3.0f == q1.y);
	CHECK(4.0f == q1.z);
	CHECK(5.0f == q1.w);
}

TEST(Vect_set_3_elements, QuatSetGetTests)
{
	Vector A(2.0f, 3.0f, 4.0f, 5.0f);

	CHECK(A.x == 2.0f);
	CHECK(A.y == 3.0f);
	CHECK(A.z == 4.0f);
	CHECK(A.w == 5.0f);

	A = Vector(10.0f, 20.0f, 30.0f);

	CHECK(A.x == 10.0f);
	CHECK(A.y == 20.0f);
	CHECK(A.z == 30.0f);
	CHECK(A.w == 1.0f);
}

TEST(Quat_Set_Vect_Real, QuatSetGetTests)
{
	Quat q1;
	Vector v1;

	// Set Vector component, Real component
	v1 = Vector(12.0f, 13.0f, 14.0f, 15.0f);
	q1 = Quat(v1, 20.0f);

	CHECK((12.0f == q1.x));
	CHECK((13.0f == q1.y));
	CHECK((14.0f == q1.z));
	CHECK((20.0f == q1.w));
}

TEST(Quat_Set_Quat_From_Matrix, QuatSetGetTests)
{
	Quat q1;
	Matrix M2, M3;

	// Set Quaternion from a Matrix
	Matrix Rx(ROT_X, Math::PiOver2);
	Matrix Ry(ROT_Y, -Math::PiOver3);
	Matrix Rz(ROT_Z, -Math::SevenPiOver8);
	M2 = Rx * Ry * Rz;

	q1.Set(M2);

	CHECK(eq(0.2273f, q1.x, Math::InternalTolerence));
	CHECK(eq(0.6695f, q1.y, Math::InternalTolerence));
	CHECK(eq(0.5316f, q1.z, Math::InternalTolerence));
	CHECK(eq(-0.4662f, q1.w, Math::InternalTolerence));

	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_IDENTITY, QuatSetGetTests)
{
	Quat q2, qtmp;
	Matrix M2, M3;
	// Set Special Type:
	q2.Set(IDENTITY);

	CHECK((q2.x == 0.0f));
	CHECK((q2.y == 0.0f));
	CHECK((q2.z == 0.0f));
	CHECK((q2.w == 1.0f));

	M2.Set(IDENTITY);
	qtmp.Set(M2);
	CHECK(eq(q2.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q2.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q2.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q2.w, qtmp.w, Math::InternalTolerence));

	M3.Set(q2);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ZERO, QuatSetGetTests)
{
	Quat q3, qtmp;
	Matrix m2, m3;

	// Set Special Type:
	q3.Set(ZERO);

	CHECK((q3.x == 0.0f));
	CHECK((q3.y == 0.0f));
	CHECK((q3.z == 0.0f));
	CHECK((q3.w == 0.0f));
}

TEST(Quat_Set_ROT_X_PiOver2, QuatSetGetTests)
{
	Quat q1, qtmp;
	Matrix M2, M3;
	// set Special Type:
	q1.Set(ROT_X, Math::PiOver2);

	// Matrix to Quaternion
	M2.Set(ROT_X, Math::PiOver2);
	qtmp.Set(M2);
	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_X_Pi, QuatSetGetTests)
{
	Quat q2, qtmp;
	Matrix M2, M3;

	// set Special Type:
	q2.Set(ROT_X, Math::Pi);

	// Matrix to Quaternion
	M2.Set(ROT_X, Math::Pi);
	qtmp.Set(M2);
	CHECK(eq(q2.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q2.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q2.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q2.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q2);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Y_PiOver2, QuatSetGetTests)
{
	Quat q3, qtmp;
	Matrix M2, M3;

	// set Special Type:
	q3.Set(ROT_Y, Math::PiOver2);

	// Matrix to Quaternion
	M2.Set(ROT_Y, Math::PiOver2);
	qtmp.Set(M2);
	CHECK(eq(q3.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q3.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q3.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q3.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q3);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Y_Pi, QuatSetGetTests)
{
	Quat q4, qtmp;
	Matrix M2, M3;

	// set Special Type:
	q4.Set(ROT_Y, Math::Pi);

	// Matrix to Quaternion
	M2.Set(ROT_Y, Math::Pi);
	qtmp.Set(M2);
	CHECK(eq(q4.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q4.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q4.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q4.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q4);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Z_PiOver2, QuatSetGetTests)
{
	Quat q1, qtmp;
	Matrix M2, M3;

	// set Special Type:
	q1.Set(ROT_Z, Math::PiOver2);

	// Matrix to Quaternion
	M2.Set(ROT_Z, Math::PiOver2);
	qtmp.Set(M2);
	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Z_Pi, QuatSetGetTests)
{
	Quat q2, qtmp;
	Matrix M2, M3;

	// set Special Type:
	q2.Set(ROT_Z, Math::Pi);

	// Matrix to Quaternion
	M2.Set(ROT_Z, Math::Pi);
	qtmp.Set(M2);
	CHECK(eq(q2.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q2.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q2.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q2.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q2);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_X, QuatSetGetTests)
{
	Quat q1, qtmp;
	Matrix M2, M3;

	// set Rot_X Type  :
	q1.Set(ROT_X, Math::PiOver3);

	// Matrix to Quaternion
	M2.Set(ROT_X, Math::PiOver3);
	qtmp.Set(M2);
	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Y, QuatSetGetTests)
{
	Quat q2, qtmp;
	Matrix M2, M3;

	// set Rot_Y Type  :
	q2.Set(ROT_Y, Math::PiOver3);

	// Matrix to Quaternion
	M2.Set(ROT_Y, Math::PiOver3);
	qtmp.Set(M2);
	CHECK(eq(q2.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q2.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q2.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q2.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q2);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_ROT_Z, QuatSetGetTests)
{
	Quat q3, qtmp;
	Matrix M2, M3;

	// set Rot_Z Type  :
	q3.Set(ROT_Z, Math::PiOver3);

	// Matrix to Quaternion
	M2.Set(ROT_Z, Math::PiOver3);
	qtmp.Set(M2);
	CHECK(eq(q3.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q3.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q3.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q3.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q3);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_XY_set, QuatSetGetTests)
{
	// Rot_XY Type Constructor:
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver3);
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver4);

	Quat Qxy;
	Qxy = Qx * Qy;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mx(ROT_X, Math::PiOver3);
	Matrix My(ROT_Y, Math::PiOver4);
	M2 = Mx * My;

	qtmp.Set(M2);
	CHECK(eq(Qxy.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qxy.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qxy.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qxy.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qxy);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_XZ_set, QuatSetGetTests)
{
	// Rot_XZ Type Constructor:
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver3);
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver4);
	Quat Qxz;

	Qxz = Qx * Qz;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;
	Matrix Mx(ROT_X, Math::PiOver3);
	Matrix Mz(ROT_Z, Math::PiOver4);
	M2 = Mx * Mz;

	qtmp.Set(M2);
	CHECK(eq(Qxz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qxz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qxz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qxz.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qxz);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_YX_set, QuatSetGetTests)
{
	// Rot_YX Type Constructor:
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver3);
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver4);
	Quat Qyx = Qy * Qx;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix My(ROT_Y, Math::PiOver3);
	Matrix Mx(ROT_X, Math::PiOver4);
	M2 = My * Mx;

	qtmp.Set(M2);
	CHECK(eq(Qyx.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qyx.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qyx.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qyx.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qyx);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_YZ_set, QuatSetGetTests)
{
	// Rot_YZ Type Constructor:
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver3);
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver4);
	Quat Qyz = Qy * Qz;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix My(ROT_Y, Math::PiOver3);
	Matrix Mz(ROT_Z, Math::PiOver4);
	M2 = My * Mz;

	qtmp.Set(M2);
	CHECK(eq(Qyz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qyz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qyz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qyz.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qyz);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ZX_set, QuatSetGetTests)
{
	// Rot_ZX Type Constructor:
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver3);
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver4);
	Quat Qzx = Qz * Qx;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mz(ROT_Z, Math::PiOver3);
	Matrix Mx(ROT_X, Math::PiOver4);
	M2 = Mz * Mx;

	qtmp.Set(M2);
	CHECK(eq(Qzx.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qzx.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qzx.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qzx.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qzx);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ZY_set, QuatSetGetTests)
{
	// Rot_ZY Type Constructor:

	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver3);
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver4);
	Quat Qzy = Qz * Qy;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mz(ROT_Z, Math::PiOver3);
	Matrix My(ROT_Y, Math::PiOver4);
	M2 = Mz * My;

	qtmp.Set(M2);
	CHECK(eq(Qzy.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qzy.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qzy.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qzy.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qzy);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_XYZ_set, QuatSetGetTests)
{
	// Rot_XYZ Type Constructor:
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver3);
	Quat Qy;
	Qy.Set(ROT_Y, Math::FivePiOver8);
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver4);
	Quat Qxyz = Qx * Qy * Qz;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mx(ROT_X, Math::PiOver3);
	Matrix My(ROT_Y, Math::FivePiOver8);
	Matrix Mz(ROT_Z, Math::PiOver4);
	M2 = Mx * My * Mz;

	qtmp.Set(M2);
	CHECK(eq(Qxyz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qxyz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qxyz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qxyz.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qxyz);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_XZY_set, QuatSetGetTests)
{
	// Rot_XZY Type Constructor:
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver3);
	Quat Qz;
	Qz.Set(ROT_Z, Math::FivePiOver8);
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver4);
	Quat Qxzy = Qx * Qz * Qy;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mx(ROT_X, Math::PiOver3);
	Matrix Mz(ROT_Z, Math::FivePiOver8);
	Matrix My(ROT_Y, Math::PiOver4);
	M2 = Mx * Mz * My;

	qtmp.Set(M2);
	CHECK(eq(Qxzy.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qxzy.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qxzy.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qxzy.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qxzy);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_YXZ_set, QuatSetGetTests)
{
	// Rot_YXZ Type Constructor:
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver3);
	Quat Qx;
	Qx.Set(ROT_X, Math::FivePiOver8);
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver4);
	Quat Qyxz = Qy * Qx * Qz;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix My(ROT_Y, Math::PiOver3);
	Matrix Mx(ROT_X, Math::FivePiOver8);
	Matrix Mz(ROT_Z, Math::PiOver4);
	M2 = My * Mx * Mz;

	qtmp.Set(M2);
	CHECK(eq(Qyxz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qyxz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qyxz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qyxz.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qyxz);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_YZX_set, QuatSetGetTests)
{
	// Rot_YZX Type Constructor:
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver3);
	Quat Qz;
	Qz.Set(ROT_Z, Math::FivePiOver8);
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver4);
	Quat Qyzx = Qy * Qz * Qx;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix My(ROT_Y, Math::PiOver3);
	Matrix Mz(ROT_Z, Math::FivePiOver8);
	Matrix Mx(ROT_X, Math::PiOver4);
	M2 = My * Mz * Mx;

	qtmp.Set(M2);
	CHECK(eq(Qyzx.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qyzx.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qyzx.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qyzx.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qyzx);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ZXY_set, QuatSetGetTests)
{
	// Rot_ZXY Type Constructor:
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver3);
	Quat Qx;
	Qx.Set(ROT_X, Math::FivePiOver8);
	Quat Qy;
	Qy.Set(ROT_Y, Math::PiOver4);
	Quat Qzxy = Qz * Qx * Qy;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mz(ROT_Z, Math::PiOver3);
	Matrix Mx(ROT_X, Math::FivePiOver8);
	Matrix My(ROT_Y, Math::PiOver4);
	M2 = Mz * Mx * My;

	qtmp.Set(M2);
	CHECK(eq(Qzxy.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qzxy.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qzxy.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qzxy.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qzxy);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ZYX_set, QuatSetGetTests)
{
	// Rot_ZYX Type Constructor:
	Quat Qz;
	Qz.Set(ROT_Z, Math::PiOver3);
	Quat Qy;
	Qy.Set(ROT_Y, Math::FivePiOver8);
	Quat Qx;
	Qx.Set(ROT_X, Math::PiOver4);
	Quat Qzyx = Qz * Qy * Qx;

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	Matrix Mz(ROT_Z, Math::PiOver3);
	Matrix My(ROT_Y, Math::FivePiOver8);
	Matrix Mx(ROT_X, Math::PiOver4);
	M2 = Mz * My * Mx;

	qtmp.Set(M2);
	CHECK(eq(Qzyx.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qzyx.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qzyx.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qzyx.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qzyx);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_AXIS_ANGLE_set, QuatSetGetTests)
{
	// Axis and Angle Type Constructor:
	Vector  v11(2.0f, 53.0f, 24.0f);
	Quat Qa1;
	Qa1.Set(ROT_AXIS_ANGLE, v11, Math::PiOver3);

	// Matrix to Quaternion
	Matrix M2, M3;
	Quat qtmp;

	M2.Set(ROT_AXIS_ANGLE, v11, Math::PiOver3);

	qtmp.Set(M2);
	CHECK(eq(Qa1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(Qa1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(Qa1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(Qa1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(Qa1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ROT_ORIENT_set, QuatSetGetTests)
{
	// Orientation Type Constructor:
	Vector  v15(2.0f, 53.0f, 24.0f);
	Vector  v16(0.0f, -24.0f, 53.0f);
	Quat q56;
	q56.Set(ROT_ORIENT, v15, v16);

	Matrix M2, M3;
	Quat qtmp;

	M2.Set(ROT_ORIENT, v15, v16);
	qtmp.Set(M2);
	CHECK(eq(q56.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q56.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q56.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q56.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q56);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_ROT_INVERSE_ORIENT_set, QuatSetGetTests)
{
	// Orientation Type Constructor:
	Vector  v17(2.0f, 53.0f, 24.0f);
	Vector  v18(0.0f, -24.0f, 53.0f);
	Quat q57;
	q57.Set(ROT_INVERSE_ORIENT, v17, v18);

	Matrix M2, M3;
	Quat qtmp;

	M2.Set(ROT_INVERSE_ORIENT, v17, v18);
	qtmp.Set(M2);
	CHECK(eq(q57.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q57.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q57.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q57.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q57);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));
}

TEST(Quat_Set_Each_ELEMENT, QuatSetGetTests)
{
	Quat q2;
	// set Quat each Element:
	q2.Set(ZERO);

	q2.x = 11.0f;
	q2.y = 12.0f;
	q2.z = 13.0f;
	q2.w = 14.0f;

	CHECK((q2.x == 11.0f));
	CHECK((q2.y == 12.0f));
	CHECK((q2.z == 13.0f));
	CHECK((q2.w == 14.0f));
}

TEST(Quat_Set_From_VECT, QuatSetGetTests)
{
	Vector v1;
	Quat q2;
	// set Quat Vector Element:

	v1 = Vector(3.0f, 4.0f, 5.0f, 6.0f);

	q2.Set(ZERO);

	q2.SetVector(v1);

	CHECK((q2.x == 3.0f));
	CHECK((q2.y == 4.0f));
	CHECK((q2.z == 5.0f));
	CHECK((q2.w == 0.0f));
}

TEST(Quat_Get_Vect_elements, QuatSetGetTests)
{
	Quat q1;
	float tmp_x, tmp_y, tmp_z, tmp_real;

	// Get Quat Vector Elements:
	q1.Set(3.0f, 4.0f, 5.0f, 6.0f);

	tmp_x = q1.x;
	tmp_y = q1.y;
	tmp_z = q1.z;
	tmp_real = q1.w;

	CHECK((q1.x == 3.0f));
	CHECK((q1.y == 4.0f));
	CHECK((q1.z == 5.0f));
	CHECK((q1.w == 6.0f));
	CHECK((tmp_x == 3.0f));
	CHECK((tmp_y == 4.0f));
	CHECK((tmp_z == 5.0f));
	CHECK((tmp_real == 6.0f));
}

TEST(Quat_Get_Vect, QuatSetGetTests)
{
	Quat q1;
	Vector v1;

	//  Get Vector:
	q1.Set(7.0f, 8.0f, 9.0f, 10.0f);

	v1 = q1.GetVector();

	CHECK((v1.x == 7.0f));
	CHECK((v1.y == 8.0f));
	CHECK((v1.z == 9.0f));
	CHECK((v1.w == 1.0f));
	CHECK((q1.x == 7.0f));
	CHECK((q1.y == 8.0f));
	CHECK((q1.z == 9.0f));
	CHECK((q1.w == 10.0f));
}

TEST(Quat_Get_Angle_of_rotation, QuatSetGetTests)
{
	Quat q1, qtmp;
	Vector v1;
	Matrix M2, M3;

	//  Get Angle of rotattion about the axis of rotation
	v1 = Vector(2.0f, 53.0f, 24.0f);
	q1.Set(ROT_AXIS_ANGLE, v1, Math::PiOver3);

	// Matrix to Quaternion
	M2.Set(ROT_AXIS_ANGLE, v1, Math::PiOver3);
	qtmp.Set(M2);
	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));

	float value;
	value = q1.GetAngle();
	CHECK(eq(value, Math::PiOver3, 0.02f));
}

TEST(Quat_Get_Axis_of_rotation, QuatSetGetTests)
{
	Quat q1, qtmp;
	Vector v1;
	Matrix M2, M3;

	//  Get Axis of rotattion about the axis of rotation
	v1 = Vector(2.0f, 53.0f, 24.0f);

	q1.Set(ROT_AXIS_ANGLE, v1, Math::PiOver3);

	// Matrix to Quaternion
	M2.Set(ROT_AXIS_ANGLE, v1, Math::PiOver3);
	qtmp.Set(M2);
	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	M3.Set(q1);

	CHECK(eq(M2[m0], M3[m0], Math::InternalTolerence));
	CHECK(eq(M2[m1], M3[m1], Math::InternalTolerence));
	CHECK(eq(M2[m2], M3[m2], Math::InternalTolerence));
	CHECK(eq(M2[m3], M3[m3], Math::InternalTolerence));
	CHECK(eq(M2[m4], M3[m4], Math::InternalTolerence));
	CHECK(eq(M2[m5], M3[m5], Math::InternalTolerence));
	CHECK(eq(M2[m6], M3[m6], Math::InternalTolerence));
	CHECK(eq(M2[m7], M3[m7], Math::InternalTolerence));
	CHECK(eq(M2[m8], M3[m8], Math::InternalTolerence));
	CHECK(eq(M2[m9], M3[m9], Math::InternalTolerence));
	CHECK(eq(M2[m10], M3[m10], Math::InternalTolerence));
	CHECK(eq(M2[m11], M3[m11], Math::InternalTolerence));
	CHECK(eq(M2[m12], M3[m12], Math::InternalTolerence));
	CHECK(eq(M2[m13], M3[m13], Math::InternalTolerence));
	CHECK(eq(M2[m14], M3[m14], Math::InternalTolerence));
	CHECK(eq(M2[m15], M3[m15], Math::InternalTolerence));

	//  Get axis of rotation:
	q1.Set(7.0f, 8.0f, 9.0f, 10.0f);

	v1 = q1.GetAxis();

	CHECK((v1.x == 7.0f));
	CHECK((v1.y == 8.0f));
	CHECK((v1.z == 9.0f));
	CHECK((v1.w == 1.0f));
	CHECK((q1.x == 7.0f));
	CHECK((q1.y == 8.0f));
	CHECK((q1.z == 9.0f));
	CHECK((q1.w == 10.0f));
}

TEST(Quat_Get_bracket1, QuatSetGetTests)
{
	Quat q1;
	float tmp_x, tmp_y, tmp_z, tmp_real;

	//  [] operator:
	q1.Set(7.0f, 8.0f, 9.0f, 10.0f);

	tmp_x = q1.x;
	tmp_y = q1.y;
	tmp_z = q1.z;
	tmp_real = q1.w;

	CHECK((q1.x == 7.0f));
	CHECK((q1.y == 8.0f));
	CHECK((q1.z == 9.0f));
	CHECK((q1.w == 10.0f));
	CHECK((tmp_x == 7.0f));
	CHECK((tmp_y == 8.0f));
	CHECK((tmp_z == 9.0f));
	CHECK((tmp_real == 10.0f));
}

TEST(Quat_Get_bracket2, QuatSetGetTests)
{
	Quat q1;
	float tmp_x, tmp_y, tmp_z, tmp_real;

	//  [] operator:
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	tmp_x = q1.x;
	tmp_y = q1.y;
	tmp_z = q1.z;
	tmp_real = q1.w;

	CHECK((q1.x == 1.0f));
	CHECK((q1.y == 2.0f));
	CHECK((q1.z == 3.0f));
	CHECK((q1.w == 4.0f));
	CHECK((tmp_x == 1.0f));
	CHECK((tmp_y == 2.0f));
	CHECK((tmp_z == 3.0f));
	CHECK((tmp_real == 4.0f));
}

TEST(Quat_Get_bracket3, QuatSetGetTests)
{
	Quat q1;
	float tmp_x, tmp_y, tmp_z, tmp_real;

	//  [] operator:
	q1.Set(7.0f, 8.0f, 9.0f, 10.0f);

	tmp_x = q1.x;
	tmp_y = q1.y;
	tmp_z = q1.z;
	tmp_real = q1.w;

	CHECK((q1.x == 7.0f));
	CHECK((q1.y == 8.0f));
	CHECK((q1.z == 9.0f));
	CHECK((q1.w == 10.0f));
	CHECK((tmp_x == 7.0f));
	CHECK((tmp_y == 8.0f));
	CHECK((tmp_z == 9.0f));
	CHECK((tmp_real == 10.0f));
}

TEST(Quat_Get_bracket4, QuatSetGetTests)
{
	Quat q1;
	float tmp_x, tmp_y, tmp_z, tmp_real;

	//  [] operator:
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	tmp_x = q1.x;
	tmp_y = q1.y;
	tmp_z = q1.z;
	tmp_real = q1.w;

	CHECK((q1.x == 1.0f));
	CHECK((q1.y == 2.0f));
	CHECK((q1.z == 3.0f));
	CHECK((q1.w == 4.0f));
	CHECK((tmp_x == 1.0f));
	CHECK((tmp_y == 2.0f));
	CHECK((tmp_z == 3.0f));
	CHECK((tmp_real == 4.0f));
}

// ---  End of File ---------------
