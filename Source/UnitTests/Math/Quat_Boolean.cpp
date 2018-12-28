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

TEST( QuatBoolean, QuatBooleanTests )
{
	// Quat Boolean tests 

	CHECK( 1 );
}


TEST( QuatIsEqual, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(9.0f,-3.0f,5.0f,4.0f);

	// => q1.IsEqual(q2, 0.1f) 
	bvalue1 = q1.IsEqual(q2, 0.1f);
	CHECK( bvalue1 == false );
}


TEST( QuatIsEqual1, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(1.0f,2.0f,3.05f,4.0f);

	// q1.IsEqual(q2, 0.1f) 
	bvalue1 = q1.IsEqual(q2, 0.1f);

	CHECK( bvalue1 == true );
}


TEST( QuatIsNegEqual, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsNegativeEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(9.0f,-3.0f,5.0f,4.0f);

	// q1.IsNegativeEqual(q2, 0.1f) 
	bvalue1 = q1.IsNegativeEqual(q2, 0.1f);

	CHECK( bvalue1 == false );
}

TEST( QuatIsNegEqual2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsNegativeEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(-1.0f,-2.0f,-3.05f,-4.0f);
	
	// q1.IsNegativeEqual(q2, 0.1f)
	bvalue1 = q1.IsNegativeEqual(q2, 0.1f);

	CHECK( bvalue1 == true );
}


TEST( QuatIsEquivalent, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsEquivalent()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(-1.0f,-2.0f,-3.05f,-4.0f);

	// q1.IsEquivalent(q2, 0.1f) 
	bvalue1 = q1.IsEquivalent(q2, 0.1f);

	CHECK( bvalue1 == true );
}

TEST( QuatIsEquivalent2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsEquivalent()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(1.0f,2.0f,3.05f,4.0f);
	
	// q1.IsEquivalent(q2, 0.1f) 
	bvalue1 = q1.IsEquivalent(q2, 0.1f);

	CHECK( bvalue1 == true );
}

TEST( QuatIsEquivalent3, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsEquivalent()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(1.0f,2.0f,3.05f,-4.0f);
	
	// q1.IsEquivalent(q2, 0.1f) 
	bvalue1 = q1.IsEquivalent(q2, 0.1f);

	CHECK( bvalue1 == false );
}

TEST( QuatIsConjugateEqual, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsConjugateEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(-1.0f,-2.0f,-3.05f,4.0f);

	// q1.IsConjugateEqual(q2, 0.1f)
	bvalue1 = q1.IsConjugateEqual(q2, 0.1f);
	
	CHECK( bvalue1 == true );
}

TEST( QuatIsConjugateEqual2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsConjugateEqual()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	q2.Set(-1.0f,-2.0f,-3.05f,-4.0f);

	// q1.IsConjugateEqual(q2, 0.1f) 
	bvalue1 = q1.IsConjugateEqual(q2, 0.1f);

	CHECK( bvalue1 == false );
}

TEST( QuatIsIdentity, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsIdentity()
	q1.Set(0.0f,0.0f,0.0f,1.0f);

	// q1.IsIdentity(0.1f) 
	bvalue1 = q1.IsIdentity(0.1f);
	
	CHECK( bvalue1 == true );
}

TEST( QuatIsIdentity2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsIdentity()
	q1.Set(0.0f,0.0f,0.0f,4.0f);

	// q1.IsIdentity(0.1f) 
	bvalue1 = q1.IsIdentity(0.1f);
	CHECK( bvalue1 == false );
}

TEST( QuatIsNormalized, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsNormalized()
	q1.Set( 0.182574f,0.365148f,0.547723f,0.730297f);
	
	// q1.IsNormalized(0.1f) 
	bvalue1 = q1.IsNormalized(0.1f);
	CHECK( bvalue1 == true );
}

TEST( QuatIsNormalized2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsNormalized()
	q1.Set( 0.2174f,0.465148f,0.647723f,0.730297f);
	
	// q1.IsNormalized(0.1f) 
	bvalue1 = q1.IsNormalized(0.1f);
	CHECK( bvalue1 == false );
}

TEST( QuatIsZero, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsZero()
	q1.Set(0.023f, 0.0f, 0.0f, 1.05f);
	
	// q1.IsZero(0.0f) 
	bvalue1 = q1.IsZero(0.0f);
	
	CHECK( bvalue1 == false );
}

TEST( QuatIsZero2, QuatBooleanTests )
{
	Quat  q1,q2;
	bool bvalue1;

	// IsZero()
	q1.Set(0.0f, 0.0f, 0.0f, 0.05f);
	
	// q1.IsZero(0.1f) 
	bvalue1 = q1.IsZero(0.1f);
	CHECK( bvalue1 == true );
}

// ---  End of File ---------------
