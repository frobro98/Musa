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

TEST( Matrix_SET_Identity, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.Set(IDENTITY);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 1.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 1.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 1.0f );
}

TEST( Matrix_SET_Zero, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.Set(ZERO);

	CHECK( A[m0] == 0.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 0.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 0.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 0.0f );
}

TEST( Matrix_SET_Trans, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.Set(TRANS, 2.0f, 3.0f, 4.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 1.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 1.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 2.0f );
	CHECK( A[m13] == 3.0f );
	CHECK( A[m14] == 4.0f );
	CHECK( A[m15] == 1.0f );
}

TEST(Matrix_SET_Trans_Vect, matrix_tests)
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector vTrans(2.0f, 3.0f, 4.0f);
	A.Set(TRANS, vTrans);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 1.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 1.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 2.0f );
	CHECK( A[m13] == 3.0f );
	CHECK( A[m14] == 4.0f );
	CHECK( A[m15] == 1.0f );
}

TEST( Matrix_SET_SCALE, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	A.Set(SCALE, 2.0f, 3.0f, 4.0f);

	CHECK( A[m0] == 2.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 3.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 4.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 1.0f );
}

TEST(Matrix_SET_SCALE_Vect, matrix_tests)
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 17.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector vScale(2.0f, 3.0f, 4.0f);
	A.Set(SCALE, vScale);

	CHECK( A[m0] == 2.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 3.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 4.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 1.0f );
}

TEST( RotX_SET, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Rx(V0,V1,V2,V3);

	CHECK( Rx[m0] == 17.0f );
	CHECK( Rx[m1] == 2.0f );
	CHECK( Rx[m2] == 3.0f );
	CHECK( Rx[m3] == 4.0f );
	CHECK( Rx[m4] == 5.0f );
	CHECK( Rx[m5] == 6.0f );
	CHECK( Rx[m6] == 7.0f );	
	CHECK( Rx[m7] == 8.0f );
	CHECK( Rx[m8] == 9.0f );
	CHECK( Rx[m9] == 10.0f );
	CHECK( Rx[m10] == 11.0f );
	CHECK( Rx[m11] == 12.0f );
	CHECK( Rx[m12] == 13.0f );
	CHECK( Rx[m13] == 14.0f );
	CHECK( Rx[m14] == 15.0f );
	CHECK( Rx[m15] == 16.0f );

	// Rot_X Type Constructor:
	Rx.Set(ROT_X, 1.0471975512f );	

	CHECK( Rx[m0] ==  1.0f );
	CHECK( Rx[m1] ==  0.0f );
	CHECK( Rx[m2] ==  0.0f );
	CHECK( Rx[m3] ==  0.0f );
	CHECK( Rx[m4] ==  0.0f );
	CHECK( eq(Rx[m5],  0.5f,  Math::InternalTolerence) );
	CHECK( eq(Rx[m6], 0.866f, Math::InternalTolerence) );
	CHECK( Rx[m7] == 0.0f );
	CHECK( Rx[m8] == 0.0f );
	CHECK( eq(Rx[m9],-0.866f, Math::InternalTolerence) );
	CHECK( eq(Rx[m10],0.5f  , Math::InternalTolerence) );
	CHECK( Rx[m11] == 0.0f );
	CHECK( Rx[m12] == 0.0f );
	CHECK( Rx[m13] == 0.0f );
	CHECK( Rx[m14] == 0.0f );
	CHECK( Rx[m15] == 1.0f );
}

TEST( RotY_SET, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Ry(V0,V1,V2,V3);

	CHECK( Ry[m0] == 17.0f );
	CHECK( Ry[m1] == 2.0f );
	CHECK( Ry[m2] == 3.0f );
	CHECK( Ry[m3] == 4.0f );
	CHECK( Ry[m4] == 5.0f );
	CHECK( Ry[m5] == 6.0f );
	CHECK( Ry[m6] == 7.0f );	
	CHECK( Ry[m7] == 8.0f );
	CHECK( Ry[m8] == 9.0f );
	CHECK( Ry[m9] == 10.0f );
	CHECK( Ry[m10] == 11.0f );
	CHECK( Ry[m11] == 12.0f );
	CHECK( Ry[m12] == 13.0f );
	CHECK( Ry[m13] == 14.0f );
	CHECK( Ry[m14] == 15.0f );
	CHECK( Ry[m15] == 16.0f );

	Ry.Set(ROT_Y,  1.0471975512f );	

	CHECK( eq(Ry[m0],0.5f,  Math::InternalTolerence) );
	CHECK( Ry[m1] ==  0.0f );
	CHECK( eq(Ry[m2],-0.866f,Math::InternalTolerence) );
	CHECK( Ry[m3] == 0.0f );
	CHECK( Ry[m4] == 0.0f );
	CHECK( Ry[m5] == 1.0f );
	CHECK( Ry[m6] == 0.0f );
	CHECK( Ry[m7] == 0.0f );
	CHECK( eq(Ry[m8],0.866f,Math::InternalTolerence) );
	CHECK( Ry[m9] ==  0.0f );
	CHECK( eq(Ry[m10],0.5f,   Math::InternalTolerence) );
	CHECK( Ry[m11] == 0.0f );
	CHECK( Ry[m12] == 0.0f );
	CHECK( Ry[m13] == 0.0f );
	CHECK( Ry[m14] == 0.0f );
	CHECK( Ry[m15] == 1.0f );
}

TEST( RotZ_SET, matrix_tests )
{
	Vector V0(17.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix Rz(V0,V1,V2,V3);

	CHECK( Rz[m0] == 17.0f );
	CHECK( Rz[m1] == 2.0f );
	CHECK( Rz[m2] == 3.0f );
	CHECK( Rz[m3] == 4.0f );
	CHECK( Rz[m4] == 5.0f );
	CHECK( Rz[m5] == 6.0f );
	CHECK( Rz[m6] == 7.0f );	
	CHECK( Rz[m7] == 8.0f );
	CHECK( Rz[m8] == 9.0f );
	CHECK( Rz[m9] == 10.0f );
	CHECK( Rz[m10] == 11.0f );
	CHECK( Rz[m11] == 12.0f );
	CHECK( Rz[m12] == 13.0f );
	CHECK( Rz[m13] == 14.0f );
	CHECK( Rz[m14] == 15.0f );
	CHECK( Rz[m15] == 16.0f );

	Rz.Set(ROT_Z, 1.0471975512f);	

	CHECK( eq(Rz[m0],0.5f,   Math::InternalTolerence) );
	CHECK( eq(Rz[m1],0.866f,Math::InternalTolerence) );
	CHECK( Rz[m2] == 0.0f );
	CHECK( Rz[m3] == 0.0f );
	CHECK( eq(Rz[m4],-0.866f,Math::InternalTolerence) );
	CHECK( eq(Rz[m5],0.5f,  Math::InternalTolerence) );
	CHECK( Rz[m6] == 0.0f );
	CHECK( Rz[m7] == 0.0f );
	CHECK( Rz[m8] == 0.0f );
	CHECK( Rz[m9] == 0.0f );
	CHECK( Rz[m10] == 1.0f );
	CHECK( Rz[m11] == 0.0f );
	CHECK( Rz[m12] == 0.0f );
	CHECK( Rz[m13] == 0.0f );
	CHECK( Rz[m14] == 0.0f );
	CHECK( Rz[m15] == 1.0f );
}

TEST( set_ROW_0, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_0, V);

	CHECK( A[m0] == 20.0f );
	CHECK( A[m1] == 30.0f );
	CHECK( A[m2] == 40.0f );
	CHECK( A[m3] == 50.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_ROW_1, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_1, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 20.0f );
	CHECK( A[m5] == 30.0f );
	CHECK( A[m6] == 40.0f );	
	CHECK( A[m7] == 50.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_ROW_2, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_2, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 20.0f );
	CHECK( A[m9] == 30.0f );
	CHECK( A[m10] == 40.0f );
	CHECK( A[m11] == 50.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_ROW_3, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	A.Set(ROW_3, V);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 20.0f );
	CHECK( A[m13] == 30.0f );
	CHECK( A[m14] == 40.0f );
	CHECK( A[m15] == 50.0f );
}

TEST( get_ROW_0, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_0);

	CHECK( V.x == 1.0f);
	CHECK( V.y == 2.0f);
	CHECK( V.z == 3.0f);
	CHECK( V.w == 4.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( get_ROW_1, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_1);

	CHECK( V.x == 5.0f);
	CHECK( V.y == 6.0f);
	CHECK( V.z == 7.0f);
	CHECK( V.w == 8.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( get_ROW_2, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_2);

	CHECK( V.x == 9.0f);
	CHECK( V.y == 10.0f);
	CHECK( V.z == 11.0f);
	CHECK( V.w == 12.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( get_ROW_3, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );

	Vector V(20.0f, 30.0f, 40.0, 50.0f);

	V = A.Get(ROW_3);

	CHECK( V.x == 13.0f);
	CHECK( V.y == 14.0f);
	CHECK( V.z == 15.0f);
	CHECK( V.w == 16.0f);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( set_Vectors, matrix_tests )
{
	Vector V0(1.0f,2.0f,3.0f,4.0f);
	Vector V1(5.0f,6.0f,7.0f,8.0f);
	Vector V2(9.0f,10.0f,11.0f,12.0f);
	Vector V3(13.0f,14.0f,15.0f,16.0f);

	Matrix A(ZERO);

	CHECK( A[m0] == 0.0f );
	CHECK( A[m1] == 0.0f );
	CHECK( A[m2] == 0.0f );
	CHECK( A[m3] == 0.0f );
	CHECK( A[m4] == 0.0f );
	CHECK( A[m5] == 0.0f );
	CHECK( A[m6] == 0.0f );	
	CHECK( A[m7] == 0.0f );
	CHECK( A[m8] == 0.0f );
	CHECK( A[m9] == 0.0f );
	CHECK( A[m10] == 0.0f );
	CHECK( A[m11] == 0.0f );
	CHECK( A[m12] == 0.0f );
	CHECK( A[m13] == 0.0f );
	CHECK( A[m14] == 0.0f );
	CHECK( A[m15] == 0.0f );

	A.Set(V0,V1,V2,V3);

	CHECK( A[m0] == 1.0f );
	CHECK( A[m1] == 2.0f );
	CHECK( A[m2] == 3.0f );
	CHECK( A[m3] == 4.0f );
	CHECK( A[m4] == 5.0f );
	CHECK( A[m5] == 6.0f );
	CHECK( A[m6] == 7.0f );	
	CHECK( A[m7] == 8.0f );
	CHECK( A[m8] == 9.0f );
	CHECK( A[m9] == 10.0f );
	CHECK( A[m10] == 11.0f );
	CHECK( A[m11] == 12.0f );
	CHECK( A[m12] == 13.0f );
	CHECK( A[m13] == 14.0f );
	CHECK( A[m14] == 15.0f );
	CHECK( A[m15] == 16.0f );
}

TEST( RotXYZ_SET, matrix_tests )
{
	Matrix Rx;
	Matrix Ry;
	Matrix Rz;

	// Rot_X Type Constructor:
	Rx.Set(	ROT_X, Math::PiOver3);

	CHECK( Rx[m0] ==  1.0f );
	CHECK( Rx[m1] ==  0.0f );
	CHECK( Rx[m2] ==  0.0f );
	CHECK( Rx[m3] ==  0.0f );
	CHECK( Rx[m4] ==  0.0f );
	CHECK( eq(Rx[m5],  0.5f,  Math::InternalTolerence) );
	CHECK( eq(Rx[m6], 0.866f, Math::InternalTolerence) );
	CHECK( Rx[m7] == 0.0f );
	CHECK( Rx[m8] == 0.0f );
	CHECK( eq(Rx[m9],-0.866f, Math::InternalTolerence) );
	CHECK( eq(Rx[m10],0.5f  , Math::InternalTolerence) );
	CHECK( Rx[m11] == 0.0f );
	CHECK( Rx[m12] == 0.0f );
	CHECK( Rx[m13] == 0.0f );
	CHECK( Rx[m14] == 0.0f );
	CHECK( Rx[m15] == 1.0f );

	Ry.Set(ROT_Y, Math::SevenPiOver8);

	CHECK( eq(Ry[m0],-0.9238f,  Math::InternalTolerence) );
	CHECK( Ry[m1] ==  0.0f );
	CHECK( eq(Ry[m2],-0.3826f,Math::InternalTolerence) );
	CHECK( Ry[m3] == 0.0f );
	CHECK( Ry[m4] == 0.0f );
	CHECK( Ry[m5] == 1.0f );
	CHECK( Ry[m6] == 0.0f );
	CHECK( Ry[m7] == 0.0f );
	CHECK( eq(Ry[m8],0.3826f,Math::InternalTolerence) );
	CHECK( Ry[m9] ==  0.0f );
	CHECK( eq(Ry[m10],-0.9238f,   Math::InternalTolerence) );
	CHECK( Ry[m11] == 0.0f );
	CHECK( Ry[m12] == 0.0f );
	CHECK( Ry[m13] == 0.0f );
	CHECK( Ry[m14] == 0.0f );
	CHECK( Ry[m15] == 1.0f );

	Rz.Set(ROT_Z, Math::PiOver2);

	CHECK( eq(Rz[m0],0.0f, Math::InternalTolerence) );
	CHECK( eq(Rz[m1],1.0f, Math::InternalTolerence) );
	CHECK( Rz[m2] == 0.0f );
	CHECK( Rz[m3] == 0.0f );
	CHECK( eq(Rz[m4],-1.0f, Math::InternalTolerence) );
	CHECK( eq(Rz[m5], 0.0f,  Math::InternalTolerence) );
	CHECK( Rz[m6] == 0.0f );
	CHECK( Rz[m7] == 0.0f );
	CHECK( Rz[m8] == 0.0f );
	CHECK( Rz[m9] == 0.0f );
	CHECK( Rz[m10] == 1.0f );
	CHECK( Rz[m11] == 0.0f );
	CHECK( Rz[m12] == 0.0f );
	CHECK( Rz[m13] == 0.0f );
	CHECK( Rz[m14] == 0.0f );
	CHECK( Rz[m15] == 1.0f );

	Matrix Rxyz;

	Rxyz = Rx * Ry * Rz;

	Matrix mTmp;

	mTmp.Set(ROT_XYZ, Math::PiOver3,Math::SevenPiOver8, Math::PiOver2);

	CHECK( eq( Rxyz[m0], mTmp[m0], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m1], mTmp[m1], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m2], mTmp[m2], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m3], mTmp[m3], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m4], mTmp[m4], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m5], mTmp[m5], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m6], mTmp[m6], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m7], mTmp[m7], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m8], mTmp[m8], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m9], mTmp[m9], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m10], mTmp[m10], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m11], mTmp[m11], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m12], mTmp[m12], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m13], mTmp[m13], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m14], mTmp[m14], Math::InternalTolerence) );
	CHECK( eq( Rxyz[m15], mTmp[m15], Math::InternalTolerence) );
}

TEST( MatrixRotAxisAngle_set, matrix_tests )
{
	// Axis and Angle Type Constructor:
	Vector  v11( 2.0f, 53.0f, 24.0f);
	Matrix m54;
	
	m54.Set( ROT_AXIS_ANGLE, v11, Math::PiOver3 );
			
	// => Vector  v11( 2.0f, 53.0f, 24.0f); \n"););
	// => Matrix m54(ROT_AXIS_ANGLE, v11, MATH_PI3 );\n"););
	CHECK( eq(m54[m0], 0.5005f, Math::InternalTolerence) );
	CHECK( eq(m54[m1], 0.3726f, Math::InternalTolerence) );
	CHECK( eq(m54[m2],-0.7813f, Math::InternalTolerence) );
	CHECK( m54[m3] == 0.0f );
	CHECK( eq(m54[m4],-0.3413f, Math::InternalTolerence) );
	CHECK( eq(m54[m5], 0.9144f, Math::InternalTolerence) );
	CHECK( eq(m54[m6], 0.2174f, Math::InternalTolerence) );
	CHECK( (m54[m7] == 0.0f) );
	CHECK( eq(m54[m8], 0.7955f, Math::InternalTolerence) );
	CHECK( eq(m54[m9], 0.1579f, Math::InternalTolerence) );
	CHECK( eq(m54[m10], 0.5849f, Math::InternalTolerence) );
	CHECK( (m54[m11] == 0.0f) );
	CHECK( (m54[m12] == 0.0f) );
	CHECK( (m54[m13] == 0.0f) );
	CHECK( (m54[m14] == 0.0f) );
	CHECK( (m54[m15] == 1.0f) );
}

TEST( MatrixRotOrient_set, matrix_tests )
{
	// Orientation Type Constructor:
	Vector  v15( 2.0f, 53.0f, 24.0f);
	Vector  v16( 0.0f, -24.0f, 53.0f);
	Matrix m56;
	m56.Set(ROT_ORIENT, v15, v16 );

	CHECK( eq(m56[m0],-0.9994f, Math::InternalTolerence) );
	CHECK( eq(m56[m1], 0.0313f, Math::InternalTolerence) );
	CHECK( eq(m56[m2], 0.0142f, Math::InternalTolerence) );
	CHECK( (m56[m3] == 0.0f) );
	CHECK( eq(m56[m4], 0.0000f, Math::InternalTolerence) );
	CHECK( eq(m56[m5],-0.4125f, Math::InternalTolerence) );
	CHECK( eq(m56[m6], 0.9110f, Math::InternalTolerence) );
	CHECK( (m56[m7] == 0.0f) );
	CHECK( eq(m56[m8], 0.0344f, Math::InternalTolerence) );
	CHECK( eq(m56[m9], 0.9104f, Math::InternalTolerence) );
	CHECK( eq(m56[m10], 0.4123f, Math::InternalTolerence) );
	CHECK( (m56[m11] == 0.0f) );
	CHECK( (m56[m12] == 0.0f) );
	CHECK( (m56[m13] == 0.0f) );
	CHECK( (m56[m14] == 0.0f) );
	CHECK( (m56[m15] == 1.0f) );
}

TEST( MatrixRotInverseOrient_set,matrix_tests )
{
	// Orientation Type Constructor:
	Vector  v17( 2.0f, 53.0f, 24.0f);
	Vector  v18( 0.0f, -24.0f, 53.0f);
	Matrix m57;
	m57.Set(ROT_INVERSE_ORIENT, v17, v18 );

	CHECK( eq(m57[m0],-0.9994f, Math::InternalTolerence) );
	CHECK( eq(m57[m1], 0.0000f, Math::InternalTolerence) );
	CHECK( eq(m57[m2], 0.0344f, Math::InternalTolerence) );
	CHECK( (m57[m3] == 0.0f) );
	CHECK( eq(m57[m4], 0.0313f, Math::InternalTolerence) );
	CHECK( eq(m57[m5],-0.4125f, Math::InternalTolerence) );
	CHECK( eq(m57[m6], 0.9104f, Math::InternalTolerence) );
	CHECK( (m57[m7] == 0.0f) );
	CHECK( eq(m57[m8], 0.0142f, Math::InternalTolerence) );
	CHECK( eq(m57[m9], 0.9110f, Math::InternalTolerence) );
	CHECK( eq(m57[m10], 0.4123f, Math::InternalTolerence) );
	CHECK( (m57[m11] == 0.0f) );
	CHECK( (m57[m12] == 0.0f) );
	CHECK( (m57[m13] == 0.0f) );
	CHECK( (m57[m14] == 0.0f) );
	CHECK( (m57[m15] == 1.0f) );
}

TEST( MatrixQuaternion_set, matrix_tests )
{
	// Quaternion Type Constructor:
	Matrix Rxyz1(ROT_XYZ, Math::PiOver3, Math::FivePiOver8, Math::PiOver4);

	Quat	Qxyz1(Rxyz1);
	Matrix Mxyz1;
	Mxyz1.Set( Qxyz1 );

	CHECK( eq(Mxyz1[m0],-0.2705f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m1],-0.2705f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m2],-0.9238f,Math::InternalTolerence) );
	CHECK( (Mxyz1[m3] == 0.0f) );
	CHECK( eq(Mxyz1[m4], 0.2122f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m5], 0.9193f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m6],-0.3314f,Math::InternalTolerence) );
	CHECK( (Mxyz1[m7] == 0.0f) );
	CHECK( eq(Mxyz1[m8], 0.9390f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m9],-0.2857f,Math::InternalTolerence) );
	CHECK( eq(Mxyz1[m10],-0.1913f,Math::InternalTolerence) );
	CHECK( (Mxyz1[m11] == 0.0f) );
	CHECK( (Mxyz1[m12] == 0.0f) );
	CHECK( (Mxyz1[m13] == 0.0f) );
	CHECK( (Mxyz1[m14] == 0.0f) );
	CHECK( (Mxyz1[m15] == 1.0f) );
}

// ---  End of File ---------------
