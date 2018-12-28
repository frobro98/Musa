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

TEST( Matrix_inverse, matix_tests )
{
	Matrix Rx(ROT_X, 0.23f);
	Matrix Ry(ROT_Y, -1.23f);
	Matrix T( TRANS, 3.4f, 2.9f, -9.0f);

	Matrix M = Rx*Ry * T;
	Matrix N(M);

	M.Inverse();

	Matrix C;
	C = N * M;

	// verify C is identity
	
	bool flag = C.IsIdentity(Math::InternalTolerence);

	CHECK( flag == true );
}

TEST( Matrix_getinverse, matix_tests )
{
	Matrix Rx(ROT_X, 0.23f);
	Matrix Ry(ROT_Y, -1.23f);
	Matrix S( SCALE, 4.0f, 5.0f, 2.0f);
	Matrix T( TRANS, -2.3f, 7.9f, 9.03f);

	Matrix M = S*T*Rx*Ry*S;
	Matrix N;

	N = M.GetInverse();

	Matrix C;
	C = N * M;

	// verify C is identity
	
	bool flag=C.IsIdentity(Math::InternalTolerence);

	CHECK( flag == true );
}

// ---  End of File ---------------
