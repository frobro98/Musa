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

TEST( VectorLine, combo_tests )
{
	Vector4 A(0.0f, 50.0f, 300.0f, 5.0f);
	Vector4 B(10.0f, 100.0f, 1000.0f, 51.0f);
	Vector4 C;

	CHECK( A.x == 0.0f );
	CHECK( A.y == 50.0f );
	CHECK( A.z == 300.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 100.0f );
	CHECK( B.z == 1000.0f );
	CHECK( B.w == 51.0f );

	Vector4 Line;
	float t;

	t = 0.0f;
	Line = A + t * ( B - A);

	CHECK( Line.x == 0.0f );
	CHECK( Line.y == 50.0f );
	CHECK( Line.z == 300.0f );
	CHECK( Line.w == 1.0f );

	CHECK( A.x == 0.0f );
	CHECK( A.y == 50.0f );
	CHECK( A.z == 300.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 100.0f );
	CHECK( B.z == 1000.0f );
	CHECK( B.w == 51.0f );

	t = 1.0f;
	Line = A + t * ( B - A);

	CHECK( Line.x == 10.0f );
	CHECK( Line.y == 100.0f );
	CHECK( Line.z == 1000.0f );
	CHECK( Line.w == 1.0f );

	CHECK( A.x == 0.0f );
	CHECK( A.y == 50.0f );
	CHECK( A.z == 300.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 100.0f );
	CHECK( B.z == 1000.0f );
	CHECK( B.w == 51.0f );

	t = 0.5f;
	Line = A + t * ( B - A);

	CHECK( Line.x == 5.0f );
	CHECK( Line.y == 75.0f );
	CHECK( Line.z == 650.0f );
	CHECK( Line.w == 1.0f );

	CHECK( A.x == 0.0f );
	CHECK( A.y == 50.0f );
	CHECK( A.z == 300.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 100.0f );
	CHECK( B.z == 1000.0f );
	CHECK( B.w == 51.0f );
}

TEST( VectorCrossAddSub, combo_tests )
{
	Vector4 A(1.0f, 2.0f, 3.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);
	Vector4 C;

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );

	C = A + (A-B).Cross(B);

	CHECK( C.x == -8.0f );
	CHECK( C.y == 20.0f );
	CHECK( C.z == -6.0f );
	CHECK( C.w == 1.0f );

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );
}

TEST( VectorCrossAddSubMatrixMult, combo_tests )
{
	Vector4 A(1.0f, 2.0f, 3.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);
	Vector4 C;

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );

	C = A + (A-B).Cross(B);
	Vector4 D = C.Dot(A) * B;

	CHECK( D.x == 140.0f );
	CHECK( D.y == 154.0f );
	CHECK( D.z == 168.0f );
	CHECK( D.w == 1.0f );

	CHECK( C.x == -8.0f );
	CHECK( C.y == 20.0f );
	CHECK( C.z == -6.0f );
	CHECK( C.w == 1.0f );

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );

	Matrix4 M(A,B,C,D);

	A = B * M;

	CHECK( A.x == 1844.0f );
	CHECK( A.y == 2383.0f );
	CHECK( A.z == 2274.0f );
	CHECK( A.w == 218.0f );
}

TEST( VectorCrossAddSubMatrixMultCross, combo_tests )
{
	Vector4 A(1.0f, 2.0f, 3.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);
	Vector4 C;

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );
	
	C = A + (A-B).Cross(B);
	Vector4 D = C.Dot(A) * B;
	CHECK( D.x == 140.0f );
	CHECK( D.y == 154.0f );
	CHECK( D.z == 168.0f );
	CHECK( D.w == 1.0f );

	CHECK( C.x == -8.0f );
	CHECK( C.y == 20.0f );
	CHECK( C.z == -6.0f );
	CHECK( C.w == 1.0f );

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );

	Matrix4 M(A,B,C,D);

	A = Vector4(1.0f, 0.0f, 4.0f,1.0f).Cross(B * M);

	CHECK( A.x == -9532.0f );
	CHECK( A.y == 5102.0f );
	CHECK( A.z == 2383.0f );
	CHECK( A.w == 1.0f );
}

TEST( VectorCrossCrossNormalize, combo_tests )
{
	Vector4 A(1.0f, 2.0f, 3.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);
	Vector4 C;

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );
	
	C = (A-B).Cross(B);
	Vector4 D = C.Cross(B);
	D.Normalize();

	CHECK( eq( D.x, 0.74790420f, Math::InternalTolerence) );
	CHECK( eq( D.y, 0.04273730f, Math::InternalTolerence) );
	CHECK( eq( D.z, -0.6624290f, Math::InternalTolerence) );
	CHECK( D.w == 1.0f );

	CHECK( C.x == -9.0f );
	CHECK( C.y == 18.0f );
	CHECK( C.z == -9.0f );
	CHECK( C.w == 1.0f );

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );
}

TEST( junkfood, combo_tests )
{
	Vector4 A(1.0f, 2.0f, 3.0f, 5.0f);
	Vector4 B(10.0f, 11.0f, 12.0f, 13.0f);
	Vector4 C;

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );

	float junk = A.Dot((A-B).Cross(B))+5.0f;

	CHECK( junk == 5.0f );

	CHECK( A.x == 1.0f );
	CHECK( A.y == 2.0f );
	CHECK( A.z == 3.0f );
	CHECK( A.w == 5.0f );

	CHECK( B.x == 10.0f );
	CHECK( B.y == 11.0f );
	CHECK( B.z == 12.0f );
	CHECK( B.w == 13.0f );
}

TEST(ComboRex, combo_tests)
{
	Vector4 v0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v1(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 v2(9.0f, 10.0f, 11.0f, 12.0f);
	Vector4 v3(13.0f, 14.0f, 15.0f, 16.0f);
	Vector4 v4(10.0f, 20.0f, 30.0f, 40.0f);
	Vector4 v5(50.0f, 60.0f, 70.0f, 80.0f);
	Vector4 v6(90.0f, 100.0f, 110.0f, 120.0f);
	Vector4 v7(130.0f, 140.0f, 150.0f, 160.0f);

	Matrix4 A(v0, v1, v2, v3);
	Matrix4 B(v4, v5, v6, v7);
	Matrix4 C;
	Matrix4 D;

	CHECK((A[m0] == 1.0f));
	CHECK((A[m1] == 2.0f));
	CHECK((A[m2] == 3.0f));
	CHECK((A[m3] == 4.0f));
	CHECK((A[m4] == 5.0f));
	CHECK((A[m5] == 6.0f));
	CHECK((A[m6] == 7.0f));
	CHECK((A[m7] == 8.0f));
	CHECK((A[m8] == 9.0f));
	CHECK((A[m9] == 10.0f));
	CHECK((A[m10] == 11.0f));
	CHECK((A[m11] == 12.0f));
	CHECK((A[m12] == 13.0f));
	CHECK((A[m13] == 14.0f));
	CHECK((A[m14] == 15.0f));
	CHECK((A[m15] == 16.0f));

	CHECK((B[m0] == 10.0f));
	CHECK((B[m1] == 20.0f));
	CHECK((B[m2] == 30.0f));
	CHECK((B[m3] == 40.0f));
	CHECK((B[m4] == 50.0f));
	CHECK((B[m5] == 60.0f));
	CHECK((B[m6] == 70.0f));
	CHECK((B[m7] == 80.0f));
	CHECK((B[m8] == 90.0f));
	CHECK((B[m9] == 100.0f));
	CHECK((B[m10] == 110.0f));
	CHECK((B[m11] == 120.0f));
	CHECK((B[m12] == 130.0f));
	CHECK((B[m13] == 140.0f));
	CHECK((B[m14] == 150.0f));
	CHECK((B[m15] == 160.0f));

	Quat	Q1;
	Matrix4	M1;

	Q1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);
	M1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);

	C = A * Q1 * B;
	D = A * M1 * B;

	CHECK(eq(C[m0],  D[m0], Math::InternalTolerence));
	CHECK(eq(C[m1],  D[m1], Math::InternalTolerence));
	CHECK(eq(C[m2],  D[m2], Math::InternalTolerence));
	CHECK(eq(C[m3],  D[m3], Math::InternalTolerence));
	CHECK(eq(C[m4],  D[m4], Math::InternalTolerence));
	CHECK(eq(C[m5],  D[m5], Math::InternalTolerence));
	CHECK(eq(C[m6],  D[m6], Math::InternalTolerence));
	CHECK(eq(C[m7],  D[m7], Math::InternalTolerence));
	CHECK(eq(C[m8],  D[m8], Math::InternalTolerence));
	CHECK(eq(C[m9],  D[m9], Math::InternalTolerence));
	CHECK(eq(C[m10], D[m10], Math::InternalTolerence));
	CHECK(eq(C[m11], D[m11], Math::InternalTolerence));
	CHECK(eq(C[m12], D[m12], Math::InternalTolerence));
	CHECK(eq(C[m13], D[m13], Math::InternalTolerence));
	CHECK(eq(C[m14], D[m14], Math::InternalTolerence));
	CHECK(eq(C[m15], D[m15], Math::InternalTolerence));
}

TEST(ComboRex2, combo_tests)
{
	Vector4 v0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 v1(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 v2(9.0f, 10.0f, 11.0f, 12.0f);
	Vector4 v3(13.0f, 14.0f, 15.0f, 16.0f);
	Vector4 v4(10.0f, 20.0f, 30.0f, 40.0f);
	Vector4 v5(50.0f, 60.0f, 70.0f, 80.0f);
	Vector4 v6(90.0f, 100.0f, 110.0f, 120.0f);
	Vector4 v7(130.0f, 140.0f, 150.0f, 160.0f);

	Matrix4 A(v0, v1, v2, v3);
	Matrix4 B(v4, v5, v6, v7);
	Matrix4 C;
	Matrix4 D;

	CHECK((A[m0] == 1.0f));
	CHECK((A[m1] == 2.0f));
	CHECK((A[m2] == 3.0f));
	CHECK((A[m3] == 4.0f));
	CHECK((A[m4] == 5.0f));
	CHECK((A[m5] == 6.0f));
	CHECK((A[m6] == 7.0f));
	CHECK((A[m7] == 8.0f));
	CHECK((A[m8] == 9.0f));
	CHECK((A[m9] == 10.0f));
	CHECK((A[m10] == 11.0f));
	CHECK((A[m11] == 12.0f));
	CHECK((A[m12] == 13.0f));
	CHECK((A[m13] == 14.0f));
	CHECK((A[m14] == 15.0f));
	CHECK((A[m15] == 16.0f));

	CHECK((B[m0] == 10.0f));
	CHECK((B[m1] == 20.0f));
	CHECK((B[m2] == 30.0f));
	CHECK((B[m3] == 40.0f));
	CHECK((B[m4] == 50.0f));
	CHECK((B[m5] == 60.0f));
	CHECK((B[m6] == 70.0f));
	CHECK((B[m7] == 80.0f));
	CHECK((B[m8] == 90.0f));
	CHECK((B[m9] == 100.0f));
	CHECK((B[m10] == 110.0f));
	CHECK((B[m11] == 120.0f));
	CHECK((B[m12] == 130.0f));
	CHECK((B[m13] == 140.0f));
	CHECK((B[m14] == 150.0f));
	CHECK((B[m15] == 160.0f));

	Quat	Q1;
	Matrix4	M1;

	Q1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);
	M1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);

	C = Q1 * B * A;
	D = M1 * B * A;

	CHECK(eq(C[m0], D[m0], Math::InternalTolerence));
	CHECK(eq(C[m1], D[m1], Math::InternalTolerence));
	CHECK(eq(C[m2], D[m2], Math::InternalTolerence));
	CHECK(eq(C[m3], D[m3], Math::InternalTolerence));
	CHECK(eq(C[m4], D[m4], Math::InternalTolerence));
	CHECK(eq(C[m5], D[m5], Math::InternalTolerence));
	CHECK(eq(C[m6], D[m6], Math::InternalTolerence));
	CHECK(eq(C[m7], D[m7], Math::InternalTolerence));
	CHECK(eq(C[m8], D[m8], Math::InternalTolerence));
	CHECK(eq(C[m9], D[m9], Math::InternalTolerence));
	CHECK(eq(C[m10], D[m10], Math::InternalTolerence));
	CHECK(eq(C[m11], D[m11], Math::InternalTolerence));
	CHECK(eq(C[m12], D[m12], Math::InternalTolerence));
	CHECK(eq(C[m13], D[m13], Math::InternalTolerence));
	CHECK(eq(C[m14], D[m14], Math::InternalTolerence));
	CHECK(eq(C[m15], D[m15], Math::InternalTolerence));
}

// ---  End of File ---------------
