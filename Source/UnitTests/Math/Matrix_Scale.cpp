//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/Matrix4.hpp"

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Matrix_Scale, matix_tests )
{
	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);
	Matrix4 C;
	float scale = 10.0f;

	CHECK_EQ(A[m0], 1.0f );
	CHECK_EQ(A[m1], 2.0f );
	CHECK_EQ(A[m2], 3.0f );
	CHECK_EQ(A[m3], 4.0f );
	CHECK_EQ(A[m4], 5.0f );
	CHECK_EQ(A[m5], 6.0f );
	CHECK_EQ(A[m6], 7.0f );	
	CHECK_EQ(A[m7], 8.0f );
	CHECK_EQ(A[m8], 9.0f );
	CHECK_EQ(A[m9], 10.0f );
	CHECK_EQ(A[m10], 11.0f );
	CHECK_EQ(A[m11], 12.0f );
	CHECK_EQ(A[m12], 13.0f );
	CHECK_EQ(A[m13], 14.0f );
	CHECK_EQ(A[m14], 15.0f );
	CHECK_EQ(A[m15], 16.0f );

	C = scale * A;

	CHECK_EQ(A[m0], 1.0f );
	CHECK_EQ(A[m1], 2.0f );
	CHECK_EQ(A[m2], 3.0f );
	CHECK_EQ(A[m3], 4.0f );
	CHECK_EQ(A[m4], 5.0f );
	CHECK_EQ(A[m5], 6.0f );
	CHECK_EQ(A[m6], 7.0f );	
	CHECK_EQ(A[m7], 8.0f );
	CHECK_EQ(A[m8], 9.0f );
	CHECK_EQ(A[m9], 10.0f );
	CHECK_EQ(A[m10], 11.0f );
	CHECK_EQ(A[m11], 12.0f );
	CHECK_EQ(A[m12], 13.0f );
	CHECK_EQ(A[m13], 14.0f );
	CHECK_EQ(A[m14], 15.0f );
	CHECK_EQ(A[m15], 16.0f );

	CHECK_EQ(C[m0], 10.0f );
	CHECK_EQ(C[m1], 20.0f );
	CHECK_EQ(C[m2], 30.0f );
	CHECK_EQ(C[m3], 40.0f );
	CHECK_EQ(C[m4], 50.0f );
	CHECK_EQ(C[m5], 60.0f );
	CHECK_EQ(C[m6], 70.0f );	
	CHECK_EQ(C[m7], 80.0f );
	CHECK_EQ(C[m8], 90.0f );
	CHECK_EQ(C[m9], 100.0f );
	CHECK_EQ(C[m10], 110.0f );
	CHECK_EQ(C[m11], 120.0f );
	CHECK_EQ(C[m12], 130.0f );
	CHECK_EQ(C[m13], 140.0f );
	CHECK_EQ(C[m14], 150.0f );
	CHECK_EQ(C[m15], 160.0f );
}

TEST(Matrix_times_Scale, Matrix_tests )
{
	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);
	Matrix4 C;
	float scale = 10.0f;

	CHECK_EQ(A[m0], 1.0f );
	CHECK_EQ(A[m1], 2.0f );
	CHECK_EQ(A[m2], 3.0f );
	CHECK_EQ(A[m3], 4.0f );
	CHECK_EQ(A[m4], 5.0f );
	CHECK_EQ(A[m5], 6.0f );
	CHECK_EQ(A[m6], 7.0f );	
	CHECK_EQ(A[m7], 8.0f );
	CHECK_EQ(A[m8], 9.0f );
	CHECK_EQ(A[m9], 10.0f );
	CHECK_EQ(A[m10], 11.0f );
	CHECK_EQ(A[m11], 12.0f );
	CHECK_EQ(A[m12], 13.0f );
	CHECK_EQ(A[m13], 14.0f );
	CHECK_EQ(A[m14], 15.0f );
	CHECK_EQ(A[m15], 16.0f );

	C = A * scale;

	CHECK_EQ(A[m0], 1.0f );
	CHECK_EQ(A[m1], 2.0f );
	CHECK_EQ(A[m2], 3.0f );
	CHECK_EQ(A[m3], 4.0f );
	CHECK_EQ(A[m4], 5.0f );
	CHECK_EQ(A[m5], 6.0f );
	CHECK_EQ(A[m6], 7.0f );	
	CHECK_EQ(A[m7], 8.0f );
	CHECK_EQ(A[m8], 9.0f );
	CHECK_EQ(A[m9], 10.0f );
	CHECK_EQ(A[m10], 11.0f );
	CHECK_EQ(A[m11], 12.0f );
	CHECK_EQ(A[m12], 13.0f );
	CHECK_EQ(A[m13], 14.0f );
	CHECK_EQ(A[m14], 15.0f );
	CHECK_EQ(A[m15], 16.0f );

	CHECK_EQ(C[m0], 10.0f );
	CHECK_EQ(C[m1], 20.0f );
	CHECK_EQ(C[m2], 30.0f );
	CHECK_EQ(C[m3], 40.0f );
	CHECK_EQ(C[m4], 50.0f );
	CHECK_EQ(C[m5], 60.0f );
	CHECK_EQ(C[m6], 70.0f );	
	CHECK_EQ(C[m7], 80.0f );
	CHECK_EQ(C[m8], 90.0f );
	CHECK_EQ(C[m9], 100.0f );
	CHECK_EQ(C[m10], 110.0f );
	CHECK_EQ(C[m11], 120.0f );
	CHECK_EQ(C[m12], 130.0f );
	CHECK_EQ(C[m13], 140.0f );
	CHECK_EQ(C[m14], 150.0f );
	CHECK_EQ(C[m15], 160.0f );
}

TEST(Matrix_ScaleEquals, Matrix_tests )
{

	Vector4 V0(1.0f,2.0f,3.0f,4.0f);
	Vector4 V1(5.0f,6.0f,7.0f,8.0f);
	Vector4 V2(9.0f,10.0f,11.0f,12.0f);
	Vector4 V3(13.0f,14.0f,15.0f,16.0f);

	Matrix4 A(V0,V1,V2,V3);
	float scale = 10.0f;

	CHECK_EQ(A[m0], 1.0f );
	CHECK_EQ(A[m1], 2.0f );
	CHECK_EQ(A[m2], 3.0f );
	CHECK_EQ(A[m3], 4.0f );
	CHECK_EQ(A[m4], 5.0f );
	CHECK_EQ(A[m5], 6.0f );
	CHECK_EQ(A[m6], 7.0f );	
	CHECK_EQ(A[m7], 8.0f );
	CHECK_EQ(A[m8], 9.0f );
	CHECK_EQ(A[m9], 10.0f );
	CHECK_EQ(A[m10], 11.0f );
	CHECK_EQ(A[m11], 12.0f );
	CHECK_EQ(A[m12], 13.0f );
	CHECK_EQ(A[m13], 14.0f );
	CHECK_EQ(A[m14], 15.0f );
	CHECK_EQ(A[m15], 16.0f );

	A *= scale;

	CHECK_EQ(A[m0], 10.0f );
	CHECK_EQ(A[m1], 20.0f );
	CHECK_EQ(A[m2], 30.0f );
	CHECK_EQ(A[m3], 40.0f );
	CHECK_EQ(A[m4], 50.0f );
	CHECK_EQ(A[m5], 60.0f );
	CHECK_EQ(A[m6], 70.0f );	
	CHECK_EQ(A[m7], 80.0f );
	CHECK_EQ(A[m8], 90.0f );
	CHECK_EQ(A[m9], 100.0f );
	CHECK_EQ(A[m10], 110.0f );
	CHECK_EQ(A[m11], 120.0f );
	CHECK_EQ(A[m12], 130.0f );
	CHECK_EQ(A[m13], 140.0f );
	CHECK_EQ(A[m14], 150.0f );
	CHECK_EQ(A[m15], 160.0f );
}

// ---  End of File ---------------
