//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Gam575
//----------------------------------------------------------------------------- 

#include "Framework/UnitTest.h"
#include "Math/Quat.hpp"

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Quat_Default_Constructor, QuatConstructorTests )
{
	// Default Constructor:
	Quat q1;

	CHECK_EQ(0.0f, q1.x );
	CHECK_EQ(0.0f, q1.y );
	CHECK_EQ(0.0f, q1.z );
	CHECK_EQ(1.0f, q1.w );
}

TEST(Quat_Constructor_Set_Every_Component, QuatConstructorTests )
{
	// Set every element constructor
	Quat q10(2.0f,3.0f,4.0f,5.0f);

	CHECK_EQ(2.0f, q10.x );
	CHECK_EQ(3.0f, q10.y );
	CHECK_EQ(4.0f, q10.z );
	CHECK_EQ(5.0f, q10.w );
}

TEST(Quat_Copy_Constructor, QuatConstructorTests )
{
	// Copy Constructor
	Quat q11(2.0f,3.0f,4.0f,5.0f);
	Quat q12(q11);

	CHECK_EQ(2.0f, q11.x );
	CHECK_EQ(3.0f, q11.y );
	CHECK_EQ(4.0f, q11.z );
	CHECK_EQ(5.0f, q11.w );
	CHECK_EQ(q12.x, q11.x );
	CHECK_EQ(q12.y, q11.y );
	CHECK_EQ(q12.z, q11.z );
	CHECK_EQ(q12.w, q11.w );
}

TEST(Quat_Vect_Component_Real_Component_Constructor, QuatConstructorTests )
{
	// Vector component, Real component Constructor
	Vector4 v10(12.0f,13.0f,14.0f,15.0f);
	Quat q13(v10,20.0f);

	CHECK_EQ(12.0f, q13.x );
	CHECK_EQ(13.0f, q13.y );
	CHECK_EQ(14.0f, q13.z );
	CHECK_EQ(20.0f, q13.w );
}

TEST(Quat_From_Matrix_Constructor, QuatConstructorTests )
{
	// Quaternion from a Matrix Constructor
	Matrix4 m40;
	Matrix4 Rx(ROT_X,Math::PiOver2);
	Matrix4 Ry(ROT_Y,-Math::PiOver3);
	Matrix4 Rz(ROT_Z,-Math::SevenPiOver8);

	m40 = Rx*Ry*Rz;

	Quat q14(m40);
	
	CHECK_EQ(0.2273f,q14.x);
	CHECK_EQ(0.6695f,q14.y);
	CHECK_EQ(0.5316f,q14.z);
	CHECK_EQ(-0.4662f,q14.w);

	Matrix4 M3;
	M3.Set(q14);

	CHECK_EQ(m40[m0],M3[m0]);
	CHECK_EQ(m40[m1],M3[m1]);
	CHECK_EQ(m40[m2],M3[m2]);
	CHECK_EQ(m40[m3],M3[m3]);
	CHECK_EQ(m40[m4],M3[m4]);
	CHECK_EQ(m40[m5],M3[m5]);
	CHECK_EQ(m40[m6],M3[m6]);
	CHECK_EQ(m40[m7],M3[m7]);
	CHECK_EQ(m40[m8],M3[m8]);
	CHECK_EQ(m40[m9],M3[m9]);
	CHECK_EQ(m40[m10],M3[m10]);
	CHECK_EQ(m40[m11],M3[m11]);
	CHECK_EQ(m40[m12],M3[m12]);
	CHECK_EQ(m40[m13],M3[m13]);
	CHECK_EQ(m40[m14],M3[m14]);
	CHECK_EQ(m40[m15],M3[m15]);
}

TEST(Quat_IDENTITY_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat q2 = Quat::Identity;

	CHECK_EQ(q2.x, 0.0f);
	CHECK_EQ(q2.y, 0.0f);
	CHECK_EQ(q2.z, 0.0f);
	CHECK_EQ(q2.w, 1.0f);

	Matrix4 	M2;
	M2.Set(IDENTITY);
	Quat qtmp;
	qtmp.Set(M2 );
	CHECK_EQ(q2.x,qtmp.x);
	CHECK_EQ(q2.y,qtmp.y);
	CHECK_EQ(q2.z,qtmp.z);
	CHECK_EQ(q2.w,qtmp.w);
	
	Matrix4 M3;
	M3.Set(q2);
	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ROTXYZ_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat qxyz(ROT_XYZ, 0.345f, -1.234f, 0.832f);

   Quat qx(ROT_X, 0.345f);
   Quat qy(ROT_Y, -1.234f);
   Quat qz(ROT_Z,  0.832f);

   Quat qtmp;

   qtmp = qx * qy * qz;

	CHECK_EQ(qxyz.x,qtmp.x);
	CHECK_EQ(qxyz.y,qtmp.y);
	CHECK_EQ(qxyz.z,qtmp.z);
	CHECK_EQ(qxyz.w,qtmp.w);
}

TEST(Quat_SET_ROTXYZ_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat qxyz;
   
   qxyz.Set(ROT_XYZ, 0.345f, -1.234f, 0.832f);

   Quat qx(ROT_X, 0.345f);
   Quat qy(ROT_Y, -1.234f);
   Quat qz(ROT_Z,  0.832f);

   Quat qtmp;

   qtmp = qx * qy * qz;

	CHECK_EQ(qxyz.x,qtmp.x);
	CHECK_EQ(qxyz.y,qtmp.y);
	CHECK_EQ(qxyz.z,qtmp.z);
	CHECK_EQ(qxyz.w,qtmp.w);
}

TEST(Quat_X_PI2_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat q4(ROT_X, Math::PiOver2);

	// Matrix to Quaternion
	Matrix4 M2;
	M2.Set(ROT_X, Math::PiOver2);

	Quat qtmp;
	qtmp.Set(M2);
	CHECK_EQ(q4.x,qtmp.x);
	CHECK_EQ(q4.y,qtmp.y);
	CHECK_EQ(q4.z,qtmp.z);
	CHECK_EQ(q4.w,qtmp.w);

	// Quaternion to Matrix
	Matrix4 M3;
	M3.Set(q4);
	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_X_PI_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat q5(ROT_X, Math::Pi);

	// Matrix to Quaternion
	Matrix4 M2, M3;
	Quat qtmp;
	M2.Set(ROT_X, Math::Pi);
	qtmp.Set(M2 );

	CHECK_EQ(q5.x,qtmp.x);
	CHECK_EQ(q5.y,qtmp.y);
	CHECK_EQ(q5.z,qtmp.z);
	CHECK_EQ(q5.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q5);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Y_PI2_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:

	Quat q6(ROT_Y, Math::PiOver2);

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Y, Math::PiOver2);
	qtmp.Set(M2 );
	CHECK_EQ(q6.x,qtmp.x);
	CHECK_EQ(q6.y,qtmp.y);
	CHECK_EQ(q6.z,qtmp.z);
	CHECK_EQ(q6.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q6);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Y_PI_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat q7(ROT_Y, Math::Pi);

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Y, Math::Pi);
	qtmp.Set(M2 );
	CHECK_EQ(q7.x,qtmp.x);
	CHECK_EQ(q7.y,qtmp.y);
	CHECK_EQ(q7.z,qtmp.z);
	CHECK_EQ(q7.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q7);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Z_PI2_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:
	Quat q8(ROT_Z, Math::PiOver2);

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Z, Math::PiOver2);
	qtmp.Set(M2 );
	CHECK_EQ(q8.x,qtmp.x);
	CHECK_EQ(q8.y,qtmp.y);
	CHECK_EQ(q8.z,qtmp.z);
	CHECK_EQ(q8.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q8);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Z_PI_Constructor, QuatConstructorTests )
{
	// Special Type Constructor:

	Quat q9(ROT_Z,Math::Pi);

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Z,Math::Pi);
	qtmp.Set(M2 );
	CHECK_EQ(q9.x,qtmp.x);
	CHECK_EQ(q9.y,qtmp.y);
	CHECK_EQ(q9.z,qtmp.z);
	CHECK_EQ(q9.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q9);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_X_Constructor, QuatConstructorTests )
{
	// Rot_X Type Constructor:
	Quat Qx(ROT_X, Math::PiOver3);	

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_X, Math::PiOver3);
	qtmp.Set(M2 );
	CHECK_EQ(Qx.x,qtmp.x);
	CHECK_EQ(Qx.y,qtmp.y);
	CHECK_EQ(Qx.z,qtmp.z);
	CHECK_EQ(Qx.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qx);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Y_Constructor, QuatConstructorTests )
{
	// Rot_Y Type Constructor:
	Quat Qy(ROT_Y, Math::PiOver3);


	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Y, Math::PiOver3);
	qtmp.Set(M2 );
	CHECK_EQ(Qy.x,qtmp.x);
	CHECK_EQ(Qy.y,qtmp.y);
	CHECK_EQ(Qy.z,qtmp.z);
	CHECK_EQ(Qy.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qy);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_Z_Constructor, QuatConstructorTests )
{
	// Rot_Z Type Constructor:
	Quat Qz(ROT_Z, Math::PiOver3);

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;
	M2.Set(ROT_Z, Math::PiOver3);
	qtmp.Set(M2 );
	CHECK_EQ(Qz.x,qtmp.x);
	CHECK_EQ(Qz.y,qtmp.y);
	CHECK_EQ(Qz.z,qtmp.z);
	CHECK_EQ(Qz.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qz);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_XY_Constructor, QuatConstructorTests )
{
	// Rot_XY Type Constructor:
	Quat Qx(ROT_X, Math::PiOver3);
	Quat Qy(ROT_Y, Math::PiOver4);

	Quat Qxy;
	Qxy = Qx * Qy;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mx(ROT_X, Math::PiOver3);
	Matrix4 My(ROT_Y, Math::PiOver4);
	M2 = Mx * My;

	qtmp.Set(M2 );
	CHECK_EQ(Qxy.x,qtmp.x);
	CHECK_EQ(Qxy.y,qtmp.y);
	CHECK_EQ(Qxy.z,qtmp.z);
	CHECK_EQ(Qxy.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qxy);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_XZ_Constructor, QuatConstructorTests )
{
	// Rot_XZ Type Constructor:
	Quat Qx(ROT_X, Math::PiOver3);
	Quat Qz(ROT_Z, Math::PiOver4);
	Quat Qxz;
	
	Qxz = Qx * Qz;

	// Matrix to Quaternion
	Matrix4 M2, M3;
	Quat qtmp;
	Matrix4 Mx(ROT_X, Math::PiOver3);
	Matrix4 Mz(ROT_Z, Math::PiOver4);
	M2 = Mx * Mz;

	qtmp.Set(M2 );
	CHECK_EQ(Qxz.x,qtmp.x);
	CHECK_EQ(Qxz.y,qtmp.y);
	CHECK_EQ(Qxz.z,qtmp.z);
	CHECK_EQ(Qxz.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qxz);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_YX_Constructor, QuatConstructorTests )
{
	// Rot_YX Type Constructor:
	Quat Qy(ROT_Y, Math::PiOver3);
	Quat Qx(ROT_X, Math::PiOver4);
	Quat Qyx = Qy * Qx;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 My(ROT_Y, Math::PiOver3);
	Matrix4 Mx(ROT_X, Math::PiOver4);
	M2 = My * Mx;

	qtmp.Set(M2 );
	CHECK_EQ(Qyx.x,qtmp.x);
	CHECK_EQ(Qyx.y,qtmp.y);
	CHECK_EQ(Qyx.z,qtmp.z);
	CHECK_EQ(Qyx.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qyx);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_YZ_Constructor, QuatConstructorTests )
{
	// Rot_YZ Type Constructor:
	Quat Qy(ROT_Y, Math::PiOver3);
	Quat Qz(ROT_Z, Math::PiOver4);
	Quat Qyz = Qy * Qz;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 My(ROT_Y, Math::PiOver3);
	Matrix4 Mz(ROT_Z, Math::PiOver4);	
	M2 = My * Mz;

	qtmp.Set(M2 );
	CHECK_EQ(Qyz.x,qtmp.x);
	CHECK_EQ(Qyz.y,qtmp.y);
	CHECK_EQ(Qyz.z,qtmp.z);
	CHECK_EQ(Qyz.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qyz);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ZX_Constructor, QuatConstructorTests )
{
	// Rot_ZX Type Constructor:
	Quat Qz(ROT_Z, Math::PiOver3);
	Quat Qx(ROT_X, Math::PiOver4);
	Quat Qzx =Qz * Qx;

	 // Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mz(ROT_Z, Math::PiOver3);
	Matrix4 Mx(ROT_X, Math::PiOver4);	
	M2 = Mz * Mx;

	qtmp.Set(M2 );
	CHECK_EQ(Qzx.x,qtmp.x);
	CHECK_EQ(Qzx.y,qtmp.y);
	CHECK_EQ(Qzx.z,qtmp.z);
	CHECK_EQ(Qzx.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qzx);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ZY_Constructor, QuatConstructorTests )
{
	// Rot_ZY Type Constructor:
	Quat Qz(ROT_Z, Math::PiOver3);
	Quat Qy(ROT_Y, Math::PiOver4);
	Quat Qzy =Qz * Qy;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mz(ROT_Z, Math::PiOver3);
	Matrix4 My(ROT_Y, Math::PiOver4);	
	M2 = Mz * My;

	qtmp.Set(M2 );
	CHECK_EQ(Qzy.x,qtmp.x);
	CHECK_EQ(Qzy.y,qtmp.y);
	CHECK_EQ(Qzy.z,qtmp.z);
	CHECK_EQ(Qzy.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qzy);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_XYZ_Constructor, QuatConstructorTests )
{
	// Rot_XYZ Type Constructor:
	Quat Qx(ROT_X, Math::PiOver3 );
	Quat Qy(ROT_Y, Math::FivePiOver8);
	Quat Qz(ROT_Z, Math::PiOver4 );
	Quat Qxyz = Qx * Qy * Qz;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mx(ROT_X, Math::PiOver3);
	Matrix4 My(ROT_Y,  Math::FivePiOver8);
	Matrix4 Mz(ROT_Z,  Math::PiOver4 );
	M2 = Mx * My * Mz;

	qtmp.Set(M2 );
	CHECK_EQ(Qxyz.x,qtmp.x);
	CHECK_EQ(Qxyz.y,qtmp.y);
	CHECK_EQ(Qxyz.z,qtmp.z);
	CHECK_EQ(Qxyz.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qxyz);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_XZY_Constructor, QuatConstructorTests )
{
	// Rot_XZY Type Constructor:
	Quat Qx(ROT_X, Math::PiOver3 );
	Quat Qz(ROT_Z, Math::FivePiOver8);
	Quat Qy(ROT_Y, Math::PiOver4 );
	Quat Qxzy = Qx * Qz * Qy;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mx(ROT_X, Math::PiOver3);
	Matrix4 Mz(ROT_Z,  Math::FivePiOver8);
	Matrix4 My(ROT_Y,  Math::PiOver4 );
	M2 = Mx * Mz * My;

	qtmp.Set(M2 );
	CHECK_EQ(Qxzy.x,qtmp.x);
	CHECK_EQ(Qxzy.y,qtmp.y);
	CHECK_EQ(Qxzy.z,qtmp.z);
	CHECK_EQ(Qxzy.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qxzy);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_YXZ_Constructor, QuatConstructorTests )
{
	// Rot_YXZ Type Constructor:
	Quat Qy(ROT_Y, Math::PiOver3 );
	Quat Qx(ROT_X, Math::FivePiOver8);
	Quat Qz(ROT_Z, Math::PiOver4 );
	Quat Qyxz = Qy * Qx * Qz;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 My(ROT_Y, Math::PiOver3);
	Matrix4 Mx(ROT_X,  Math::FivePiOver8);
	Matrix4 Mz(ROT_Z,  Math::PiOver4 );
	M2 = My * Mx * Mz;

	qtmp.Set(M2 );
	CHECK_EQ(Qyxz.x,qtmp.x);
	CHECK_EQ(Qyxz.y,qtmp.y);
	CHECK_EQ(Qyxz.z,qtmp.z);
	CHECK_EQ(Qyxz.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qyxz);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_YZX_Constructor, QuatConstructorTests )
{
	// Rot_YZX Type Constructor:

	Quat Qy(ROT_Y, Math::PiOver3 );
	Quat Qz(ROT_Z, Math::FivePiOver8);
	Quat Qx(ROT_X, Math::PiOver4 );
	Quat Qyzx = Qy * Qz * Qx;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 My(ROT_Y, Math::PiOver3);
	Matrix4 Mz(ROT_Z,  Math::FivePiOver8);
	Matrix4 Mx(ROT_X,  Math::PiOver4 );
	M2 = My * Mz * Mx;

	qtmp.Set(M2 );
	CHECK_EQ(Qyzx.x,qtmp.x);
	CHECK_EQ(Qyzx.y,qtmp.y);
	CHECK_EQ(Qyzx.z,qtmp.z);
	CHECK_EQ(Qyzx.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qyzx);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ZXY_Constructor, QuatConstructorTests )
{
	// Rot_ZXY Type Constructor:
	Quat Qz(ROT_Z, Math::PiOver3 );
	Quat Qx(ROT_X, Math::FivePiOver8);
	Quat Qy(ROT_Y, Math::PiOver4 );
	Quat Qzxy = Qz * Qx * Qy;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mz(ROT_Z, Math::PiOver3);
	Matrix4 Mx(ROT_X,  Math::FivePiOver8);
	Matrix4 My(ROT_Y,  Math::PiOver4 );
	M2 = Mz * Mx * My;

	qtmp.Set(M2 );
	CHECK_EQ(Qzxy.x,qtmp.x);
	CHECK_EQ(Qzxy.y,qtmp.y);
	CHECK_EQ(Qzxy.z,qtmp.z);
	CHECK_EQ(Qzxy.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qzxy);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ZYX_Constructor, QuatConstructorTests )
{
	// Rot_ZYX Type Constructor:
	Quat Qz(ROT_Z, Math::PiOver3 );
	Quat Qy(ROT_Y, Math::FivePiOver8);
	Quat Qx(ROT_X, Math::PiOver4 );
	Quat Qzyx = Qz * Qy * Qx;

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	Matrix4 Mz(ROT_Z, Math::PiOver3);
	Matrix4 My(ROT_Y,  Math::FivePiOver8);
	Matrix4 Mx(ROT_X,  Math::PiOver4 );
	M2 = Mz * My * Mx;

	qtmp.Set(M2 );
	CHECK_EQ(Qzyx.x,qtmp.x);
	CHECK_EQ(Qzyx.y,qtmp.y);
	CHECK_EQ(Qzyx.z,qtmp.z);
	CHECK_EQ(Qzyx.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qzyx);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_AXIS_ANGLE_Constructor, QuatConstructorTests )
{
	// Axis and Angle Type Constructor:
	Vector4  v11(2.0f, 53.0f, 24.0f);
	Quat Qa1(ROT_AXIS_ANGLE, v11, Math::PiOver3 );

	// Matrix to Quaternion
	Matrix4 M2,M3;
	Quat qtmp;

	M2.Set(ROT_AXIS_ANGLE, v11, Math::PiOver3 );

	qtmp.Set(M2 );
	CHECK_EQ(Qa1.x,qtmp.x);
	CHECK_EQ(Qa1.y,qtmp.y);
	CHECK_EQ(Qa1.z,qtmp.z);
	CHECK_EQ(Qa1.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(Qa1);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ROT_ORIENT_Constructor, QuatConstructorTests )
{
	// Orientation Type Constructor:

	Vector4  v15(2.0f, 53.0f, 24.0f);
	Vector4  v16(0.0f, -24.0f, 53.0f);
	Quat q56(ROT_ORIENT, v15, v16 );

	Matrix4 M2,M3;
	Quat qtmp;

	M2.Set(ROT_ORIENT, v15, v16 );
	qtmp.Set(M2 );
	CHECK_EQ(q56.x,qtmp.x);
	CHECK_EQ(q56.y,qtmp.y);
	CHECK_EQ(q56.z,qtmp.z);
	CHECK_EQ(q56.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q56);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

TEST(Quat_ROT_INVERSE_ORIENT_Constructor, QuatConstructorTests )
{
	// Orientation Type Constructor:
	Vector4  v17(2.0f, 53.0f, 24.0f);
	Vector4  v18(0.0f, -24.0f, 53.0f);
	Quat q57(ROT_INVERSE_ORIENT, v17, v18 );

	Matrix4 M2,M3;
	Quat qtmp;

	M2.Set(ROT_INVERSE_ORIENT, v17, v18 );
	qtmp.Set(M2 );
	CHECK_EQ(q57.x,qtmp.x);
	CHECK_EQ(q57.y,qtmp.y);
	CHECK_EQ(q57.z,qtmp.z);
	CHECK_EQ(q57.w,qtmp.w);

	// Quaternion to Matrix
	M3.Set(q57);

	CHECK_EQ(M2[m0],M3[m0]);
	CHECK_EQ(M2[m1],M3[m1]);
	CHECK_EQ(M2[m2],M3[m2]);
	CHECK_EQ(M2[m3],M3[m3]);
	CHECK_EQ(M2[m4],M3[m4]);
	CHECK_EQ(M2[m5],M3[m5]);
	CHECK_EQ(M2[m6],M3[m6]);
	CHECK_EQ(M2[m7],M3[m7]);
	CHECK_EQ(M2[m8],M3[m8]);
	CHECK_EQ(M2[m9],M3[m9]);
	CHECK_EQ(M2[m10],M3[m10]);
	CHECK_EQ(M2[m11],M3[m11]);
	CHECK_EQ(M2[m12],M3[m12]);
	CHECK_EQ(M2[m13],M3[m13]);
	CHECK_EQ(M2[m14],M3[m14]);
	CHECK_EQ(M2[m15],M3[m15]);
}

// ---  End of File ---------------
