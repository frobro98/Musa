//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/MathEngine.h"

#define eq	Math::IsEqual 

void verifyRot2Angle( float a1, float a2, bool &result );
void verifyRot3Angle( float a1, float a2, float a3, bool &result );

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST( QuatFunctions, QuatFunctionsTests )
{
	// Quat Functions tests:
	CHECK( 1 );
}

TEST(getConjugate, QuatFunctionsTests)
{
	Matrix	mm2, mm3;
	Quat	q1, q2, q3, qtmp;
	Vector	v1, v2, v3, v4;

	// Conjugate

	//q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx * qy*qz;

	CHECK(eq(q1.x, 0.01887f, Math::InternalTolerence));
	CHECK(eq(q1.y, 0.7715f, Math::InternalTolerence));
	CHECK(eq(q1.z, 0.1999f, Math::InternalTolerence));
	CHECK(eq(q1.w, 0.6036f, Math::InternalTolerence));

	q2 = q1.GetConjugate();
	// => q2 = q1.getConj();\n"););

	CHECK(eq(q2.x, -0.01887f, Math::InternalTolerence));
	CHECK(eq(q2.y, -0.7715f, Math::InternalTolerence));
	CHECK(eq(q2.z, -0.1999f, Math::InternalTolerence));
	CHECK(eq(q2.w, 0.6036f, Math::InternalTolerence));


	// Matrix to Quat
	//m2.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Matrix mx(ROT_X, Math::TwoPiOver3);
	Matrix my(ROT_Y, Math::ThreePiOver8);
	Matrix mz(ROT_Z, Math::ThreePiOver4);

	mm2 = mx * my * mz;

	qtmp.Set(mm2);

	CHECK(eq(q1.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qtmp.w, Math::InternalTolerence));

	// Quaternion to Matrix
	mm3.Set(q1);

	CHECK(eq(mm2[m0], mm3[m0], Math::InternalTolerence));
	CHECK(eq(mm2[m1], mm3[m1], Math::InternalTolerence));
	CHECK(eq(mm2[m2], mm3[m2], Math::InternalTolerence));
	CHECK(eq(mm2[m3], mm3[m3], Math::InternalTolerence));
	CHECK(eq(mm2[m4], mm3[m4], Math::InternalTolerence));
	CHECK(eq(mm2[m5], mm3[m5], Math::InternalTolerence));
	CHECK(eq(mm2[m6], mm3[m6], Math::InternalTolerence));
	CHECK(eq(mm2[m7], mm3[m7], Math::InternalTolerence));
	CHECK(eq(mm2[m8], mm3[m8], Math::InternalTolerence));
	CHECK(eq(mm2[m9], mm3[m9], Math::InternalTolerence));
	CHECK(eq(mm2[m10], mm3[m10], Math::InternalTolerence));
	CHECK(eq(mm2[m11], mm3[m11], Math::InternalTolerence));
	CHECK(eq(mm2[m12], mm3[m12], Math::InternalTolerence));
	CHECK(eq(mm2[m13], mm3[m13], Math::InternalTolerence));
	CHECK(eq(mm2[m14], mm3[m14], Math::InternalTolerence));
	CHECK(eq(mm2[m15], mm3[m15], Math::InternalTolerence));
}

TEST( QuatConjugateInPlace, QuatFunctionsTests )
{
	Quat q1;
	
	// Conjugate in place 

	//q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx*qy*qz;
	
	CHECK( eq(q1.x,0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );
	
	q1.Conjugate();
	// => q1.conj()

	CHECK( eq(q1.x,-0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,-0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,-0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );
}

TEST( QuatConjugateByValue, QuatFunctionsTests )
{
	Quat q1,q2,qtmp;
	Matrix mm2, mm3;
	
	// Conjugate by value
	// q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx*qy*qz;
	
	CHECK( eq(q1.x,0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );
	
	q2 = q1.GetConjugate();
	// => q2 = q1.getConj();
	
	CHECK( eq(q2.x,-0.01887f,Math::InternalTolerence) );
	CHECK( eq(q2.y,-0.7715f,Math::InternalTolerence) );
	CHECK( eq(q2.z,-0.1999f,Math::InternalTolerence) );
	CHECK( eq(q2.w,0.6036f,Math::InternalTolerence) );

	// Matrix to Quat
	//m2.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);

	Matrix mx(ROT_X, Math::TwoPiOver3);
	Matrix my(ROT_Y, Math::ThreePiOver8);
	Matrix mz(ROT_Z, Math::ThreePiOver4);

	mm2 = mx * my * mz;
 
	qtmp.Set( mm2 );
	
	CHECK( eq(q1.x,qtmp.x,Math::InternalTolerence) );
	CHECK( eq(q1.y,qtmp.y,Math::InternalTolerence) );
	CHECK( eq(q1.z,qtmp.z,Math::InternalTolerence) );
	CHECK( eq(q1.w,qtmp.w,Math::InternalTolerence) );

	// Quaternion to Matrix
	mm3.Set(q1);
	CHECK( eq(mm2[m0],mm3[m0],Math::InternalTolerence) );
	CHECK( eq(mm2[m1],mm3[m1],Math::InternalTolerence) );
	CHECK( eq(mm2[m2],mm3[m2],Math::InternalTolerence) );
	CHECK( eq(mm2[m3],mm3[m3],Math::InternalTolerence) );
	CHECK( eq(mm2[m4],mm3[m4],Math::InternalTolerence) );
	CHECK( eq(mm2[m5],mm3[m5],Math::InternalTolerence) );
	CHECK( eq(mm2[m6],mm3[m6],Math::InternalTolerence) );
	CHECK( eq(mm2[m7],mm3[m7],Math::InternalTolerence) );
	CHECK( eq(mm2[m8],mm3[m8],Math::InternalTolerence) );
	CHECK( eq(mm2[m9],mm3[m9],Math::InternalTolerence) );
	CHECK( eq(mm2[m10],mm3[m10],Math::InternalTolerence) );
	CHECK( eq(mm2[m11],mm3[m11],Math::InternalTolerence) );
	CHECK( eq(mm2[m12],mm3[m12],Math::InternalTolerence) );
	CHECK( eq(mm2[m13],mm3[m13],Math::InternalTolerence) );
	CHECK( eq(mm2[m14],mm3[m14],Math::InternalTolerence) );
	CHECK( eq(mm2[m15],mm3[m15],Math::InternalTolerence) );
}

TEST( QuatTransposeToDest, QuatFunctionsTests )
{
	// transpose
	// transpose to destination
	Quat q1,q2,q3,qtmp;
	//q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);

	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx*qy*qz;

	CHECK( eq(q1.x,0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );
	
	q2 = q1.GetTranspose();

	// => q2 = q1.GetTranspose()
	
	CHECK( eq(q2.x,-0.01887f,Math::InternalTolerence) );
	CHECK( eq(q2.y,-0.7715f,Math::InternalTolerence) );
	CHECK( eq(q2.z,-0.1999f,Math::InternalTolerence) );
	CHECK( eq(q2.w,0.6036f,Math::InternalTolerence) );

	// Matrix to Quat
	Matrix mm2, mm3;
	//mm2.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Matrix mx(ROT_X, Math::TwoPiOver3);
	Matrix my(ROT_Y, Math::ThreePiOver8);
	Matrix mz(ROT_Z, Math::ThreePiOver4);

	mm2 = mx * my * mz;

	qtmp.Set( mm2 );
	CHECK( eq(q1.x,qtmp.x,Math::InternalTolerence) );
	CHECK( eq(q1.y,qtmp.y,Math::InternalTolerence) );
	CHECK( eq(q1.z,qtmp.z,Math::InternalTolerence) );
	CHECK( eq(q1.w,qtmp.w,Math::InternalTolerence) );

	// Quaternion to Matrix
	mm3.Set(q1);

	CHECK( eq(mm2[m0],mm3[m0],Math::InternalTolerence) );
	CHECK( eq(mm2[m1],mm3[m1],Math::InternalTolerence) );
	CHECK( eq(mm2[m2],mm3[m2],Math::InternalTolerence) );
	CHECK( eq(mm2[m3],mm3[m3],Math::InternalTolerence) );
	CHECK( eq(mm2[m4],mm3[m4],Math::InternalTolerence) );
	CHECK( eq(mm2[m5],mm3[m5],Math::InternalTolerence) );
	CHECK( eq(mm2[m6],mm3[m6],Math::InternalTolerence) );
	CHECK( eq(mm2[m7],mm3[m7],Math::InternalTolerence) );
	CHECK( eq(mm2[m8],mm3[m8],Math::InternalTolerence) );
	CHECK( eq(mm2[m9],mm3[m9],Math::InternalTolerence) );
	CHECK( eq(mm2[m10],mm3[m10],Math::InternalTolerence) );
	CHECK( eq(mm2[m11],mm3[m11],Math::InternalTolerence) );
	CHECK( eq(mm2[m12],mm3[m12],Math::InternalTolerence) );
	CHECK( eq(mm2[m13],mm3[m13],Math::InternalTolerence) );
	CHECK( eq(mm2[m14],mm3[m14],Math::InternalTolerence) );
	CHECK( eq(mm2[m15],mm3[m15],Math::InternalTolerence) );

	// Matrix to Quat
	mm2.Transpose();
	qtmp.Set( mm2 );
	CHECK( eq(q2.x,qtmp.x,Math::InternalTolerence) );
	CHECK( eq(q2.y,qtmp.y,Math::InternalTolerence) );
	CHECK( eq(q2.z,qtmp.z,Math::InternalTolerence) );
	CHECK( eq(q2.w,qtmp.w,Math::InternalTolerence) );

	// Quaternion to Matrix
	mm3.Set(q2);

	CHECK( eq(mm2[m0],mm3[m0],Math::InternalTolerence) );
	CHECK( eq(mm2[m1],mm3[m1],Math::InternalTolerence) );
	CHECK( eq(mm2[m2],mm3[m2],Math::InternalTolerence) );
	CHECK( eq(mm2[m3],mm3[m3],Math::InternalTolerence) );
	CHECK( eq(mm2[m4],mm3[m4],Math::InternalTolerence) );
	CHECK( eq(mm2[m5],mm3[m5],Math::InternalTolerence) );
	CHECK( eq(mm2[m6],mm3[m6],Math::InternalTolerence) );
	CHECK( eq(mm2[m7],mm3[m7],Math::InternalTolerence) );
	CHECK( eq(mm2[m8],mm3[m8],Math::InternalTolerence) );
	CHECK( eq(mm2[m9],mm3[m9],Math::InternalTolerence) );
	CHECK( eq(mm2[m10],mm3[m10],Math::InternalTolerence) );
	CHECK( eq(mm2[m11],mm3[m11],Math::InternalTolerence) );
	CHECK( eq(mm2[m12],mm3[m12],Math::InternalTolerence) );
	CHECK( eq(mm2[m13],mm3[m13],Math::InternalTolerence) );
	CHECK( eq(mm2[m14],mm3[m14],Math::InternalTolerence) );
	CHECK( eq(mm2[m15],mm3[m15],Math::InternalTolerence) );

	q3 = q2 * q1;
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence) );
	q3 = q1 * q2;
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence) );
}

TEST( QuatTransposeByValue, QuatFunctionsTests )
{
	// "transpose by value
	Quat q1,q2,q3,qtmp;
	Matrix mm1,mm2,mm3;
	//q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx*qy*qz;

	CHECK( eq(q1.x,0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );

	q2 = q1.GetTranspose();

	// => q2 = q1.GetTranspose()

	CHECK( eq(q2.x,-0.01887f,Math::InternalTolerence) );
	CHECK( eq(q2.y,-0.7715f,Math::InternalTolerence) );
	CHECK( eq(q2.z,-0.1999f,Math::InternalTolerence) );
	CHECK( eq(q2.w,0.6036f,Math::InternalTolerence) );

	// Matrix to Quat
	//m2.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Matrix mx(ROT_X, Math::TwoPiOver3);
	Matrix my(ROT_Y, Math::ThreePiOver8);
	Matrix mz(ROT_Z, Math::ThreePiOver4);

	mm2 = mx * my * mz;

	qtmp.Set( mm2 );
	CHECK( eq(q1.x,qtmp.x,Math::InternalTolerence) );
	CHECK( eq(q1.y,qtmp.y,Math::InternalTolerence) );
	CHECK( eq(q1.z,qtmp.z,Math::InternalTolerence) );
	CHECK( eq(q1.w,qtmp.w,Math::InternalTolerence) );

	// Quaternion to Matrix
	mm3.Set(q1);

	CHECK( eq(mm2[m0],mm3[m0],Math::InternalTolerence) );
	CHECK( eq(mm2[m1],mm3[m1],Math::InternalTolerence) );
	CHECK( eq(mm2[m2],mm3[m2],Math::InternalTolerence) );
	CHECK( eq(mm2[m3],mm3[m3],Math::InternalTolerence) );
	CHECK( eq(mm2[m4],mm3[m4],Math::InternalTolerence) );
	CHECK( eq(mm2[m5],mm3[m5],Math::InternalTolerence) );
	CHECK( eq(mm2[m6],mm3[m6],Math::InternalTolerence) );
	CHECK( eq(mm2[m7],mm3[m7],Math::InternalTolerence) );
	CHECK( eq(mm2[m8],mm3[m8],Math::InternalTolerence) );
	CHECK( eq(mm2[m9],mm3[m9],Math::InternalTolerence) );
	CHECK( eq(mm2[m10],mm3[m10],Math::InternalTolerence) );
	CHECK( eq(mm2[m11],mm3[m11],Math::InternalTolerence) );
	CHECK( eq(mm2[m12],mm3[m12],Math::InternalTolerence) );
	CHECK( eq(mm2[m13],mm3[m13],Math::InternalTolerence) );
	CHECK( eq(mm2[m14],mm3[m14],Math::InternalTolerence) );
	CHECK( eq(mm2[m15],mm3[m15],Math::InternalTolerence) );

	// Matrix to Quat
	mm2.Transpose();
	qtmp.Set( mm2 );
	CHECK( eq(q2.x,qtmp.x,Math::InternalTolerence) );
	CHECK( eq(q2.y,qtmp.y,Math::InternalTolerence) );
	CHECK( eq(q2.z,qtmp.z,Math::InternalTolerence) );
	CHECK( eq(q2.w,qtmp.w,Math::InternalTolerence) );

	// Quaternion to Matrix
	mm3.Set(q2);

	CHECK( eq(mm2[m0],mm3[m0],Math::InternalTolerence) );
	CHECK( eq(mm2[m1],mm3[m1],Math::InternalTolerence) );
	CHECK( eq(mm2[m2],mm3[m2],Math::InternalTolerence) );
	CHECK( eq(mm2[m3],mm3[m3],Math::InternalTolerence) );
	CHECK( eq(mm2[m4],mm3[m4],Math::InternalTolerence) );
	CHECK( eq(mm2[m5],mm3[m5],Math::InternalTolerence) );
	CHECK( eq(mm2[m6],mm3[m6],Math::InternalTolerence) );
	CHECK( eq(mm2[m7],mm3[m7],Math::InternalTolerence) );
	CHECK( eq(mm2[m8],mm3[m8],Math::InternalTolerence) );
	CHECK( eq(mm2[m9],mm3[m9],Math::InternalTolerence) );
	CHECK( eq(mm2[m10],mm3[m10],Math::InternalTolerence) );
	CHECK( eq(mm2[m11],mm3[m11],Math::InternalTolerence) );
	CHECK( eq(mm2[m12],mm3[m12],Math::InternalTolerence) );
	CHECK( eq(mm2[m13],mm3[m13],Math::InternalTolerence) );
	CHECK( eq(mm2[m14],mm3[m14],Math::InternalTolerence) );
	CHECK( eq(mm2[m15],mm3[m15],Math::InternalTolerence) );
}

TEST( QuatTransposeInplace, QuatFunctionsTests )
{
	// transpose inplace
	Quat q1;
	//q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4);
	Quat qx(ROT_X, Math::TwoPiOver3);
	Quat qy(ROT_Y, Math::ThreePiOver8);
	Quat qz(ROT_Z, Math::ThreePiOver4);

	// => q1.Set(ROT_XYZ,Math::2PiOver3,Math::3PiOver8,Math::3PiOver4
	q1 = qx*qy*qz;

	CHECK( eq(q1.x,0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );

	q1.Transpose();
	// => q1.T();

	CHECK( eq(q1.x,-0.01887f,Math::InternalTolerence) );
	CHECK( eq(q1.y,-0.7715f,Math::InternalTolerence) );
	CHECK( eq(q1.z,-0.1999f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.6036f,Math::InternalTolerence) );
}

TEST( QuatMag, QuatFunctionsTests )
{
	// magnitude()
	Quat q1;
	float value;
	q1.Set(1.0f,2.0f,3.0f,4.0f);

	// => mag = q1.Magnitude) 
	value = q1.Magnitude();
	
	CHECK( eq(value,5.477225f,Math::InternalTolerence));
}

TEST( QuatMagSquared, QuatFunctionsTests )
{
	// magnitude Squared()
	
	Quat q1;
	float value;
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	
	// => value = q1.MagnitudeSquared() 
	value = q1.MagnitudeSqr();

	CHECK( eq(value,30.0f,Math::InternalTolerence));
}

TEST( QuatInvMag, QuatFunctionsTests )
{
	// inverse magnitude()
	Quat q1;
	float value;

	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => invMag = q1.invMagnitude) 
	value = q1.InverseMagnitude();

	CHECK( eq(value,0.1825742f,Math::InternalTolerence) );
}

TEST( Norm, QuatFunctionsTests )
{
	Quat q1,q2;
	// Normalize()

	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q2 = q1.Normalize() 
	q2 = q1.Normalize();

	CHECK( eq(q2.Magnitude(),1.0f,Math::InternalTolerence) );
	CHECK( eq(q1.x,0.18257418f,Math::InternalTolerence));
	CHECK( eq(q1.y,0.36514837f,Math::InternalTolerence));
	CHECK( eq(q1.z,0.54772258f,Math::InternalTolerence));
	CHECK( eq(q1.w,0.73029673f,Math::InternalTolerence));
	
	CHECK( q1.IsNormalized(Math::InternalTolerence) );
	
	CHECK( eq(q1.MagnitudeSqr(),1.0f, Math::InternalTolerence));
	
	CHECK( eq(q2.x,0.18257418f,Math::InternalTolerence));
	CHECK( eq(q2.y,0.36514837f,Math::InternalTolerence));
	CHECK( eq(q2.z,0.54772258f,Math::InternalTolerence));
	CHECK( eq(q2.w,0.73029673f,Math::InternalTolerence));
	
	CHECK( q2.IsNormalized(Math::InternalTolerence) );
	CHECK( eq(q2.MagnitudeSqr(),1.0f, Math::InternalTolerence));
}

TEST( getNorm, QuatFunctionsTests )
{
	Quat q1,q2;
	// GetNormalized()
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q2 = q1.GetNormalized(); 
	q2 = q1.GetNormalized();

	CHECK( eq(q2.Magnitude(),1.0f,Math::InternalTolerence) );
	CHECK( eq(q2.x,0.18257418f,Math::InternalTolerence));
	CHECK( eq(q2.y,0.36514837f,Math::InternalTolerence));
	CHECK( eq(q2.z,0.54772258f,Math::InternalTolerence));
	CHECK( eq(q2.w,0.73029673f,Math::InternalTolerence));
	
	CHECK( (q1.x == 1.0f) );
	CHECK( (q1.y == 2.0f) );
	CHECK( (q1.z == 3.0f) );
	CHECK( (q1.w == 4.0f));
}

TEST( GetInverse, QuatFunctionsTests )
{
	Quat q1,q2,q3;
	// GetInverse()\n");
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q2 = q1.GetInverse() 
	q2 = q1.GetInverse();
	
	CHECK( eq(q1.x,1.0f,Math::InternalTolerence) );
	CHECK( eq(q1.y,2.0f,Math::InternalTolerence) );
	CHECK( eq(q1.z,3.0f,Math::InternalTolerence) );
	CHECK( eq(q1.w,4.0f,Math::InternalTolerence));
	
	CHECK( eq(q2.x,-0.0333333f,Math::InternalTolerence));
	CHECK( eq(q2.y,-0.0666666f,Math::InternalTolerence));
	CHECK( eq(q2.z,-0.1000000f,Math::InternalTolerence));
	CHECK( eq(q2.w,0.1333333f,Math::InternalTolerence));
	
	q3 = q1 * q2;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence) );
	
	q3 = q2 * q1;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence));
}

TEST( Inverse, QuatFunctionsTests )
{
	Quat q1,q2,q3;
	// inverse()
	
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q1.Inverse() 
	q2 = q1;
	q1.Inverse();

	CHECK( eq(q1.x,-0.0333333f,Math::InternalTolerence) );
	CHECK( eq(q1.y,-0.0666666f,Math::InternalTolerence) );
	CHECK( eq(q1.z,-0.1000000f,Math::InternalTolerence) );
	CHECK( eq(q1.w,0.1333333f,Math::InternalTolerence));
	
	q3 = q1 * q2;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence) );
	
	q3 = q2 * q1;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence));
}

TEST( QuatInverse_3, QuatFunctionsTests )
{
	Quat q1,q2,q3;
	// inverse()
	
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q1.Inverse(q1) 
	q2 = q1;
	q1.Inverse();
	
	CHECK( eq(q2.x,1.0f,Math::InternalTolerence) );
	CHECK( eq(q2.y,2.0f,Math::InternalTolerence) );
	CHECK( eq(q2.z,3.0f,Math::InternalTolerence) );
	CHECK( eq(q2.w,4.0f,Math::InternalTolerence));
	
	CHECK( eq(q1.x,-0.033333335f,Math::InternalTolerence));
	CHECK( eq(q1.y,-0.066666670f,Math::InternalTolerence));
	CHECK( eq(q1.z,-0.10000001f,Math::InternalTolerence));
	CHECK( eq(q1.w, 0.13333334f,Math::InternalTolerence));
	
	q3 = q1 * q2;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence) );
	
	q3 = q2 * q1;
	
	CHECK( eq(q3.x,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.y,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.z,0.0f,Math::InternalTolerence) );
	CHECK( eq(q3.w,1.0f,Math::InternalTolerence));
}

TEST( Inverse_Transpose_Test, QuatFunctionsTests )
{
	Quat q1,q2,q3;
	// inverse()
	
	q1.Set(1.0f,2.0f,3.0f,4.0f);
	// => q1.Inverse(q1) 
	q2 = q1.Normalize();
	q2.Inverse();
	
	CHECK( eq(q1.Magnitude(), 1.0f, Math::InternalTolerence));
	CHECK( eq(q1.x, 0.18257418f,Math::InternalTolerence));
	CHECK( eq(q1.y, 0.36514837f,Math::InternalTolerence));
	CHECK( eq(q1.z, 0.54772258f,Math::InternalTolerence));
	CHECK( eq(q1.w, 0.73029673f,Math::InternalTolerence));
	
	CHECK( eq(q2.x, -0.18257418f,Math::InternalTolerence));
	CHECK( eq(q2.y, -0.36514837f,Math::InternalTolerence));
	CHECK( eq(q2.z, -0.54772258f,Math::InternalTolerence));
	CHECK( eq(q2.w, 0.73029673f,Math::InternalTolerence));
	
	q3 = q1 * q2;
	
	CHECK( eq(q3.x, 0.0f,Math::InternalTolerence));
	CHECK( eq(q3.y, 0.0f,Math::InternalTolerence));
	CHECK( eq(q3.z, 0.0f,Math::InternalTolerence));
	CHECK( eq(q3.w, 1.0f,Math::InternalTolerence));
}

TEST( QuatLqcvq_Lqvqc_1, QuatFunctionsTests )
{
	// Lqcvq() and Lqvqc() operator
	
	//Matrix Mxyz(ROT_XYZ,Math::PiOver3,Math::3PiOver4,Math::PiOver2);
	Matrix mx(ROT_X,Math::PiOver3);
	Matrix my(ROT_Y,Math::ThreePiOver4);
	Matrix mz(ROT_Z,Math::PiOver2);
	Matrix Mxyz = mx*my*mz;

	Matrix MTxyz(Mxyz.GetTranspose());
	//Quat   Qxyz(ROT_XYZ,Math::PiOver3,Math::3PiOver4,Math::PiOver2);
	Quat qx(ROT_X,Math::PiOver3);
	Quat qy(ROT_Y,Math::ThreePiOver4);
	Quat qz(ROT_Z,Math::PiOver2);
	Quat Qxyz = qx*qy*qz;

	Quat   QTxyz(Qxyz.GetTranspose());

	Vector vx(1.0f, 0.0f, 0.0f, 1.0f);
	Vector vy(0.0f, 1.0f, 0.0f, 1.0f);
	Vector vz(0.0f, 0.0f, 1.0f, 1.0f);
	Vector vo(1.0f,2.0f,3.0f);
	Vector Vout1_x,Vout1_y,Vout1_z,Vout1_o;
	Vector Vout2_x,Vout2_y,Vout2_z,Vout2_o;

	Vout1_x = vx * Mxyz;
	Vout1_y = vy * Mxyz;
	Vout1_z = vz * Mxyz;
	Vout1_o = vo * Mxyz;

	// Vout = vIn * Quat
	Qxyz.Lqcvq(vx, Vout2_x);
	Qxyz.Lqcvq(vy, Vout2_y);
	Qxyz.Lqcvq(vz, Vout2_z);
	Qxyz.Lqcvq(vo, Vout2_o);

	// => Vout1_x = vx * Mxyz;
	// => Vout1_y = vy * Mxyz;
	// => Vout1_z = vz * Mxyz;
	// => Vout1_o = vo * Mxyz;
	// => Qxyz.Lqcvq(vx, Vout2_x);
	// => Qxyz.Lqcvq(vy, Vout2_y);
	// => Qxyz.Lqcvq(vz, Vout2_z);  
	// => Qxyz.Lqcvq(vo, Vout2_o);  

	CHECK( eq(Vout1_x.x,Vout2_x.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.y,Vout2_x.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.z,Vout2_x.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.w,Vout2_x.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_y.x,Vout2_y.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.y,Vout2_y.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.z,Vout2_y.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.w,Vout2_y.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_z.x,Vout2_z.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.y,Vout2_z.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.z,Vout2_z.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.w,Vout2_z.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_o.x,Vout2_o.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.y,Vout2_o.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.z,Vout2_o.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.w,Vout2_o.w,Math::InternalTolerence) );

	Vout1_x = vx * Mxyz.GetTranspose();
	Vout1_y = vy * Mxyz.GetTranspose();
	Vout1_z = vz * Mxyz.GetTranspose();
	Vout1_o = vo * Mxyz.GetTranspose();

	// Vout = vIn * Quat.T()
	Qxyz.Lqvqc(vx, Vout2_x);
	Qxyz.Lqvqc(vy, Vout2_y);
	Qxyz.Lqvqc(vz, Vout2_z);
	Qxyz.Lqvqc(vo, Vout2_o);

	// => Vout1_x = vx * Mxyz.T();
	// => Vout1_y = vy * Mxyz.T();
	// => Vout1_z = vz * Mxyz.T();
	// => Vout1_o = vo * Mxyz.T();
	// => Qxyz.Lqvqc(vx, Vout2_x);
	// => Qxyz.Lqvqc(vy, Vout2_y);
	// => Qxyz.Lqvqc(vz, Vout2_z); 
	// => Qxyz.Lqvqc(vo, Vout2_o);  

	CHECK( eq(Vout1_x.x,Vout2_x.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.y,Vout2_x.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.z,Vout2_x.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_x.w,Vout2_x.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_y.x,Vout2_y.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.y,Vout2_y.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.z,Vout2_y.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_y.w,Vout2_y.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_z.x,Vout2_z.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.y,Vout2_z.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.z,Vout2_z.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_z.w,Vout2_z.w,Math::InternalTolerence) );

	CHECK( eq(Vout1_o.x,Vout2_o.x,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.y,Vout2_o.y,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.z,Vout2_o.z,Math::InternalTolerence) );
	CHECK( eq(Vout1_o.w,Vout2_o.w,Math::InternalTolerence) );
}

TEST( dot_Test, QuatFunctionsTests )
{
	Quat q1;
	
	q1.Set(1.0f,2.0f,3.0f,4.0f);

	// 4D dot product
	float value = q1.Dot(q1);

	CHECK( eq(value, 30.0f, Math::InternalTolerence));
	CHECK( eq(q1.x,1.0f,Math::InternalTolerence));
	CHECK( eq(q1.y, 2.0f,Math::InternalTolerence));
	CHECK( eq(q1.z, 3.0f,Math::InternalTolerence));
	CHECK( eq(q1.w, 4.0f,Math::InternalTolerence));
}

TEST( Assignment_Matrix_Test, QuatFunctionsTests )
{
	Quat q1;
	Matrix A(ROT_XYZ, 0.34f, -2.3f, 1.2f);
	
	Quat q2(A);

	q1 = A;

	Matrix B(q1);

	CHECK( eq(A[m0], B[m0], Math::InternalTolerence));
	CHECK( eq(A[m1], B[m1], Math::InternalTolerence));
	CHECK( eq(A[m2], B[m2], Math::InternalTolerence));
	CHECK( eq(A[m3], B[m3], Math::InternalTolerence));
	CHECK( eq(A[m4], B[m4], Math::InternalTolerence));
	CHECK( eq(A[m5], B[m5], Math::InternalTolerence));
	CHECK( eq(A[m6], B[m6], Math::InternalTolerence));
	CHECK( eq(A[m7], B[m7], Math::InternalTolerence));
	CHECK( eq(A[m8], B[m8], Math::InternalTolerence));
	CHECK( eq(A[m9], B[m9], Math::InternalTolerence));
	CHECK( eq(A[m10], B[m10], Math::InternalTolerence));
	CHECK( eq(A[m11], B[m11], Math::InternalTolerence));
	CHECK( eq(A[m12], B[m12], Math::InternalTolerence));
	CHECK( eq(A[m13], B[m13], Math::InternalTolerence));
	CHECK( eq(A[m14], B[m14], Math::InternalTolerence));
	CHECK( eq(A[m15], B[m15], Math::InternalTolerence));
}

// ---  End of File ---------------
