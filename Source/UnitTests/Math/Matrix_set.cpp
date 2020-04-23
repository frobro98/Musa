//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/Matrix4.hpp"
#include "Math/Quat.hpp"

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Matrix_SET_Identity, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	A.Set(IDENTITY);

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

TEST(Matrix_SET_Zero, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	A.Set(ZERO);

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

TEST(Matrix_SET_Trans, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	A.Set(TRANS, 2.0f, 3.0f, 4.0f);

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

TEST(Matrix_SET_Trans_Vect, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	Vector4 vTrans(2.0f, 3.0f, 4.0f);
	A.Set(TRANS, vTrans);

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

TEST(Matrix_SET_SCALE, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	A.Set(SCALE, 2.0f, 3.0f, 4.0f);

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

TEST(Matrix_SET_SCALE_Vect, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);

	CHECK_EQ(A[m0], 17.0f);
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

	Vector4 vScale(2.0f, 3.0f, 4.0f);
	A.Set(SCALE, vScale);

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

TEST(RotX_SET, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 Rx(V0,V1,V2,V3);

	CHECK_EQ(Rx[m0], 17.0f);
	CHECK_EQ(Rx[m1], 2.0f);
	CHECK_EQ(Rx[m2], 3.0f);
	CHECK_EQ(Rx[m3], 4.0f);
	CHECK_EQ(Rx[m4], 5.0f);
	CHECK_EQ(Rx[m5], 6.0f);
	CHECK_EQ(Rx[m6], 7.0f);	
	CHECK_EQ(Rx[m7], 8.0f);
	CHECK_EQ(Rx[m8], 9.0f);
	CHECK_EQ(Rx[m9], 10.0f);
	CHECK_EQ(Rx[m10], 11.0f);
	CHECK_EQ(Rx[m11], 12.0f);
	CHECK_EQ(Rx[m12], 13.0f);
	CHECK_EQ(Rx[m13], 14.0f);
	CHECK_EQ(Rx[m14], 15.0f);
	CHECK_EQ(Rx[m15], 16.0f);

	// Rot_X Type Constructor:
	Rx.Set(ROT_X, 1.0471975512f);	

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

TEST(RotY_SET, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 Ry(V0,V1,V2,V3);

	CHECK_EQ(Ry[m0], 17.0f);
	CHECK_EQ(Ry[m1], 2.0f);
	CHECK_EQ(Ry[m2], 3.0f);
	CHECK_EQ(Ry[m3], 4.0f);
	CHECK_EQ(Ry[m4], 5.0f);
	CHECK_EQ(Ry[m5], 6.0f);
	CHECK_EQ(Ry[m6], 7.0f);	
	CHECK_EQ(Ry[m7], 8.0f);
	CHECK_EQ(Ry[m8], 9.0f);
	CHECK_EQ(Ry[m9], 10.0f);
	CHECK_EQ(Ry[m10], 11.0f);
	CHECK_EQ(Ry[m11], 12.0f);
	CHECK_EQ(Ry[m12], 13.0f);
	CHECK_EQ(Ry[m13], 14.0f);
	CHECK_EQ(Ry[m14], 15.0f);
	CHECK_EQ(Ry[m15], 16.0f);

	Ry.Set(ROT_Y,  1.0471975512f);	

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

TEST(RotZ_SET, matrix_tests)
{
	Vector4 V0(17.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 Rz(V0,V1,V2,V3);

	CHECK_EQ(Rz[m0], 17.0f);
	CHECK_EQ(Rz[m1], 2.0f);
	CHECK_EQ(Rz[m2], 3.0f);
	CHECK_EQ(Rz[m3], 4.0f);
	CHECK_EQ(Rz[m4], 5.0f);
	CHECK_EQ(Rz[m5], 6.0f);
	CHECK_EQ(Rz[m6], 7.0f);	
	CHECK_EQ(Rz[m7], 8.0f);
	CHECK_EQ(Rz[m8], 9.0f);
	CHECK_EQ(Rz[m9], 10.0f);
	CHECK_EQ(Rz[m10], 11.0f);
	CHECK_EQ(Rz[m11], 12.0f);
	CHECK_EQ(Rz[m12], 13.0f);
	CHECK_EQ(Rz[m13], 14.0f);
	CHECK_EQ(Rz[m14], 15.0f);
	CHECK_EQ(Rz[m15], 16.0f);

	Rz.Set(ROT_Z, 1.0471975512f);	

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

TEST(set_ROW_0, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_0, V);

	CHECK_EQ(A[m0], 20.0f);
	CHECK_EQ(A[m1], 30.0f);
	CHECK_EQ(A[m2], 40.0f);
	CHECK_EQ(A[m3], 50.0f);
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
}

TEST(set_ROW_1, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_1, V);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 2.0f);
	CHECK_EQ(A[m2], 3.0f);
	CHECK_EQ(A[m3], 4.0f);
	CHECK_EQ(A[m4], 20.0f);
	CHECK_EQ(A[m5], 30.0f);
	CHECK_EQ(A[m6], 40.0f);	
	CHECK_EQ(A[m7], 50.0f);
	CHECK_EQ(A[m8], 9.0f);
	CHECK_EQ(A[m9], 10.0f);
	CHECK_EQ(A[m10], 11.0f);
	CHECK_EQ(A[m11], 12.0f);
	CHECK_EQ(A[m12], 13.0f);
	CHECK_EQ(A[m13], 14.0f);
	CHECK_EQ(A[m14], 15.0f);
	CHECK_EQ(A[m15], 16.0f);
}

TEST(set_ROW_2, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_2, V);

	CHECK_EQ(A[m0], 1.0f);
	CHECK_EQ(A[m1], 2.0f);
	CHECK_EQ(A[m2], 3.0f);
	CHECK_EQ(A[m3], 4.0f);
	CHECK_EQ(A[m4], 5.0f);
	CHECK_EQ(A[m5], 6.0f);
	CHECK_EQ(A[m6], 7.0f);	
	CHECK_EQ(A[m7], 8.0f);
	CHECK_EQ(A[m8], 20.0f);
	CHECK_EQ(A[m9], 30.0f);
	CHECK_EQ(A[m10], 40.0f);
	CHECK_EQ(A[m11], 50.0f);
	CHECK_EQ(A[m12], 13.0f);
	CHECK_EQ(A[m13], 14.0f);
	CHECK_EQ(A[m14], 15.0f);
	CHECK_EQ(A[m15], 16.0f);
}

TEST(set_ROW_3, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_3, V);

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
	CHECK_EQ(A[m12], 20.0f);
	CHECK_EQ(A[m13], 30.0f);
	CHECK_EQ(A[m14], 40.0f);
	CHECK_EQ(A[m15], 50.0f);
}

TEST(get_ROW_0, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_0);

	CHECK_EQ(V.x, 1.0f);
	CHECK_EQ(V.y, 2.0f);
	CHECK_EQ(V.z, 3.0f);
	CHECK_EQ(V.w, 4.0f);

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
}

TEST(get_ROW_1, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_1);

	CHECK_EQ(V.x, 5.0f);
	CHECK_EQ(V.y, 6.0f);
	CHECK_EQ(V.z, 7.0f);
	CHECK_EQ(V.w, 8.0f);

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
}

TEST(get_ROW_2, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_2);

	CHECK_EQ(V.x, 9.0f);
	CHECK_EQ(V.y, 10.0f);
	CHECK_EQ(V.z, 11.0f);
	CHECK_EQ(V.w, 12.0f);

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
}

TEST(get_ROW_3, matrix_tests)
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

	Vector4 V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_3);

	CHECK_EQ(V.x, 13.0f);
	CHECK_EQ(V.y, 14.0f);
	CHECK_EQ(V.z, 15.0f);
	CHECK_EQ(V.w, 16.0f);

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
}

TEST(set_Vectors, matrix_tests)
{
	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

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

	A.Set(V0,V1,V2,V3);

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
}

TEST(RotXYZ_SET, matrix_tests)
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

	Matrix4 mTmp;

	mTmp.Set(ROT_XYZ, Math::PiOver3,Math::SevenPiOver8, Math::PiOver2);

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

TEST(MatrixRotAxisAngle_set, matrix_tests)
{
	// Axis and Angle Type Constructor:
	Vector4  v11(2.0f, 53.0f, 24.0f);
	Matrix4 m54;
	
	m54.Set(ROT_AXIS_ANGLE, v11, Math::PiOver3);
			
	// => Vector  v11(2.0f, 53.0f, 24.0f); \n"););
	// => Matrix m54(ROT_AXIS_ANGLE, v11, MATH_PI3);\n"););
	CHECK_EQ(m54[m0], 0.5005f);
	CHECK_EQ(m54[m1], 0.3726f);
	CHECK_EQ(m54[m2],-0.7813f);
	CHECK_EQ(m54[m3], 0.0f);
	CHECK_EQ(m54[m4],-0.3413f);
	CHECK_EQ(m54[m5], 0.9144f);
	CHECK_EQ(m54[m6], 0.2174f);
	CHECK_EQ(m54[m7], 0.0f);
	CHECK_EQ(m54[m8], 0.7955f);
	CHECK_EQ(m54[m9], 0.1579f);
	CHECK_EQ(m54[m10], 0.5849f);
	CHECK_EQ(m54[m11], 0.0f);
	CHECK_EQ(m54[m12], 0.0f);
	CHECK_EQ(m54[m13], 0.0f);
	CHECK_EQ(m54[m14], 0.0f);
	CHECK_EQ(m54[m15], 1.0f);
}

TEST(MatrixRotOrient_set, matrix_tests)
{
	// Orientation Type Constructor:
	Vector4  v15(2.0f, 53.0f, 24.0f);
	Vector4  v16(0.0f, -24.0f, 53.0f);
	Matrix4 m56;
	m56.Set(ROT_ORIENT, v15, v16);

	CHECK_EQ(m56[m0],-0.9994f);
	CHECK_EQ(m56[m1], 0.0313f);
	CHECK_EQ(m56[m2], 0.0142f);
	CHECK_EQ(m56[m3], 0.0f);
	CHECK_EQ(m56[m4], 0.0000f);
	CHECK_EQ(m56[m5],-0.4125f);
	CHECK_EQ(m56[m6], 0.9110f);
	CHECK_EQ(m56[m7], 0.0f);
	CHECK_EQ(m56[m8], 0.0344f);
	CHECK_EQ(m56[m9], 0.9104f);
	CHECK_EQ(m56[m10], 0.4123f);
	CHECK_EQ(m56[m11], 0.0f);
	CHECK_EQ(m56[m12], 0.0f);
	CHECK_EQ(m56[m13], 0.0f);
	CHECK_EQ(m56[m14], 0.0f);
	CHECK_EQ(m56[m15], 1.0f);
}

TEST(MatrixRotInverseOrient_set,matrix_tests)
{
	// Orientation Type Constructor:
	Vector4  v17(2.0f, 53.0f, 24.0f);
	Vector4  v18(0.0f, -24.0f, 53.0f);
	Matrix4 m57;
	m57.Set(ROT_INVERSE_ORIENT, v17, v18);

	CHECK_EQ(m57[m0],-0.9994f);
	CHECK_EQ(m57[m1], 0.0000f);
	CHECK_EQ(m57[m2], 0.0344f);
	CHECK_EQ(m57[m3], 0.0f);
	CHECK_EQ(m57[m4], 0.0313f);
	CHECK_EQ(m57[m5],-0.4125f);
	CHECK_EQ(m57[m6], 0.9104f);
	CHECK_EQ(m57[m7], 0.0f);
	CHECK_EQ(m57[m8], 0.0142f);
	CHECK_EQ(m57[m9], 0.9110f);
	CHECK_EQ(m57[m10], 0.4123f);
	CHECK_EQ(m57[m11], 0.0f);
	CHECK_EQ(m57[m12], 0.0f);
	CHECK_EQ(m57[m13], 0.0f);
	CHECK_EQ(m57[m14], 0.0f);
	CHECK_EQ(m57[m15], 1.0f);
}

TEST(MatrixQuaternion_set, matrix_tests)
{
	// Quaternion Type Constructor:
	Matrix4 Rxyz1(ROT_XYZ, Math::PiOver3, Math::FivePiOver8, Math::PiOver4);

	Quat	Qxyz1(Rxyz1);
	Matrix4 Mxyz1;
	Mxyz1.Set(Qxyz1);

	CHECK_EQ(Mxyz1[m0],-0.2705f);
	CHECK_EQ(Mxyz1[m1],-0.2705f);
	CHECK_EQ(Mxyz1[m2],-0.9238f);
	CHECK_EQ(Mxyz1[m3], 0.0f);
	CHECK_EQ(Mxyz1[m4], 0.2122f);
	CHECK_EQ(Mxyz1[m5], 0.9193f);
	CHECK_EQ(Mxyz1[m6],-0.3314f);
	CHECK_EQ(Mxyz1[m7], 0.0f);
	CHECK_EQ(Mxyz1[m8], 0.9390f);
	CHECK_EQ(Mxyz1[m9],-0.2857f);
	CHECK_EQ(Mxyz1[m10],-0.1913f);
	CHECK_EQ(Mxyz1[m11], 0.0f);
	CHECK_EQ(Mxyz1[m12], 0.0f);
	CHECK_EQ(Mxyz1[m13], 0.0f);
	CHECK_EQ(Mxyz1[m14], 0.0f);
	CHECK_EQ(Mxyz1[m15], 1.0f);
}

// ---  End of File ---------------
