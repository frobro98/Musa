//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/MathEngine.h"
#include "Math/VectorFunctions.h"

#define eq	Math::IsEqual  

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(VectorLERPFunction, VectorLerpTests)
{
	// Vector Lerp Functions()
	CHECK(1);
}

TEST(LERP_Array, VectorLerpTests)
{
	Vector4 vSource[] =
	{
		Vector4(1.00f, 2.0f,   3.0f,  4.0f),
		Vector4(21.00f, 12.0f, 23.0f, 40.0f),
		Vector4(13.00f, 24.0f, 33.0f, 40.0f),
		Vector4(0.50f, 22.0f, 43.0f, 40.0f),
		Vector4(1.30f, 23.0f, 35.0f, 40.0f),
		Vector4(15.00f, 72.0f, 35.0f, 40.0f),
		Vector4(21.00f, 92.0f, 33.0f, 40.0f),
	};

	Vector4 vTarget[] =
	{
		Vector4(-1.00f, 12.0f,   13.0f,  41.0f),
		Vector4(213.00f, -12.0f, 263.0f, 140.0f),
		Vector4(133.00f, -24.0f, 363.0f, 240.0f),
		Vector4(3.50f, -22.0f, 453.0f, 340.0f),
		Vector4(13.30f, -23.0f, 365.0f, 440.0f),
		Vector4(153.00f, -72.0f, 735.0f, 540.0f),
		Vector4(231.00f, -92.0f, 733.0f, 340.0f),
	};

#define numVectors  ((sizeof(vSource))/(sizeof(vSource[0])) )

	Vector4 vResult[numVectors];
	Vector4 vOut;

	Math::LerpArray(vResult, vSource, vTarget, 0.65f, numVectors);

	for (int i = 0; i < numVectors; i++)
	{
		Math::Lerp(vOut, vSource[i], vTarget[i], 0.65f);

		CHECK(eq(vOut.x, vResult[i].x, Math::InternalTolerence));
		CHECK(eq(vOut.y, vResult[i].y, Math::InternalTolerence));
		CHECK(eq(vOut.z, vResult[i].z, Math::InternalTolerence));
		CHECK(eq(vOut.w, vResult[i].w, Math::InternalTolerence));
	}
}

TEST(MathLerp, MathTests)
{
	Vector4 va(0.0f, 0.0f, 0.0f);
	Vector4 vb(10.0f, 20.0f, 30.0f);
	Vector4 vout;

	Math::Lerp(vout, va, vb, 0.0f);

	CHECK((vout.x == 0.0f));
	CHECK((vout.y == 0.0f));
	CHECK((vout.z == 0.0f));
	CHECK((vout.w == 1.0f));
}

TEST(MathLerp2, MathTests)
{
	Vector4 va(0.0f, 0.0f, 0.0f);
	Vector4 vb(10.0f, 20.0f, 30.0f);
	Vector4 vout;

	Math::Lerp(vout, va, vb, 0.5f);

	CHECK((vout.x == 5.0f));
	CHECK((vout.y == 10.0f));
	CHECK((vout.z == 15.0f));
	CHECK((vout.w == 1.0f));
}

TEST(MathLerp3, MathTests)
{
	Vector4 va(0.0f, 0.0f, 0.0f);
	Vector4 vb(10.0f, 20.0f, 30.0f);
	Vector4 vout;

	Math::Lerp(vout, va, vb, 0.25f);

	CHECK((vout.x == 2.5f));
	CHECK((vout.y == 5.0f));
	CHECK((vout.z == 7.50f));
	CHECK((vout.w == 1.0f));
}

TEST(MathLerp4, MathTests)
{
	Vector4 va(0.0f, 0.0f, 0.0f);
	Vector4 vb(10.0f, 20.0f, 30.0f);
	Vector4 vout;

	Math::Lerp(vout, va, vb, 1.0f);

	CHECK((vout.x == 10.0f));
	CHECK((vout.y == 20.0f));
	CHECK((vout.z == 30.0f));
	CHECK((vout.w == 1.0f));
}

// ---  End of File ---------------
