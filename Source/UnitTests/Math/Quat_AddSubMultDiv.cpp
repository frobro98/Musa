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

TEST(QuatAddSubMultDiv, QuatAddSubMultDivTests)
{
	// Quat Add Sub Mult Div tests;
	CHECK(1);
}

TEST(QuatPlusOperator, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// +Quat

	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	// => out1 = +q1 \n"););
	out1 = +q1;

	CHECK((out1.x == 1.0f));
	CHECK((out1.y == 2.0f));
	CHECK((out1.z == 3.0f));
	CHECK((out1.w == 4.0f));
}

TEST(Quat_QuatPlusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat + Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 5.0f, 20.0f);

	// => out1 = q1 + q2 
	out1 = q1 + q2;

	CHECK((out1.x == 4.0f));
	CHECK((out1.y == 12.0f));
	CHECK((out1.z == 8.0f));
	CHECK((out1.w == 24.0f));
}

TEST(Quat_QuatPlusEqualQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat += Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 5.0f, 20.0f);

	// q1 += q2 
	q1 += q2;

	CHECK((q2.x == 3.0f));
	CHECK((q2.y == 10.0f));
	CHECK((q2.z == 5.0f));
	CHECK((q2.w == 20.0f));

	CHECK((q1.x == 4.0f));
	CHECK((q1.y == 12.0f));
	CHECK((q1.z == 8.0f));
	CHECK((q1.w == 24.0f));
}

TEST(Quat_QuatPlusConstant, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat + Constant
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// out1 = q1 + 5.0f 
	out1 = q1 + 5.0f;

	CHECK((out1.x == 6.0f));
	CHECK((out1.y == 7.0f));
	CHECK((out1.z == 8.0f));
	CHECK((out1.w == 9.0f));
}

TEST(Quat_QuatPlusEqualConstant, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat += Const
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => q1 += 10.0f 
	q1 += 10.0f;

	CHECK((q1.x == 11.0f));
	CHECK((q1.y == 12.0f));
	CHECK((q1.z == 13.0f));
	CHECK((q1.w == 14.0f));
}

TEST(Quat_ConstantPlusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Constant + Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => out1 = 10.0f + q1 
	out1 = 10.0f + q1;

	CHECK((out1.x == 11.0f));
	CHECK((out1.y == 12.0f));
	CHECK((out1.z == 13.0f));
	CHECK((out1.w == 14.0f));
}

TEST(Quat_ConstantPlusQuatPlusConstant, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Constant + Quat + Constant
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => out1 = 10.0f + q1 + 5.0f 
	out1 = 10.0f + q1 + 5.0f;

	CHECK((out1.x == 16.0f));
	CHECK((out1.y == 17.0f));
	CHECK((out1.z == 18.0f));
	CHECK((out1.w == 19.0f));
}

TEST(Quat_QuatPlusConstPlusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat + Constant + Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 5.0f, 20.0f);

	// => out1 = q1 + 5.0f + q2 
	out1 = q1 + 5.0f + q2;

	CHECK((out1.x == 9.0f));
	CHECK((out1.y == 17.0f));
	CHECK((out1.z == 13.0f));
	CHECK((out1.w == 29.0f));
}

TEST(Quat_QuatMinusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat - Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 7.0f, 20.0f);

	// out1 = q1 - q2 
	out1 = q1 - q2;

	CHECK((out1.x == -2.0f));
	CHECK((out1.y == -8.0f));
	CHECK((out1.z == -4.0f));
	CHECK((out1.w == -16.0f));
}

TEST(Quat_QuatMinusEqualQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat -= Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 6.0f, 20.0f);

	// => q1 -= q2 
	q1 -= q2;

	CHECK((q1.x == -2.0f));
	CHECK((q1.y == -8.0f));
	CHECK((q1.z == -3.0f));
	CHECK((q1.w == -16.0f));

	CHECK((q2.x == 3.0f));
	CHECK((q2.y == 10.0f));
	CHECK((q2.z == 6.0f));
	CHECK((q2.w == 20.0f));
}

TEST(Quat_QuatMinusConstant, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat - Constant
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	//=> out1 = q1 - 5.0f 
	out1 = q1 - 5.0f;

	CHECK((out1.x == -4.0f));
	CHECK((out1.y == -3.0f));
	CHECK((out1.z == -2.0f));
	CHECK((out1.w == -1.0f));
}


TEST(Quat_QuatMinusEqualConst, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat -= Const
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => q1 -= 10.0f 
	q1 -= 10.0f;

	CHECK((q1.x == -9.0f));
	CHECK((q1.y == -8.0f));
	CHECK((q1.z == -7.0f));
	CHECK((q1.w == -6.0f));
}

TEST(Quat_ConstantMinusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Constant - Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	// => out1 = 10.0f - q1 
	out1 = 10.0f - q1;

	CHECK((out1.x == 9.0f));
	CHECK((out1.y == 8.0f));
	CHECK((out1.z == 7.0f));
	CHECK((out1.w == 6.0f));
}

TEST(Quat_ConstantMinusQuatMinusConstant, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Constant - Quat - Constant
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => out1 = 10.0f - q1 - 5.0f
	out1 = 10.0f - q1 - 5.0f;

	CHECK((out1.x == 4.0f));
	CHECK((out1.y == 3.0f));
	CHECK((out1.z == 2.0f));
	CHECK((out1.w == 1.0f));
}

TEST(Quat_QuatMinusConstantMinusQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat - Constant - Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(3.0f, 10.0f, 5.0f, 20.0f);

	// out1 = q1 - 5.0f - q2 
	out1 = q1 - 5.0f - q2;

	CHECK((out1.x == -7.0f));
	CHECK((out1.y == -13.0f));
	CHECK((out1.z == -7.0f));
	CHECK((out1.w == -21.0f));
}

TEST(Quat_MinusOperator, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// -Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	//=> out1 = -q1 
	out1 = -q1;
	CHECK((out1.x == -1.0f));
	CHECK((out1.y == -2.0f));
	CHECK((out1.z == -3.0f));
	CHECK((out1.w == -4.0f));
}

TEST(Quat_QuatTimesQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat * Quat 
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(9.0f, -3.0f, 5.0f, 20.0f);

	// out1 = q1 * q2 
	out1 = q1 * q2;

	CHECK((out1.x == 37.0f));
	CHECK((out1.y == 6.0f));
	CHECK((out1.z == 101.0f));
	CHECK((out1.w == 62.0f));
}

TEST(Quat_QuatTimeEqualQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat *= Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(9.0f, -3.0f, 5.0f, 20.0f);

	// => q1 *= q2
	q1 *= q2;

	CHECK((q1.x == 37.0f));
	CHECK((q1.y == 6.0f));
	CHECK((q1.z == 101.0f));
	CHECK((q1.w == 62.0f));
}

TEST(Quat_QuatTimeConst, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat * Constant

	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => out1 = q1 * 5.0f
	out1 = q1 * 5.0f;

	CHECK((out1.x == 5.0f));
	CHECK((out1.y == 10.0f));
	CHECK((out1.z == 15.0f));
	CHECK((out1.w == 20.0f));
}

TEST(Quat_QuatTimesConst, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat *= Constant

	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => q1 *= -10.0f
	q1 *= -10.0f;

	CHECK((q1.x == -10.0f));
	CHECK((q1.y == -20.0f));
	CHECK((q1.z == -30.0f));
	CHECK((q1.w == -40.0f));
}

TEST(Quat_ConstTimesQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Constant * Quat

	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);

	// => out1 = 5.0f * q1
	out1 = 5.0f * q1;

	CHECK((out1.x == 5.0f));
	CHECK((out1.y == 10.0f));
	CHECK((out1.z == 15.0f));
	CHECK((out1.w == 20.0f));
}

TEST(Quat_QuatTimesMatrix, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat * Matrix 
	Matrix mxyz, mx, my, mz, mtmp;
	Quat qxyz, qx, qy, qz, qtmp, qxy;

	mx.Set(ROT_X, Math::PiOver3);
	my.Set(ROT_Y, Math::SevenPiOver8);
	mz.Set(ROT_Z, Math::PiOver2);

	mxyz = mx * my * mz;

	qx.Set(ROT_X, Math::PiOver3);
	qy.Set(ROT_Y, Math::SevenPiOver8);
	qz.Set(ROT_Z, Math::PiOver2);

	qxyz = qx * qy * qz;

	qtmp.Set(mxyz);

	CHECK(eq(qxyz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(qxyz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(qxyz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(qxyz.w, qtmp.w, Math::InternalTolerence));

	mtmp.Set(qxyz);
	CHECK(eq(mtmp[m0], mxyz[m0], Math::InternalTolerence));
	CHECK(eq(mtmp[m1], mxyz[m1], Math::InternalTolerence));
	CHECK(eq(mtmp[m2], mxyz[m2], Math::InternalTolerence));
	CHECK(eq(mtmp[m3], mxyz[m3], Math::InternalTolerence));
	CHECK(eq(mtmp[m4], mxyz[m4], Math::InternalTolerence));
	CHECK(eq(mtmp[m5], mxyz[m5], Math::InternalTolerence));
	CHECK(eq(mtmp[m6], mxyz[m6], Math::InternalTolerence));
	CHECK(eq(mtmp[m7], mxyz[m7], Math::InternalTolerence));
	CHECK(eq(mtmp[m8], mxyz[m8], Math::InternalTolerence));
	CHECK(eq(mtmp[m9], mxyz[m9], Math::InternalTolerence));
	CHECK(eq(mtmp[m10], mxyz[m10], Math::InternalTolerence));
	CHECK(eq(mtmp[m11], mxyz[m11], Math::InternalTolerence));
	CHECK(eq(mtmp[m12], mxyz[m12], Math::InternalTolerence));
	CHECK(eq(mtmp[m13], mxyz[m13], Math::InternalTolerence));
	CHECK(eq(mtmp[m14], mxyz[m14], Math::InternalTolerence));
	CHECK(eq(mtmp[m15], mxyz[m15], Math::InternalTolerence));

	// => q1 = qx * my * mz
	q1 = qx * my * mz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	// => qxy = qx * qy
	// => q1 = qxy * mz
	qxy = qx * qy;
	q1 = qxy * mz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	// => q1 = (qx * qy) * mz
	q1 = (qx * qy) * mz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	// => q1 = qx * (qy * mz)
	q1 = qx * (qy * mz);

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));
}

TEST(Quat_QuatTimesEqualMatrix, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat * Matrix 
	Matrix mxyz, mx, my, mz, mtmp;
	Quat qxyz, qx, qy, qz, qtmp, qxy;
	// Quat *= Matrix
	//mxyz.Set(ROT_XYZ, MATH_PI3, MATH_7PI8, MATH_PI2);
	mx.Set(ROT_X, Math::PiOver3);
	my.Set(ROT_Y, Math::SevenPiOver8);
	mz.Set(ROT_Z, Math::PiOver2);

	mxyz = mx * my * mz;

	//qxyz.Set(ROT_XYZ, MATH_PI3, MATH_7PI8, MATH_PI2);
	qx.Set(ROT_X, Math::PiOver3);
	qy.Set(ROT_Y, Math::SevenPiOver8);
	qz.Set(ROT_Z, Math::PiOver2);

	qxyz = qx * qy * qz;

	qtmp.Set(mxyz);

	CHECK(eq(qxyz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(qxyz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(qxyz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(qxyz.w, qtmp.w, Math::InternalTolerence));

	mtmp.Set(qxyz);

	CHECK(eq(mtmp[m0], mxyz[m0], Math::InternalTolerence));
	CHECK(eq(mtmp[m1], mxyz[m1], Math::InternalTolerence));
	CHECK(eq(mtmp[m2], mxyz[m2], Math::InternalTolerence));
	CHECK(eq(mtmp[m3], mxyz[m3], Math::InternalTolerence));
	CHECK(eq(mtmp[m4], mxyz[m4], Math::InternalTolerence));
	CHECK(eq(mtmp[m5], mxyz[m5], Math::InternalTolerence));
	CHECK(eq(mtmp[m6], mxyz[m6], Math::InternalTolerence));
	CHECK(eq(mtmp[m7], mxyz[m7], Math::InternalTolerence));
	CHECK(eq(mtmp[m8], mxyz[m8], Math::InternalTolerence));
	CHECK(eq(mtmp[m9], mxyz[m9], Math::InternalTolerence));
	CHECK(eq(mtmp[m10], mxyz[m10], Math::InternalTolerence));
	CHECK(eq(mtmp[m11], mxyz[m11], Math::InternalTolerence));
	CHECK(eq(mtmp[m12], mxyz[m12], Math::InternalTolerence));
	CHECK(eq(mtmp[m13], mxyz[m13], Math::InternalTolerence));
	CHECK(eq(mtmp[m14], mxyz[m14], Math::InternalTolerence));
	CHECK(eq(mtmp[m15], mxyz[m15], Math::InternalTolerence));

	// => q1 = qx
	// => q1 *= my * mz

	q1 = qx;
	q1 *= my * mz;

	q1 = -q1;
	// Matrix mm;
	//BEN
	// mm = my * mz;
	// Quat qq;
   //  qq = mm;
   //  q1 *= qq;
   //  q1 *= -1.0f;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	// => qxy = qx * qy
	// => q1 = qxy
	// => q1 *= mz

	qxy = qx * qy;
	q1 = qxy;
	q1 *= mz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));
}

TEST(Quat_MatrixTimesQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat * Matrix 
	Matrix mxyz, mx, my, mz, mtmp;
	Quat qxyz, qx, qy, qz, qtmp, qxy;

	mx.Set(ROT_X, Math::PiOver3);
	my.Set(ROT_Y, Math::SevenPiOver8);
	mz.Set(ROT_Z, Math::PiOver2);

	mxyz = mx * my * mz;

	qx.Set(ROT_X, Math::PiOver3);
	qy.Set(ROT_Y, Math::SevenPiOver8);
	qz.Set(ROT_Z, Math::PiOver2);

	qxyz = qx * qy * qz;
	qtmp.Set(mxyz);

	CHECK(eq(qxyz.x, qtmp.x, Math::InternalTolerence));
	CHECK(eq(qxyz.y, qtmp.y, Math::InternalTolerence));
	CHECK(eq(qxyz.z, qtmp.z, Math::InternalTolerence));
	CHECK(eq(qxyz.w, qtmp.w, Math::InternalTolerence));

	mtmp.Set(qxyz);
	CHECK(eq(mtmp[m0], mxyz[m0], Math::InternalTolerence));
	CHECK(eq(mtmp[m1], mxyz[m1], Math::InternalTolerence));
	CHECK(eq(mtmp[m2], mxyz[m2], Math::InternalTolerence));
	CHECK(eq(mtmp[m3], mxyz[m3], Math::InternalTolerence));
	CHECK(eq(mtmp[m4], mxyz[m4], Math::InternalTolerence));
	CHECK(eq(mtmp[m5], mxyz[m5], Math::InternalTolerence));
	CHECK(eq(mtmp[m6], mxyz[m6], Math::InternalTolerence));
	CHECK(eq(mtmp[m7], mxyz[m7], Math::InternalTolerence));
	CHECK(eq(mtmp[m8], mxyz[m8], Math::InternalTolerence));
	CHECK(eq(mtmp[m9], mxyz[m9], Math::InternalTolerence));
	CHECK(eq(mtmp[m10], mxyz[m10], Math::InternalTolerence));
	CHECK(eq(mtmp[m11], mxyz[m11], Math::InternalTolerence));
	CHECK(eq(mtmp[m12], mxyz[m12], Math::InternalTolerence));
	CHECK(eq(mtmp[m13], mxyz[m13], Math::InternalTolerence));
	CHECK(eq(mtmp[m14], mxyz[m14], Math::InternalTolerence));
	CHECK(eq(mtmp[m15], mxyz[m15], Math::InternalTolerence));

	q1 =  mx * my * qz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	Quat qyz;
	qyz = qy * qz;
	q1 = mx * qyz;

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	q1 = mx * (qy * qz);

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));

	q1 = mx * (my * qz);

	CHECK(eq(q1.x, qxyz.x, Math::InternalTolerence));
	CHECK(eq(q1.y, qxyz.y, Math::InternalTolerence));
	CHECK(eq(q1.z, qxyz.z, Math::InternalTolerence));
	CHECK(eq(q1.w, qxyz.w, Math::InternalTolerence));
}

TEST(Quat_QuatMultElement, QuatAddSubMultDivTests)
{
	Quat q1, q2, qtmp;
	Quat out1, out2;
	// Quat * Quat multiply element by element
	//	const Quat multByElement(const Quat &q) const;

	q1.Set(2.0f, 3.0f, 4.0f, 5.0f);
	q2.Set(6.0f, 7.0f, 8.0f, 9.0f);

	qtmp = q1.MultiplyByElement(q2);

	CHECK((q1.x == 2.0f));
	CHECK((q1.y == 3.0f));
	CHECK((q1.z == 4.0f));
	CHECK((q1.w == 5.0f));
	CHECK((qtmp.x == 12.0f));
	CHECK((qtmp.y == 21.0f));
	CHECK((qtmp.z == 32.0f));
	CHECK((qtmp.w == 45.0f));
}

TEST(Quat_QuatDivQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;

	// Quat / Quat 
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(9.0f, -3.0f, 5.0f, 20.0f);

	// => out1 = q1 / q2 
	out1 = q1 / q2;

	CHECK(eq(out1.x, 0.1111f, Math::InternalTolerence));
	CHECK(eq(out1.y, -0.6667f, Math::InternalTolerence));
	CHECK(eq(out1.z, 0.6f, Math::InternalTolerence));
	CHECK(eq(out1.w, 0.2f, Math::InternalTolerence));
}

TEST(Quat_QuatDivEqualQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat /= Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(9.0f, -3.0f, 5.0f, 20.0f);

	// => q1 /= q2 
	q1 /= q2;

	CHECK(eq(q1.x, 0.1111f, Math::InternalTolerence));
	CHECK(eq(q1.y, -0.6667f, Math::InternalTolerence));
	CHECK(eq(q1.z, 0.6f, Math::InternalTolerence));
	CHECK(eq(q1.w, 0.20f, Math::InternalTolerence));
}

TEST(Quat_QuatDivConst, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat / Constant

	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	/// => out1 = q1 / 5.0f
	out1 = q1 / 5.0f;

	CHECK(eq(out1.x, 0.2f, Math::InternalTolerence));
	CHECK(eq(out1.y, 0.4f, Math::InternalTolerence));
	CHECK(eq(out1.z, 0.6f, Math::InternalTolerence));
	CHECK(eq(out1.w, 0.80f, Math::InternalTolerence));
}

TEST(Quat_QuatDivEqualConst, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Quat /= Constant
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	// => q1 /= -10.0f 
	q1 /= -10.0f;

	CHECK(eq(q1.x, -0.1f, Math::InternalTolerence));
	CHECK(eq(q1.y, -0.2f, Math::InternalTolerence));
	CHECK(eq(q1.z, -0.3f, Math::InternalTolerence));
	CHECK(eq(q1.w, -0.40f, Math::InternalTolerence));
}

TEST(Quat_ConstDivQuat, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Constant / Quat
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	// => out1 = 5.0f / q1 \n"););
	out1 = 5.0f / q1;

	CHECK(eq(out1.x, 5.0f, Math::InternalTolerence));
	CHECK(eq(out1.y, 2.5f, Math::InternalTolerence));
	CHECK(eq(out1.z, 1.6666f, Math::InternalTolerence));
	CHECK(eq(out1.w, 1.25f, Math::InternalTolerence));
}

TEST(Quat_EqualOperator, QuatAddSubMultDivTests)
{
	Quat q1, q2;
	Quat out1, out2;
	// Overload the = operator
	q1.Set(1.0f, 2.0f, 3.0f, 4.0f);
	q2.Set(9.0f, -3.0f, 5.0f, 7.0f);
	// => q1 = q2 \n"););
	q1 = q2;

	CHECK((q1.x == q2.x));
	CHECK((q1.y == q2.y));
	CHECK((q1.z == q2.z));
	CHECK((q1.w == q2.w));

	CHECK((9.0f == q2.x));
	CHECK((-3.0f == q2.y));
	CHECK((5.0f == q2.z));
	CHECK((7.0f == q2.w));
}

TEST(Quat_VectorTimesQuat, QuatAddSubMultDivTests)
{
	Quat	q1;
	Matrix	m1;
	Vector4	v1(1.0f, 2.0f, 3.0f);
	Vector4	v2, v3;

	q1.Set(ROT_X, Math::TwoPiOver3);
	m1.Set(q1);

	v2 = v1 * m1;
	v3 = v1 * q1;

	CHECK(eq(v2.x, v3.x, Math::InternalTolerence));
	CHECK(eq(v2.y, v3.y, Math::InternalTolerence));
	CHECK(eq(v2.z, v3.z, Math::InternalTolerence));
	CHECK(eq(v2.w, v3.w, Math::InternalTolerence))
}

TEST(Quat_VectorTimesEqualQuat, QuatAddSubMultDivTests)
{
	Quat	q1;
	Matrix	m1;
	Vector4	v1(1.0f, 2.0f, 3.0f);
	Vector4	v2,v3;

	q1.Set(ROT_X, Math::TwoPiOver3);
	m1.Set(q1);

	v2 = v1;
	v3 = v1;

	v2 *= m1;
	v3 *= q1;

	CHECK(eq(v2.x, v3.x, Math::InternalTolerence));
	CHECK(eq(v2.y, v3.y, Math::InternalTolerence));
	CHECK(eq(v2.z, v3.z, Math::InternalTolerence));
	CHECK(eq(v2.w, v3.w, Math::InternalTolerence));
}

TEST(Quat_VectorTimesQuat2, QuatAddSubMultDivTests)
{
	Quat	q1;
	Matrix	m1;
	Vector4	v1(1.0f, 2.0f, 3.0f);
	Vector4	v2, v3;

	q1.Set(ROT_AXIS_ANGLE, Vector4(3,-4,7), -Math::TwoPiOver3);
	m1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);

	v2 = v1;
	v3 = v1;

	v2 = v1 * m1;
	v3 = v1 * q1;

	CHECK(eq(v2.x, v3.x, Math::InternalTolerence));
	CHECK(eq(v2.y, v3.y, Math::InternalTolerence));
	CHECK(eq(v2.z, v3.z, Math::InternalTolerence));
	CHECK(eq(v2.w, v3.w, Math::InternalTolerence));
}

TEST(Quat_VectorTimesEqualQuat2, QuatAddSubMultDivTests)
{
	Quat	q1;
	Matrix	m1;
	Vector4	v1(1.0f, 2.0f, 3.0f);
	Vector4	v2, v3;

	q1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);
	m1.Set(ROT_AXIS_ANGLE, Vector4(3, -4, 7), -Math::TwoPiOver3);

	v2 *= m1;
	v3 *= q1;

	CHECK(eq(v2.x, v3.x, Math::InternalTolerence));
	CHECK(eq(v2.y, v3.y, Math::InternalTolerence));
	CHECK(eq(v2.z, v3.z, Math::InternalTolerence));
	CHECK(eq(v2.w, v3.w, Math::InternalTolerence));
}

// ---  End of File ---------------
