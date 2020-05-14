// Copyright 2020, Nathan Blane

#include "Matrix4.hpp"
#include "Vector4.hpp"
#include "Quat.hpp"
#include "MathFunctions.hpp"
#include "Assertion.h"

Matrix4::Matrix4()
	: v0(0.f, 0.f, 0.f, 0.f),
	v1(0.f, 0.f, 0.f, 0.f),
	v2(0.f, 0.f, 0.f, 0.f),
	v3(0.f, 0.f, 0.f, 0.f)
{
}

Matrix4::Matrix4(const Vector4 & row0, const Vector4 & row1, const Vector4 & row2, const Vector4 & row3)
	: v0(row0), v1(row1), v2(row2), v3(row3)
{
}

Matrix4::Matrix4(MatrixSpecialType specialEnum)
{
	Set(specialEnum);
}

Matrix4::Matrix4(MatrixTransType transEnum, const Vector4 & transVec)
{
	Set(transEnum, transVec);
}

Matrix4::Matrix4(MatrixTransType transEnum, float32 x, float32 y, float32 z)
{
	Set(transEnum, x, y, z);
}

Matrix4::Matrix4(RotType rotationEnum, float32 angle)
{
	Set(rotationEnum, angle);
}

Matrix4::Matrix4(Rot3AxisType type, float32 xAngleRad, float32 yAngleRad, float32 zAngleRad)
{
	Set(type, xAngleRad, yAngleRad, zAngleRad);
}

Matrix4::Matrix4(RotAxisAngleType type, const Vector4& vect, float32 angleRads)
{
	Set(type, vect, angleRads);
}

Matrix4::Matrix4(RotOrientType type, const Vector4& dof, const Vector4& up)
{
	Set(type, dof, up);
}

Matrix4::Matrix4(MatrixScaleType scaleEnum, const Vector4& scaleVec)
{
	Set(scaleEnum, scaleVec);
}

Matrix4::Matrix4(MatrixScaleType scaleEnum, float32 sx, float32 sy, float32 sz)
{
	Set(scaleEnum, sx, sy, sz);
}

Matrix4::Matrix4(const Quat& q)
{
	Set(q);
}

void Matrix4::Set(MatrixTransType /*transEnum*/, const Vector4& transVec)
{
	v0 = Vector4(1.f, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, 1.f, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, 1.f, 0.f);
	v3 = Vector4(transVec.x, transVec.y, transVec.z, 1.f);
}

void Matrix4::Set(MatrixTransType /*transEnum*/, float x, float32 y, float32 z)
{
	v0 = Vector4(1.f, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, 1.f, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, 1.f, 0.f);
	v3 = Vector4(x, y, z, 1.f);
}

void Matrix4::Set(RotType rotationEnum, float32 angle)
{
	switch (rotationEnum)
	{
		case ROT_X:
		{
			v0 = Vector4(1.f, 0.f, 0.f, 0.f);
			v1 = Vector4(0.f, Math::Cos(angle), Math::Sin(angle), 0.f);
			v2 = Vector4(0.f, -Math::Sin(angle), Math::Cos(angle), 0.f);
			v3 = Vector4(0.f, 0.f, 0.f, 1.f);
		}break;
		case ROT_Y:
		{
			v0 = Vector4(Math::Cos(angle), 0.f, -Math::Sin(angle), 0.f);
			v1 = Vector4(0.f, 1.f, 0.f, 0.f);
			v2 = Vector4(Math::Sin(angle), 0.f, Math::Cos(angle), 0.f);
			v3 = Vector4(0.f, 0.f, 0.f, 1.f);
		}break;
		case ROT_Z:
		{
			v0 = Vector4(Math::Cos(angle), Math::Sin(angle), 0.f, 0.f);
			v1 = Vector4(-Math::Sin(angle), Math::Cos(angle), 0.f, 0.f);
			v2 = Vector4(0.f, 0.f, 1.f, 0.f);
			v3 = Vector4(0.f, 0.f, 0.f, 1.f);
		}break;
		default:
		{
			Assert(false);
		}
	}
}

void Matrix4::Set(Rot3AxisType, float32 xRad, float32 yRad, float32 zRad)
{
	Quat qX(ROT_X, xRad);
	Quat qY(ROT_Y, yRad);
	Quat qZ(ROT_Z, zRad);

	Set(qX*qY*qZ);
}

void Matrix4::Set(RotAxisAngleType type, const Vector4& vect, float32 angleRads)
{
	Quat q(type, vect, angleRads);
	Set(q);
}

void Matrix4::Set(RotOrientType type, const Vector4& dof, const Vector4& up)
{
	switch (type)
	{
		case ROT_ORIENT:
		{
			Vector4 right = dof.Cross(up);
			Assert(!right.IsZero());
			right.Normalize();
			
			Vector4 realUp = right.Cross(dof);
			realUp.Normalize();

			Vector4 upDof = realUp.Cross(dof);
			upDof.Normalize();

			Vector4 newUp = dof.Cross(upDof);
			newUp.Normalize();

			Vector4 normDof = dof.GetNormalized();
			Set(
				Vector4(upDof.x, upDof.y, upDof.z, 0),
				Vector4(newUp.x, newUp.y, newUp.z, 0),
				Vector4(normDof.x, normDof.y, normDof.z, 0),
				Vector4(0, 0, 0, 1)
			);
		}break;
		case ROT_INVERSE_ORIENT:
		{
			Vector4 right = dof.Cross(up);
			Assert(!right.IsZero());
			right.Normalize();

			Vector4 realUp = right.Cross(dof);
			realUp.Normalize();

			Vector4 upDof = realUp.Cross(dof);
			upDof.Normalize();

			Vector4 newUp = dof.Cross(upDof);
			newUp.Normalize();

			Vector4 normDof = dof.GetNormalized();
			Set(
				Vector4(upDof.x, upDof.y, upDof.z, 0),
				Vector4(newUp.x, newUp.y, newUp.z, 0),
				Vector4(normDof.x, normDof.y, normDof.z, 0),
				Vector4(0, 0, 0, 1)
			);
			Transpose();
		}break;
		default:
			Assert(false);
			break;
	}
}

void Matrix4::Set(const Quat& q)
{
	v0 = Vector4(
		1.f - 2.f * (q.y * q.y + q.z * q.z),
		2.f * (q.x * q.y + q.w * q.z),
		2.f * (q.x * q.z - q.w * q.y),
		0.f
	);

	v1 = Vector4(
		2.f * (q.x * q.y - q.w * q.z),
		1.f - 2.f * (q.x * q.x + q.z * q.z),
		2.f * (q.y * q.z + q.w * q.x),
		0.f
	);

	v2 = Vector4(
		2.f * (q.x * q.z + q.w * q.y),
		2.f * (q.y * q.z - q.w * q.x),
		1.f - 2.f * (q.x * q.x + q.y * q.y),
		0.f
	);

	v3 = Vector4(
		0.f, 0.f, 0.f, 1.f
	);
}

void Matrix4::Set(MatrixScaleType /*scaleEnum*/, const Vector4& scaleVec)
{
	v0 = Vector4(scaleVec.x, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, scaleVec.y, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, scaleVec.z, 0.f);
	v3 = Vector4(0.f, 0.f, 0.f, 1.f);
}

void Matrix4::Set(MatrixScaleType /*scaleEnum*/, float32 sx, float32 sy, float32 sz)
{
	v0 = Vector4(sx, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, sy, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, sz, 0.f);
	v3 = Vector4(0.f, 0.f, 0.f, 1.f);
}

void Matrix4::Set(MatrixSpecialType specialEnum)
{
	switch (specialEnum)
	{
		case ZERO:
		{
			v0 = Vector4(0.f, 0.f, 0.f, 0.f);
			v1 = Vector4(0.f, 0.f, 0.f, 0.f);
			v2 = Vector4(0.f, 0.f, 0.f, 0.f);
			v3 = Vector4(0.f, 0.f, 0.f, 0.f);
		}break;
		case IDENTITY:
		{
			v0 = Vector4(1.f, 0.f, 0.f, 0.f);
			v1 = Vector4(0.f, 1.f, 0.f, 0.f);
			v2 = Vector4(0.f, 0.f, 1.f, 0.f);
			v3 = Vector4(0.f, 0.f, 0.f, 1.f);
		}break;
		default:
		{
			Assert(false);
		}
	}
}

void Matrix4::Set(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
{
	v0 = row0;
	v1 = row1;
	v2 = row2;
	v3 = row3;
}

void Matrix4::Set(MatrixRowType rowEnum, const Vector4& rowVec)
{
	switch (rowEnum)
	{
		case ROW_0:
		{
			v0 = rowVec;
		}break;
		case ROW_1:
		{
			v1 = rowVec;
		}break;
		case ROW_2:
		{
			v2 = rowVec;
		}break;
		case ROW_3:
		{
			v3 = rowVec;
		}break;
		default:
			Assert(false);
	}
}

void Matrix4::Set(const Vector4 &axis, float32 angle)
{
	// angle
	// axis;
	const float32 angle_a = 0.5f * angle;
	float32 cos_a;
	float32 sin_a;

	cos_a = cosf(angle_a);
	sin_a = sinf(angle_a);

	Vector4 qV = axis.GetNormalized();

	qV *= sin_a;

	Vector4 Q;
	Q.x = qV.x;
	Q.y = qV.y;
	Q.z = qV.z;
	Q.w = cos_a;

	// this function has been transposed
	float32 x2, y2, z2;
	float32 xx, xy, xz;
	float32 yy, yz, zz;
	float32 wx, wy, wz;

	// ADD test to make sure that quat is normalized

	x2 = Q.x + Q.x;
	y2 = Q.y + Q.y;
	z2 = Q.z + Q.z;
		  
	xx = Q.x * x2;
	xy = Q.x * y2;
	xz = Q.x * z2;
		  
	yy = Q.y * y2;
	yz = Q.y * z2;
	zz = Q.z * z2;
		  
	wx = Q.w * x2;
	wy = Q.w * y2;
	wz = Q.w * z2;

	v0 = Vector4(
		1.0f - (yy + zz),
		xy + wz,
		xz - wy,
		0.f
	);

	v1 = Vector4(
		xy - wz,
		1.0f - (xx + zz),
		yz + wx,
		0.f
	);

	v2 = Vector4(
		xz + wy,
		yz - wx,
		1.0f - (xx + yy),
		0.f
	);

	v3 = Vector4(
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Vector4 Matrix4::Get(MatrixRowType rowEnum) const
{
	switch (rowEnum)
	{
		case ROW_0:
		{
			return v0;
		}
		case ROW_1:
		{
			return v1;
		}
		case ROW_2:
		{
			return v2;
		}
		case ROW_3:
		{
			return v3;
		}
	}

	Assert(false);
	return Vector4();
}

float32 Matrix4::Determinant() const
{
	const float32 a = v2.y * v3.w - v2.w * v3.y; //_m9*_m15 - _m11*_m13;
	const float32 b = v2.x * v3.z - v2.z * v3.x; //_m8*_m14 -_m10*_m12;

	const float32 l01 = a;
	const float32 l20 = a;
	const float32 l12 = b;
	const float32 l31 = b;
	const float32 l00 = v2.z * v3.w - v3.z * v2.w; //_m10*_m15 - _m11*_m14;
	const float32 l02 = v2.y * v3.z - v3.y * v2.z; //_m9*_m14 - _m10*_m13;
	const float32 l11 = v2.x * v3.w - v3.x * v2.w; //_m8*_m15 - _m11*_m12;
	const float32 l22 = v2.x * v3.y - v3.x * v2.y; //_m8*_m13 - _m9*_m12;
	

	const float32 det00 = v1.y*(l00) - v1.z*(l01) + v1.w*(l02);
	const float32 det01 = v1.x*(l00) - v1.z*(l11) + v1.w*(l12);
	const float32 det02 = v1.x*(l20) - v1.y*(l11) + v1.w*(l22);
	const float32 det03 = v1.x*(l02) - v1.y*(l31) +v1.z*(l22);

	return v0.x*det00 - v0.y*det01 + v0.z*det02 - v0.w*det03;
}

void Matrix4::Inverse()
{
	// TODO - Optimize the math that happens here!
	float32 detScalar = Determinant();
	if (Math::IsNonZero(detScalar))
	{
		detScalar = 1.f / detScalar;

		// Adjunct of current Matrix

		float32 b00 = v1.z * v2.w * v3.y
			- v1.w * v2.z * v3.y
			+ v1.w * v2.y * v3.z
			- v1.y * v2.w * v3.z
			- v1.z * v2.y * v3.w
			+ v1.y * v2.z * v3.w;
		float32 b01 = v0.w * v2.z * v3.y
			- v0.z * v2.w * v3.y
			- v0.w * v2.y * v3.z
			+ v0.y * v2.w * v3.z
			+ v0.z * v2.y * v3.w
			- v0.y * v2.z * v3.w;
		float32 b02 = v0.z * v1.w * v3.y
			- v0.w * v1.z * v3.y
			+ v0.w * v1.y * v3.z
			- v0.y * v1.w * v3.z
			- v0.z * v1.y * v3.w
			+ v0.y * v1.z * v3.w;
		float32 b03 = v0.w * v1.z * v2.y
			- v0.z * v1.w * v2.y
			- v0.w * v1.y * v2.z
			+ v0.y * v1.w * v2.z
			+ v0.z * v1.y * v2.w
			- v0.y * v1.z * v2.w;
		float32 b10 = v1.w * v2.z * v3.x
			- v1.z * v2.w * v3.x
			- v1.w * v2.x * v3.z
			+ v1.x * v2.w * v3.z
			+ v1.z * v2.x * v3.w
			- v1.x * v2.z * v3.w;
		float32 b11 = v0.z * v2.w * v3.x
			- v0.w * v2.z * v3.x
			+ v0.w * v2.x * v3.z
			- v0.x * v2.w * v3.z
			- v0.z * v2.x * v3.w
			+ v0.x * v2.z * v3.w;
		float32 b12 = v0.w * v1.z * v3.x
			- v0.z * v1.w * v3.x
			- v0.w * v1.x * v3.z
			+ v0.x * v1.w * v3.z
			+ v0.z * v1.x * v3.w
			- v0.x * v1.z * v3.w;
		float32 b13 = v0.z * v1.w * v2.x
			- v0.w * v1.z * v2.x
			+ v0.w * v1.x * v2.z
			- v0.x * v1.w * v2.z
			- v0.z * v1.x * v2.w
			+ v0.x * v1.z * v2.w;
		float32 b20 = v1.y * v2.w * v3.x
			- v1.w * v2.y * v3.x
			+ v1.w * v2.x * v3.y
			- v1.x * v2.w * v3.y
			- v1.y * v2.x * v3.w
			+ v1.x * v2.y * v3.w;
		float32 b21 = v0.w * v2.y * v3.x
			- v0.y * v2.w * v3.x
			- v0.w * v2.x * v3.y
			+ v0.x * v2.w * v3.y
			+ v0.y * v2.x * v3.w
			- v0.x * v2.y * v3.w;
		float32 b22 = v0.y * v1.w * v3.x
			- v0.w * v1.y * v3.x
			+ v0.w * v1.x * v3.y
			- v0.x * v1.w * v3.y
			- v0.y * v1.x * v3.w
			+ v0.x * v1.y * v3.w;
		float32 b23 = v0.w * v1.y * v2.x
			- v0.y * v1.w * v2.x
			- v0.w * v1.x * v2.y
			+ v0.x * v1.w * v2.y
			+ v0.y * v1.x * v2.w
			- v0.x * v1.y * v2.w;
		float32 b30 = v1.z * v2.y * v3.x
			- v1.y * v2.z * v3.x
			- v1.z * v2.x * v3.y
			+ v1.x * v2.z * v3.y
			+ v1.y * v2.x * v3.z
			- v1.x * v2.y * v3.z;
		float32 b31 = v0.y * v2.z * v3.x
			- v0.z * v2.y * v3.x
			+ v0.z * v2.x * v3.y
			- v0.x * v2.z * v3.y
			- v0.y * v2.x * v3.z
			+ v0.x * v2.y * v3.z;
		float32 b32 = v0.z * v1.y * v3.x
			- v0.y * v1.z * v3.x
			- v0.z * v1.x * v3.y
			+ v0.x * v1.z * v3.y
			+ v0.y * v1.x * v3.z
			- v0.x * v1.y * v3.z;

		float32 b33 = v0.y * v1.z * v2.x
			- v0.z * v1.y * v2.x
			+ v0.z * v1.x * v2.y
			- v0.x * v1.z * v2.y
			- v0.y * v1.x * v2.z
			+ v0.x * v1.y * v2.z;

		v0 = Vector4(
			b00 * detScalar,
			b01 * detScalar,
			b02 * detScalar,
			b03 * detScalar
		);
		v1 = Vector4(
			b10 * detScalar,
			b11 * detScalar,
			b12 * detScalar,
			b13 * detScalar
		);
		v2 = Vector4(
			b20 * detScalar,
			b21 * detScalar,
			b22 * detScalar,
			b23 * detScalar
		);
		v3 = Vector4(
			b30 * detScalar,
			b31 * detScalar,
			b32 * detScalar,
			b33 * detScalar
		);
	}
}

Matrix4 Matrix4::GetInverse() const
{
	// TODO - Optimize the math that happens here!
	float32 detScalar = Determinant();
	if (Math::IsNonZero(detScalar))
	{
		detScalar = 1.f / detScalar;

		// Adjunct of current Matrix

		float32 b00 = v1.z * v2.w * v3.y
			- v1.w * v2.z * v3.y
			+ v1.w * v2.y * v3.z
			- v1.y * v2.w * v3.z
			- v1.z * v2.y * v3.w
			+ v1.y * v2.z * v3.w;
		float32 b01 = v0.w * v2.z * v3.y
			- v0.z * v2.w * v3.y
			- v0.w * v2.y * v3.z
			+ v0.y * v2.w * v3.z
			+ v0.z * v2.y * v3.w
			- v0.y * v2.z * v3.w;
		float32 b02 = v0.z * v1.w * v3.y
			- v0.w * v1.z * v3.y
			+ v0.w * v1.y * v3.z
			- v0.y * v1.w * v3.z
			- v0.z * v1.y * v3.w
			+ v0.y * v1.z * v3.w;
		float32 b03 = v0.w * v1.z * v2.y
			- v0.z * v1.w * v2.y
			- v0.w * v1.y * v2.z
			+ v0.y * v1.w * v2.z
			+ v0.z * v1.y * v2.w
			- v0.y * v1.z * v2.w;
		float32 b10 = v1.w * v2.z * v3.x
			- v1.z * v2.w * v3.x
			- v1.w * v2.x * v3.z
			+ v1.x * v2.w * v3.z
			+ v1.z * v2.x * v3.w
			- v1.x * v2.z * v3.w;
		float32 b11 = v0.z * v2.w * v3.x
			- v0.w * v2.z * v3.x
			+ v0.w * v2.x * v3.z
			- v0.x * v2.w * v3.z
			- v0.z * v2.x * v3.w
			+ v0.x * v2.z * v3.w;
		float32 b12 = v0.w * v1.z * v3.x
			- v0.z * v1.w * v3.x
			- v0.w * v1.x * v3.z
			+ v0.x * v1.w * v3.z
			+ v0.z * v1.x * v3.w
			- v0.x * v1.z * v3.w;
		float32 b13 = v0.z * v1.w * v2.x
			- v0.w * v1.z * v2.x
			+ v0.w * v1.x * v2.z
			- v0.x * v1.w * v2.z
			- v0.z * v1.x * v2.w
			+ v0.x * v1.z * v2.w;
		float32 b20 = v1.y * v2.w * v3.x
			- v1.w * v2.y * v3.x
			+ v1.w * v2.x * v3.y
			- v1.x * v2.w * v3.y
			- v1.y * v2.x * v3.w
			+ v1.x * v2.y * v3.w;
		float32 b21 = v0.w * v2.y * v3.x
			- v0.y * v2.w * v3.x
			- v0.w * v2.x * v3.y
			+ v0.x * v2.w * v3.y
			+ v0.y * v2.x * v3.w
			- v0.x * v2.y * v3.w;
		float32 b22 = v0.y * v1.w * v3.x
			- v0.w * v1.y * v3.x
			+ v0.w * v1.x * v3.y
			- v0.x * v1.w * v3.y
			- v0.y * v1.x * v3.w
			+ v0.x * v1.y * v3.w;
		float32 b23 = v0.w * v1.y * v2.x
			- v0.y * v1.w * v2.x
			- v0.w * v1.x * v2.y
			+ v0.x * v1.w * v2.y
			+ v0.y * v1.x * v2.w
			- v0.x * v1.y * v2.w;
		float32 b30 = v1.z * v2.y * v3.x
			- v1.y * v2.z * v3.x
			- v1.z * v2.x * v3.y
			+ v1.x * v2.z * v3.y
			+ v1.y * v2.x * v3.z
			- v1.x * v2.y * v3.z;
		float32 b31 = v0.y * v2.z * v3.x
			- v0.z * v2.y * v3.x
			+ v0.z * v2.x * v3.y
			- v0.x * v2.z * v3.y
			- v0.y * v2.x * v3.z
			+ v0.x * v2.y * v3.z;
		float32 b32 = v0.z * v1.y * v3.x
			- v0.y * v1.z * v3.x
			- v0.z * v1.x * v3.y
			+ v0.x * v1.z * v3.y
			+ v0.y * v1.x * v3.z
			- v0.x * v1.y * v3.z;

		float32 b33 = v0.y * v1.z * v2.x
			- v0.z * v1.y * v2.x
			+ v0.z * v1.x * v2.y
			- v0.x * v1.z * v2.y
			- v0.y * v1.x * v2.z
			+ v0.x * v1.y * v2.z;

		Matrix4 m;
		m.v0 = Vector4(
			b00 * detScalar,
			b01 * detScalar,
			b02 * detScalar,
			b03 * detScalar
		);
		m.v1 = Vector4(
			b10 * detScalar,
			b11 * detScalar,
			b12 * detScalar,
			b13 * detScalar
		);
		m.v2 = Vector4(
			b20 * detScalar,
			b21 * detScalar,
			b22 * detScalar,
			b23 * detScalar
		);
		m.v3 = Vector4(
			b30 * detScalar,
			b31 * detScalar,
			b32 * detScalar,
			b33 * detScalar
		);

		return m;
	}

	return Matrix4();
}

void Matrix4::Transpose()
{
	float32 tmp = v0.y;
	v0.y = v1.x;
	v1.x = tmp;

	tmp = v0.z;
	v0.z = v2.x;
	v2.x = tmp;

	tmp = v0.w;
	v0.w = v3.x;
	v3.x = tmp;

	tmp = v3.y;
	v3.y = v1.w;
	v1.w = tmp;

	tmp = v3.z;
	v3.z = v2.w;
	v2.w = tmp;

	// middle
	tmp = v2.y;
	v2.y = v1.z;
	v1.z = tmp;
}

Matrix4 Matrix4::GetTranspose() const
{
	Matrix4 m(*this);

	float32 tmp = m.v0.y;
	m.v0.y = m.v1.x;
	m.v1.x = tmp;

	tmp = m.v0.z;
	m.v0.z = m.v2.x;
	m.v2.x = tmp;

	tmp = m.v0.w;
	m.v0.w = m.v3.x;
	m.v3.x = tmp;

	tmp = m.v3.y;
	m.v3.y = m.v1.w;
	m.v1.w = tmp;

	tmp = m.v3.z;
	m.v3.z = m.v2.w;
	m.v2.w = tmp;

	// middle
	tmp = m.v2.y;
	m.v2.y = m.v1.z;
	m.v1.z = tmp;

	return m;
}

bool Matrix4::IsIdentity(float32 epsilon) const
{
	return v0.IsEqual(Vector4(1, 0, 0, 0), epsilon) &&
		v1.IsEqual(Vector4(0, 1, 0, 0), epsilon) &&
		v2.IsEqual(Vector4(0, 0, 1, 0), epsilon) &&
		v3.IsEqual(Vector4(0, 0, 0, 1), epsilon);
}

bool Matrix4::IsEqual(const Matrix4& m, float32 epsilon) const
{
	return v0.IsEqual(m.v0, epsilon) &&
		v1.IsEqual(m.v1, epsilon) &&
		v2.IsEqual(m.v2, epsilon) &&
		v3.IsEqual(m.v3, epsilon);
}

Matrix4 Matrix4::operator+(const Matrix4& m) const
{
	Matrix4 ret(*this);
	ret += m;
	return ret;
}

Matrix4 Matrix4::operator-(const Matrix4& m) const
{
	Matrix4 ret(*this);
	return ret -= m;
}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
	Matrix4 ret(*this);
	return ret *= m;
}

Matrix4 Matrix4::operator*(float32 s) const
{
	Matrix4 m(*this);
	return m *= s;
}

Matrix4& Matrix4::operator+=(const Matrix4& m)
{
	v0.x += m.v0.x;
	v0.y += m.v0.y;
	v0.z += m.v0.z;
	v0.w += m.v0.w;

	v1.x += m.v1.x;
	v1.y += m.v1.y;
	v1.z += m.v1.z;
	v1.w += m.v1.w;

	v2.x += m.v2.x;
	v2.y += m.v2.y;
	v2.z += m.v2.z;
	v2.w += m.v2.w;

	v3.x += m.v3.x;
	v3.y += m.v3.y;
	v3.z += m.v3.z;
	v3.w += m.v3.w;

	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& m)
{
	v0.x -= m.v0.x;
	v0.y -= m.v0.y;
	v0.z -= m.v0.z;
	v0.w -= m.v0.w;

	v1.x -= m.v1.x;
	v1.y -= m.v1.y;
	v1.z -= m.v1.z;
	v1.w -= m.v1.w;

	v2.x -= m.v2.x;
	v2.y -= m.v2.y;
	v2.z -= m.v2.z;
	v2.w -= m.v2.w;

	v3.x -= m.v3.x;
	v3.y -= m.v3.y;
	v3.z -= m.v3.z;
	v3.w -= m.v3.w;

	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& m)
{
	v0 = Vector4(
		v0.x*m.v0.x + v0.y*m.v1.x + v0.z*m.v2.x + v0.w*m.v3.x,
		v0.x*m.v0.y + v0.y*m.v1.y + v0.z*m.v2.y + v0.w*m.v3.y,
		v0.x*m.v0.z + v0.y*m.v1.z + v0.z*m.v2.z + v0.w*m.v3.z,
		v0.x*m.v0.w + v0.y*m.v1.w + v0.z*m.v2.w + v0.w*m.v3.w
		);
	
	v1 = Vector4(
		v1.x*m.v0.x + v1.y*m.v1.x + v1.z*m.v2.x + v1.w*m.v3.x,
		v1.x*m.v0.y + v1.y*m.v1.y + v1.z*m.v2.y + v1.w*m.v3.y,
		v1.x*m.v0.z + v1.y*m.v1.z + v1.z*m.v2.z + v1.w*m.v3.z,
		v1.x*m.v0.w + v1.y*m.v1.w + v1.z*m.v2.w + v1.w*m.v3.w
	);

	v2 = Vector4(
		v2.x*m.v0.x + v2.y*m.v1.x + v2.z*m.v2.x + v2.w*m.v3.x,
		v2.x*m.v0.y + v2.y*m.v1.y + v2.z*m.v2.y + v2.w*m.v3.y,
		v2.x*m.v0.z + v2.y*m.v1.z + v2.z*m.v2.z + v2.w*m.v3.z,
		v2.x*m.v0.w + v2.y*m.v1.w + v2.z*m.v2.w + v2.w*m.v3.w
	);

	v3 = Vector4(
		v3.x*m.v0.x + v3.y*m.v1.x + v3.z*m.v2.x + v3.w*m.v3.x,
		v3.x*m.v0.y + v3.y*m.v1.y + v3.z*m.v2.y + v3.w*m.v3.y,
		v3.x*m.v0.z + v3.y*m.v1.z + v3.z*m.v2.z + v3.w*m.v3.z,
		v3.x*m.v0.w + v3.y*m.v1.w + v3.z*m.v2.w + v3.w*m.v3.w
	);

	return *this;
}

Matrix4& Matrix4::operator*=(float32 s)
{
	v0.x *= s;
	v0.y *= s;
	v0.z *= s;
	v0.w *= s;

	v1.x *= s;
	v1.y *= s;
	v1.z *= s;
	v1.w *= s;

	v2.x *= s;
	v2.y *= s;
	v2.z *= s;
	v2.w *= s;

	v3.x *= s;
	v3.y *= s;
	v3.z *= s;
	v3.w *= s;
	
	return *this;
}

Matrix4 Matrix4::operator+() const
{
	return Matrix4(
		Vector4(+v0.x, +v0.y, +v0.z, +v0.w),
		Vector4(+v1.x, +v1.y, +v1.z, +v1.w),
		Vector4(+v2.x, +v2.y, +v2.z, +v2.w),
		Vector4(+v3.x, +v3.y, +v3.z, +v3.w)
	);
}

Matrix4 Matrix4::operator-() const
{
	return Matrix4(
		Vector4(-v0.x, -v0.y, -v0.z, -v0.w),
		Vector4(-v1.x, -v1.y, -v1.z, -v1.w), 
		Vector4(-v2.x, -v2.y, -v2.z, -v2.w),
		Vector4(-v3.x, -v3.y, -v3.z, -v3.w)
	);
}

float32& Matrix4::m0()
{
	return v0.x;
}

float32& Matrix4::m1()
{
	return v0.y;
}

float32& Matrix4::m2()
{
	return v0.z;
}

float32& Matrix4::m3()
{
	return v0.w;
}

float32& Matrix4::m4()
{
	return v1.x;
}

float32& Matrix4::m5()
{
	return v1.y;
}

float32& Matrix4::m6()
{
	return v1.z;
}

float32& Matrix4::m7()
{
	return v1.w;
}

float32& Matrix4::m8()
{
	return v2.x;
}

float32& Matrix4::m9()
{
	return v2.y;
}

float32& Matrix4::m10()
{
	return v2.z;
}

float32& Matrix4::m11()
{
	return v2.w;
}

float32& Matrix4::m12()
{
	return v3.x;
}

float32& Matrix4::m13()
{
	return v3.y;
}

float32& Matrix4::m14()
{
	return v3.z;
}

float32& Matrix4::m15()
{
	return v3.w;
}

const float32& Matrix4::m0() const
{
	return v0.x;
}

const float32& Matrix4::m1() const
{
	return v0.y;
}

const float32& Matrix4::m2() const
{
	return v0.z;
}

const float32& Matrix4::m3() const
{
	return v0.w;
}

const float32& Matrix4::m4() const
{
	return v1.x;
}

const float32& Matrix4::m5() const
{
	return v1.y;
}

const float32& Matrix4::m6() const
{
	return v1.z;
}

const float32& Matrix4::m7() const
{
	return v1.w;
}

const float32& Matrix4::m8() const
{
	return v2.x;
}

const float32& Matrix4::m9() const
{
	return v2.y;
}

const float32& Matrix4::m10() const
{
	return v2.z;
}

const float32& Matrix4::m11() const 
{
	return v2.w;
}

const float32& Matrix4::m12() const
{
	return v3.x;
}

const float32& Matrix4::m13() const 
{
	return v3.y;
}

const float32& Matrix4::m14() const 
{
	return v3.z;
}

const float32& Matrix4::m15() const 
{
	return v3.w;
}
float32& Matrix4::operator[](m0_enum)
{
	return v0.x;
}

float32& Matrix4::operator[](m1_enum)
{
	return v0.y;
}

float32& Matrix4::operator[](m2_enum)
{
	return v0.z;
}

float32& Matrix4::operator[](m3_enum)
{
	return v0.w;
}

float32& Matrix4::operator[](m4_enum)
{
	return v1.x;
}

float32& Matrix4::operator[](m5_enum)
{
	return v1.y;
}

float32& Matrix4::operator[](m6_enum)
{
	return v1.z;
}

float32& Matrix4::operator[](m7_enum)
{
	return v1.w;
}

float32& Matrix4::operator[](m8_enum)
{
	return v2.x;
}

float32& Matrix4::operator[](m9_enum)
{
	return v2.y;
}

float32& Matrix4::operator[](m10_enum)
{
	return v2.z;
}

float32& Matrix4::operator[](m11_enum)
{
	return v2.w;
}

float32& Matrix4::operator[](m12_enum)
{
	return v3.x;
}

float32& Matrix4::operator[](m13_enum)
{
	return v3.y;
}

float32& Matrix4::operator[](m14_enum)
{
	return v3.z;
}

float32& Matrix4::operator[](m15_enum)
{
	return v3.w;
}

const float32& Matrix4::operator[](m0_enum) const
{
	return v0.x;
}

const float32& Matrix4::operator[](m1_enum) const 
{
	return v0.y;
}

const float32& Matrix4::operator[](m2_enum) const
{
	return v0.z;
}

const float32& Matrix4::operator[](m3_enum) const
{
	return v0.w;
}

const float32& Matrix4::operator[](m4_enum) const
{
	return v1.x;
}

const float32& Matrix4::operator[](m5_enum) const
{
	return v1.y;
}

const float32& Matrix4::operator[](m6_enum) const
{
	return v1.z;
}

const float32& Matrix4::operator[](m7_enum) const
{
	return v1.w;
}

const float32& Matrix4::operator[](m8_enum) const
{
	return v2.x;
}

const float32& Matrix4::operator[](m9_enum) const
{
	return v2.y;
}

const float32& Matrix4::operator[](m10_enum) const 
{
	return v2.z;
}

const float32& Matrix4::operator[](m11_enum) const
{
	return v2.w;
}

const float32& Matrix4::operator[](m12_enum) const
{
	return v3.x;
}

const float32& Matrix4::operator[](m13_enum) const
{
	return v3.y;
}

const float32& Matrix4::operator[](m14_enum) const
{
	return v3.z;
}

const float32& Matrix4::operator[](m15_enum) const
{
	return v3.w;
}

Vector4 operator*(const Vector4& v, const Matrix4& m)
{
	Vector4 ret(v);
	return ret *= m;
}

Matrix4 operator*(float32 s, const Matrix4& m)
{
	Matrix4 ret(m);
	return ret *= s;
}

Vector4& operator*=(Vector4& v, const Matrix4& m)
{
	v = Vector4(
		v.x * m.v0.x + v.y * m.v1.x + v.z * m.v2.x + v.w * m.v3.x,
		v.x * m.v0.y + v.y * m.v1.y + v.z * m.v2.y + v.w * m.v3.y,
		v.x * m.v0.z + v.y * m.v1.z + v.z * m.v2.z + v.w * m.v3.z,
		v.x * m.v0.w + v.y * m.v1.w + v.z * m.v2.w + v.w * m.v3.w
	);
	return v;
}
