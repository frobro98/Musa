//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/Matrix4.hpp"

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Matrix_Identity, matrix_tests)
{

	Matrix4 A(IDENTITY);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 1.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 1.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 0.0f);
	CHECK_EQ(A[m13], 0.0f);
	CHECK_EQ(A[m14], 0.0f);
	CHECK_EQ(A[m15], 1.0f);
}

TEST(Matrix_Zero, matrix_tests)
{

	Matrix4 A(ZERO);

	CHECK_EQ(A[m0], 0.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 0.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 0.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 0.0f);
	CHECK_EQ(A[m13], 0.0f);
	CHECK_EQ(A[m14], 0.0f);
	CHECK_EQ(A[m15], 0.0f);
}

TEST(Matrix_Trans, matrix_tests)
{

	Matrix4 A(TRANS, 2.0f, 3.0f, 4.0f);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 1.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 1.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 2.0f);
	CHECK_EQ(A[m13], 3.0f);
	CHECK_EQ(A[m14], 4.0f);
	CHECK_EQ(A[m15], 1.0f);
}

TEST(Matrix_Trans_Vect, matrix_tests)
{
	Vector4 vTrans(2.0f, 3.0f, 4.0f);
	Matrix4 A(TRANS, vTrans);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 1.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 1.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 2.0f);
	CHECK_EQ(A[m13], 3.0f);
	CHECK_EQ(A[m14], 4.0f);
	CHECK_EQ(A[m15], 1.0f);
}

TEST(Matrix_SCALE, matrix_tests)
{

	Matrix4 A(SCALE, 2.0f, 3.0f, 4.0f);

	CHECK_EQ(A[m0], 2.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 3.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 4.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 0.0f);
	CHECK_EQ(A[m13], 0.0f);
	CHECK_EQ(A[m14], 0.0f);
	CHECK_EQ(A[m15], 1.0f);
}

TEST(Matrix_SCALE_Vect, matrix_tests)
{
	Vector4 vScale(2.0f, 3.0f, 4.0f);
	Matrix4 A(SCALE, vScale);

	CHECK_EQ(A[m0], 2.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 3.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 4.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 0.0f);
	CHECK_EQ(A[m13], 0.0f);
	CHECK_EQ(A[m14], 0.0f);
	CHECK_EQ(A[m15], 1.0f);
}

TEST(RotX, matrix_tests)
{
	// Rot_X Type Constructor:
	Matrix4 Rx(ROT_X, 1.0471975512f);	

	CHECK_EQ(Rx[m0],  1.0f);
	CHECK_EQ(Rx[m1],  0.0f);
	CHECK_EQ(Rx[m2],  0.0f);
	CHECK_EQ(Rx[m3],  0.0f);
	CHECK_EQ(Rx[m4],  0.0f);
	CHECK_EQ(Rx[m5],  0.5f);
	CHECK_EQ(Rx[m6], 0.866f);
	CHECK_EQ(Rx[m7], 0.0f);
	CHECK_EQ(Rx[m8], 0.0f);
	CHECK_EQ(Rx[m9],-0.866f);
	CHECK_EQ(Rx[m10],0.5f);
	CHECK_EQ(Rx[m11], 0.0f);
	CHECK_EQ(Rx[m12], 0.0f);
	CHECK_EQ(Rx[m13], 0.0f);
	CHECK_EQ(Rx[m14], 0.0f);
	CHECK_EQ(Rx[m15], 1.0f);
}


TEST(RotY, matrix_tests)
{
	Matrix4 Ry(ROT_Y,  1.0471975512f);	

	CHECK_EQ(Ry[m0],0.5f);
	CHECK_EQ(Ry[m1],  0.0f);
	CHECK_EQ(Ry[m2],-0.866f);
	CHECK_EQ(Ry[m3], 0.0f);
	CHECK_EQ(Ry[m4], 0.0f);
	CHECK_EQ(Ry[m5], 1.0f);
	CHECK_EQ(Ry[m6], 0.0f);
	CHECK_EQ(Ry[m7], 0.0f);
	CHECK_EQ(Ry[m8],0.866f);
	CHECK_EQ(Ry[m9],  0.0f);
	CHECK_EQ(Ry[m10],0.5f);
	CHECK_EQ(Ry[m11], 0.0f);
	CHECK_EQ(Ry[m12], 0.0f);
	CHECK_EQ(Ry[m13], 0.0f);
	CHECK_EQ(Ry[m14], 0.0f);
	CHECK_EQ(Ry[m15], 1.0f);
}

TEST(RotZ, matrix_tests)
{
	Matrix4 Rz(ROT_Z, 1.0471975512f);	

	CHECK_EQ(Rz[m0],0.5f);
	CHECK_EQ(Rz[m1],0.866f);
	CHECK_EQ(Rz[m2], 0.0f);
	CHECK_EQ(Rz[m3], 0.0f);
	CHECK_EQ(Rz[m4],-0.866f);
	CHECK_EQ(Rz[m5],0.5f);
	CHECK_EQ(Rz[m6], 0.0f);
	CHECK_EQ(Rz[m7], 0.0f);
	CHECK_EQ(Rz[m8], 0.0f);
	CHECK_EQ(Rz[m9], 0.0f);
	CHECK_EQ(Rz[m10], 1.0f);
	CHECK_EQ(Rz[m11], 0.0f);
	CHECK_EQ(Rz[m12], 0.0f);
	CHECK_EQ(Rz[m13], 0.0f);
	CHECK_EQ(Rz[m14], 0.0f);
	CHECK_EQ(Rz[m15], 1.0f);
}

TEST(RotXYZ_Constructor, matrix_tests)
{
	Matrix4 Rx;
	Matrix4 Ry;
	Matrix4 Rz;

	// Rot_X Type Constructor:
	Rx.Set(	ROT_X, Math::PiOver3);

	CHECK_EQ(Rx[m0],  1.0f);
	CHECK_EQ(Rx[m1],  0.0f);
	CHECK_EQ(Rx[m2],  0.0f);
	CHECK_EQ(Rx[m3],  0.0f);
	CHECK_EQ(Rx[m4],  0.0f);
	CHECK_EQ(Rx[m5],  0.5f);
	CHECK_EQ(Rx[m6], 0.866f);
	CHECK_EQ(Rx[m7], 0.0f);
	CHECK_EQ(Rx[m8], 0.0f);
	CHECK_EQ(Rx[m9],-0.866f);
	CHECK_EQ(Rx[m10],0.5f);
	CHECK_EQ(Rx[m11], 0.0f);
	CHECK_EQ(Rx[m12], 0.0f);
	CHECK_EQ(Rx[m13], 0.0f);
	CHECK_EQ(Rx[m14], 0.0f);
	CHECK_EQ(Rx[m15], 1.0f);

	Ry.Set(ROT_Y, Math::SevenPiOver8);

	CHECK_EQ(Ry[m0],-0.9238f);
	CHECK_EQ(Ry[m1],  0.0f);
	CHECK_EQ(Ry[m2],-0.3826f);
	CHECK_EQ(Ry[m3], 0.0f);
	CHECK_EQ(Ry[m4], 0.0f);
	CHECK_EQ(Ry[m5], 1.0f);
	CHECK_EQ(Ry[m6], 0.0f);
	CHECK_EQ(Ry[m7], 0.0f);
	CHECK_EQ(Ry[m8],0.3826f);
	CHECK_EQ(Ry[m9],  0.0f);
	CHECK_EQ(Ry[m10],-0.9238f);
	CHECK_EQ(Ry[m11], 0.0f);
	CHECK_EQ(Ry[m12], 0.0f);
	CHECK_EQ(Ry[m13], 0.0f);
	CHECK_EQ(Ry[m14], 0.0f);
	CHECK_EQ(Ry[m15], 1.0f);

	Rz.Set(ROT_Z, Math::PiOver2);

	CHECK_EQ(Rz[m0],0.0f);
	CHECK_EQ(Rz[m1],1.0f);
	CHECK_EQ(Rz[m2], 0.0f);
	CHECK_EQ(Rz[m3], 0.0f);
	CHECK_EQ(Rz[m4],-1.0f);
	CHECK_EQ(Rz[m5], 0.0f);
	CHECK_EQ(Rz[m6], 0.0f);
	CHECK_EQ(Rz[m7], 0.0f);
	CHECK_EQ(Rz[m8], 0.0f);
	CHECK_EQ(Rz[m9], 0.0f);
	CHECK_EQ(Rz[m10], 1.0f);
	CHECK_EQ(Rz[m11], 0.0f);
	CHECK_EQ(Rz[m12], 0.0f);
	CHECK_EQ(Rz[m13], 0.0f);
	CHECK_EQ(Rz[m14], 0.0f);
	CHECK_EQ(Rz[m15], 1.0f);

	Matrix4 Rxyz;

	Rxyz = Rx * Ry * Rz;

	Matrix4 mTmp(ROT_XYZ, Math::PiOver3,Math::SevenPiOver8, Math::PiOver2);

	CHECK_EQ( Rxyz[m0], mTmp[m0]);
	CHECK_EQ( Rxyz[m1], mTmp[m1]);
	CHECK_EQ( Rxyz[m2], mTmp[m2]);
	CHECK_EQ( Rxyz[m3], mTmp[m3]);
	CHECK_EQ( Rxyz[m4], mTmp[m4]);
	CHECK_EQ( Rxyz[m5], mTmp[m5]);
	CHECK_EQ( Rxyz[m6], mTmp[m6]);
	CHECK_EQ( Rxyz[m7], mTmp[m7]);
	CHECK_EQ( Rxyz[m8], mTmp[m8]);
	CHECK_EQ( Rxyz[m9], mTmp[m9]);
	CHECK_EQ( Rxyz[m10], mTmp[m10]);
	CHECK_EQ( Rxyz[m11], mTmp[m11]);
	CHECK_EQ( Rxyz[m12], mTmp[m12]);
	CHECK_EQ( Rxyz[m13], mTmp[m13]);
	CHECK_EQ( Rxyz[m14], mTmp[m14]);
	CHECK_EQ( Rxyz[m15], mTmp[m15]);
}

TEST(isEqual_false , matrix_tests)
{
	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 2.0f);
	CHECK_EQ(A[m2], 3.0f);
	CHECK_EQ(A[m3], 4.0f);
	CHECK_EQ(A[m4], 5.0f);
	CHECK_EQ(A[m5], 6.0f);
	CHECK_EQ(A[m6], 7.0f);	
	CHECK_EQ(A[m7], 8.0f);
	CHECK_EQ(A[m8], 9.0f);
	CHECK_EQ(A[m9], 10.0f);
	CHECK_EQ(A[m10], 11.0f);
	CHECK_EQ(A[m11], 12.0f);
	CHECK_EQ(A[m12], 13.0f);
	CHECK_EQ(A[m13], 14.0f);
	CHECK_EQ(A[m14], 15.0f);
	CHECK_EQ(A[m15], 16.0f);

	Vector4 V4(20.0f,21.0f,22.0f,23.0f);
	Vector4 V5(24.0f,25.0f,26.0f,27.0f);
	Vector4 V6(28.0f,29.0f,30.0f,31.0f);
	Vector4 V7(32.0f,33.0f,34.0f,35.0f);

	Matrix4 B(V4,V5,V6,V7);

	CHECK_EQ(B[m0], 20.0f);
	CHECK_EQ(B[m1], 21.0f);
	CHECK_EQ(B[m2], 22.0f);
	CHECK_EQ(B[m3], 23.0f);
	CHECK_EQ(B[m4], 24.0f);
	CHECK_EQ(B[m5], 25.0f);
	CHECK_EQ(B[m6], 26.0f);	
	CHECK_EQ(B[m7], 27.0f);
	CHECK_EQ(B[m8], 28.0f);
	CHECK_EQ(B[m9], 29.0f);
	CHECK_EQ(B[m10], 30.0f);
	CHECK_EQ(B[m11], 31.0f);
	CHECK_EQ(B[m12], 32.0f);
	CHECK_EQ(B[m13], 33.0f);
	CHECK_EQ(B[m14], 34.0f);
	CHECK_EQ(B[m15], 35.0f);

	bool value = A.IsEqual(B);

	CHECK_EQ(value, false);
}

TEST(isEqual_true, matrix_tests)
{
	Vector4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vector4 V3(13.0f, 14.0f, 15.0f, 16.0f);

	Matrix4 A(V0, V1, V2, V3);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 2.0f);
	CHECK_EQ(A[m2], 3.0f);
	CHECK_EQ(A[m3], 4.0f);
	CHECK_EQ(A[m4], 5.0f);
	CHECK_EQ(A[m5], 6.0f);
	CHECK_EQ(A[m6], 7.0f);
	CHECK_EQ(A[m7], 8.0f);
	CHECK_EQ(A[m8], 9.0f);
	CHECK_EQ(A[m9], 10.0f);
	CHECK_EQ(A[m10], 11.0f);
	CHECK_EQ(A[m11], 12.0f);
	CHECK_EQ(A[m12], 13.0f);
	CHECK_EQ(A[m13], 14.0f);
	CHECK_EQ(A[m14], 15.0f);
	CHECK_EQ(A[m15], 16.0f);

	Vector4 V4(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 V5(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 V6(9.0f, 10.0f, 11.0f, 12.0f);
	Vector4 V7(13.0f, 14.0f, 15.0f, 16.0f);

	Matrix4 B(V4, V5, V6, V7);

	CHECK_EQ(B[m0], 1.0f);
	CHECK_EQ(B[m1], 2.0f);
	CHECK_EQ(B[m2], 3.0f);
	CHECK_EQ(B[m3], 4.0f);
	CHECK_EQ(B[m4], 5.0f);
	CHECK_EQ(B[m5], 6.0f);
	CHECK_EQ(B[m6], 7.0f);
	CHECK_EQ(B[m7], 8.0f);
	CHECK_EQ(B[m8], 9.0f);
	CHECK_EQ(B[m9], 10.0f);
	CHECK_EQ(B[m10], 11.0f);
	CHECK_EQ(B[m11], 12.0f);
	CHECK_EQ(B[m12], 13.0f);
	CHECK_EQ(B[m13], 14.0f);
	CHECK_EQ(B[m14], 15.0f);
	CHECK_EQ(B[m15], 16.0f);

	bool value = A.IsEqual(B);

	CHECK_EQ(value, true);
}

TEST(isIdentity_false , matrix_tests)
{
	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 2.0f);
	CHECK_EQ(A[m2], 3.0f);
	CHECK_EQ(A[m3], 4.0f);
	CHECK_EQ(A[m4], 5.0f);
	CHECK_EQ(A[m5], 6.0f);
	CHECK_EQ(A[m6], 7.0f);	
	CHECK_EQ(A[m7], 8.0f);
	CHECK_EQ(A[m8], 9.0f);
	CHECK_EQ(A[m9], 10.0f);
	CHECK_EQ(A[m10], 11.0f);
	CHECK_EQ(A[m11], 12.0f);
	CHECK_EQ(A[m12], 13.0f);
	CHECK_EQ(A[m13], 14.0f);
	CHECK_EQ(A[m14], 15.0f);
	CHECK_EQ(A[m15], 16.0f);

	bool value = A.IsIdentity();

	CHECK_EQ(value, false);
}

TEST(isIdentity_true , matrix_tests)
{
	Vector4 V0(1.0f,0.0f,0.0f,0.0f);
	Vector4 V1(0.0f,1.0f,0.0f,0.0f);
	Vector4 V2(0.0f,0.0f,1.0f,0.0f);
	Vector4 V3(0.0f,0.0f,0.0f,1.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 0.0f);
	CHECK_EQ(A[m2], 0.0f);
	CHECK_EQ(A[m3], 0.0f);
	CHECK_EQ(A[m4], 0.0f);
	CHECK_EQ(A[m5], 1.0f);
	CHECK_EQ(A[m6], 0.0f);	
	CHECK_EQ(A[m7], 0.0f);
	CHECK_EQ(A[m8], 0.0f);
	CHECK_EQ(A[m9], 0.0f);
	CHECK_EQ(A[m10], 1.0f);
	CHECK_EQ(A[m11], 0.0f);
	CHECK_EQ(A[m12], 0.0f);
	CHECK_EQ(A[m13], 0.0f);
	CHECK_EQ(A[m14], 0.0f);
	CHECK_EQ(A[m15], 1.0f);

	bool value = A.IsIdentity();

	CHECK_EQ(value, true);
}

// ---  End of File ---------------
