// Copyright 2020, Nathan Blane

#include "Matrix4.hpp"
#include "Vector4.hpp"
#include "Quat.hpp"
#include "MathFunctions.hpp"
#include "Assertion.h"

Matrix4::Matrix4()
	: _m0(0.f), _m1(0.f), _m2(0.f), _m3(0.f),
	  _m4(0.f), _m5(0.f), _m6(0.f), _m7(0.f),
	  _m8(0.f), _m9(0.f), _m10(0.f), _m11(0.f),
	  _m12(0.f), _m13(0.f), _m14(0.f), _m15(0.f)
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

Matrix4::Matrix4(MatrixTransType transEnum, float x, float y, float z)
{
	Set(transEnum, x, y, z);
}

Matrix4::Matrix4(RotType rotationEnum, float angle)
{
	Set(rotationEnum, angle);
}

Matrix4::Matrix4(Rot3AxisType type, float xAngleRad, float yAngleRad, float zAngleRad)
{
	Set(type, xAngleRad, yAngleRad, zAngleRad);
}

Matrix4::Matrix4(RotAxisAngleType type, const Vector4& vect, float angleRads)
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

Matrix4::Matrix4(MatrixScaleType scaleEnum, float sx, float sy, float sz)
{
	Set(scaleEnum, sx, sy, sz);
}

Matrix4::Matrix4(const Quat& q)
{
	Set(q);
}

Matrix4::Matrix4(const Matrix4& other)
{
	_m0 = other._m0;
	_m1 = other._m1;
	_m2 = other._m2;
	_m3 = other._m3;
	_m4 = other._m4;
	_m5 = other._m5;
	_m6 = other._m6;
	_m7 = other._m7;
	_m8 = other._m8;
	_m9 = other._m9;
	_m10 = other._m10;
	_m11 = other._m11;
	_m12 = other._m12;
	_m13 = other._m13;
	_m14 = other._m14;
	_m15 = other._m15;
}

Matrix4::Matrix4(Matrix4&& other) noexcept
	: v0(std::move(other.v0)),
	  v1(std::move(other.v1)),
	  v2(std::move(other.v2)),
	  v3(std::move(other.v3))
{
}

Matrix4& Matrix4::operator=(const Matrix4& m)
{
	if (this != &m)
	{
		_m0 = m._m0;
		_m1 = m._m1;
		_m2 = m._m2;
		_m3 = m._m3;
		_m4 = m._m4;
		_m5 = m._m5;
		_m6 = m._m6;
		_m7 = m._m7;
		_m8 = m._m8;
		_m9 = m._m9;
		_m10 = m._m10;
		_m11 = m._m11;
		_m12 = m._m12;
		_m13 = m._m13;
		_m14 = m._m14;
		_m15 = m._m15;
	}

	return *this;
}

Matrix4& Matrix4::operator=(Matrix4&& m) noexcept
{
	if (this != &m)
	{
		v0 = std::move(m.v0);
		v1 = std::move(m.v1);
		v2 = std::move(m.v2);
		v3 = std::move(m.v3);
	}

	return *this;
}

void Matrix4::Set(MatrixTransType /*transEnum*/, const Vector4& transVec)
{
	v0 = Vector4(1.f, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, 1.f, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, 1.f, 0.f);
	v3 = Vector4(transVec.x, transVec.y, transVec.z, 1.f);
}

void Matrix4::Set(MatrixTransType /*transEnum*/, float x, float y, float z)
{
	v0 = Vector4(1.f, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, 1.f, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, 1.f, 0.f);
	v3 = Vector4(x, y, z, 1.f);
}

void Matrix4::Set(RotType rotationEnum, float angle)
{
	switch (rotationEnum)
	{
		case ROT_X:
		{
			_m0 = 1.f;
			_m1 = 0.f;
			_m2 = 0.f;
			_m3 = 0.f;

			_m4 = 0.f;
			_m5 = Math::Cos(angle);
			_m6 = Math::Sin(angle);
			_m7 = 0.f;

			_m8 = 0.f;
			_m9 = -Math::Sin(angle);
			_m10 = Math::Cos(angle);
			_m11 = 0.f;

			_m12 = 0.f;
			_m13 = 0.f;
			_m14 = 0.f;
			_m15 = 1.f;
		}break;
		case ROT_Y:
		{
			_m0 = Math::Cos(angle);
			_m1 = 0.f;
			_m2 = -Math::Sin(angle);
			_m3 = 0.f;

			_m4 = 0.f;
			_m5 = 1.f;
			_m6 = 0.f;
			_m7 = 0.f;

			_m8 = Math::Sin(angle);
			_m9 = 0;
			_m10 = Math::Cos(angle);
			_m11 = 0.f;

			_m12 = 0.f;
			_m13 = 0.f;
			_m14 = 0.f;
			_m15 = 1.f;
		}break;
		case ROT_Z:
		{
			_m0 = Math::Cos(angle);
			_m1 = Math::Sin(angle);
			_m2 = 0.f;
			_m3 = 0.f;

			_m4 = -Math::Sin(angle);
			_m5 = Math::Cos(angle);
			_m6 = 0.f;
			_m7 = 0.f;

			_m8 = 0.f;
			_m9 = 0.f;
			_m10 = 1.f;
			_m11 = 0.f;

			_m12 = 0.f;
			_m13 = 0.f;
			_m14 = 0.f;
			_m15 = 1.f;
		}break;
		default:
		{
			Assert(false);
		}
	}
}

void Matrix4::Set(Rot3AxisType, float xRad, float yRad, float zRad)
{
	Quat qX(ROT_X, xRad);
	Quat qY(ROT_Y, yRad);
	Quat qZ(ROT_Z, zRad);

	Set(qX*qY*qZ);
}

void Matrix4::Set(RotAxisAngleType type, const Vector4& vect, float angleRads)
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
	_m0 = 1.f - 2.f * (q.y * q.y + q.z * q.z);
	_m1 =		2.f * (q.x * q.y + q.w * q.z);
	_m2 =		2.f * (q.x * q.z - q.w * q.y);
	_m3 = 0.f;

	_m4 =		2.f * (q.x * q.y - q.w * q.z);
	_m5 = 1.f - 2.f * (q.x * q.x + q.z * q.z);
	_m6 =		2.f * (q.y * q.z + q.w * q.x);
	_m7 = 0.f;

	_m8 =		2.f * (q.x * q.z + q.w * q.y);
	_m9 =		2.f * (q.y * q.z - q.w * q.x);
	_m10= 1.f - 2.f * (q.x * q.x + q.y * q.y);
	_m11 = 0.f;

	_m12 = 0.f;
	_m13 = 0.f;
	_m14 = 0.f;
	_m15 = 1.f;
}

void Matrix4::Set(MatrixScaleType /*scaleEnum*/, const Vector4& scaleVec)
{
	v0 = Vector4(scaleVec.x, 0.f, 0.f, 0.f);
	v1 = Vector4(0.f, scaleVec.y, 0.f, 0.f);
	v2 = Vector4(0.f, 0.f, scaleVec.z, 0.f);
	v3 = Vector4(0.f, 0.f, 0.f, 1.f);
}

void Matrix4::Set(MatrixScaleType /*scaleEnum*/, float sx, float sy, float sz)
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

void Matrix4::Set(const Vector4 &axis, float angle)
{
	// angle
	// axis;
	const float angle_a = 0.5f * angle;
	float cos_a;
	float sin_a;

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
	float x2, y2, z2;
	float xx, xy, xz;
	float yy, yz, zz;
	float wx, wy, wz;

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

	_m0 = 1.0f - (yy + zz);
	_m1 = xy + wz;
	_m2 = xz - wy;
	_m3 = 0.0f;

	_m4 = xy - wz;
	_m5 = 1.0f - (xx + zz);
	_m6 = yz + wx;
	_m7 = 0.0f;

	_m8 = xz + wy;
	_m9 = yz - wx;
	_m10 = 1.0f - (xx + yy);
	_m11 = 0.0f;

	v3 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
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

float Matrix4::Determinant() const
{
	const float a = _m9*_m15 - _m11*_m13;
	const float b = _m8*_m14 -_m10*_m12;

	const float l01 = a;
	const float l20 = a;
	const float l12 = b;
	const float l31 = b;
	const float l00 = _m10*_m15 - _m11*_m14;
	const float l02 = _m9*_m14 - _m10*_m13;
	const float l11 = _m8*_m15 - _m11*_m12;
	const float l22 = _m8*_m13 - _m9*_m12;
	

	const float det00 = _m5*(l00) - _m6*(l01) + _m7*(l02);
	const float det01 = _m4*(l00) - _m6*(l11) + _m7*(l12);
	const float det02 = _m4*(l20) - _m5*(l11) + _m7*(l22);
	const float det03 = _m4*(l02) - _m5*(l31) + _m6*(l22);

	return _m0*det00 - _m1*det01 + _m2*det02 - _m3*det03;
}

void Matrix4::Inverse()
{
	float detScalar = Determinant();
	if (Math::IsNonZero(detScalar))
	{
		detScalar = 1.f / detScalar;

		// Caching multiplications
		float m2Xm7 = _m2*_m7;
		float m3Xm6 = _m3*_m6;
		float m3Xm5 = _m3*_m5;
		float m1Xm7 = _m1*_m7;
		float m1Xm6 = _m1*_m6;
		float m2Xm5 = _m2*_m5;
		float m3Xm4 = _m3*_m4;
		float m0Xm7 = _m0*_m7;
		float m4Xm10 = _m4*_m10;
		float m2Xm4 = _m2*_m4;
		float m0Xm6 = _m0*_m6;
		float m5Xm10 = _m5*_m10;
		float m1Xm4 = _m1*_m4;
		float m0Xm5 = _m0*_m5;

		float v0x = _m13*(_m6*_m11 - _m7*_m10) + _m14*(_m7*_m9 - _m5*_m11) - _m6*_m9*_m15 + m5Xm10*_m15;
		float v0y = _m13*(_m3*_m10 - _m2*_m11) - _m14*(_m3*_m9 + _m1*_m11) + _m15*(_m2*_m9 - _m1*_m10);
		float v0z = _m13*(m2Xm7 - m3Xm6)       + _m14*(m3Xm5   - m1Xm7)    - _m15*(m2Xm5 + m1Xm6);
		float v0w = _m9*(m3Xm6 - m2Xm7)        - _m10*(m3Xm5   + m1Xm7)    + _m11*(m2Xm5 - m1Xm6);

		float v1x = _m12*(_m7*_m10 - _m6*_m11) - _m14*(_m7*_m8 + _m4*_m11) + _m15*(_m6*_m8 - m4Xm10);
		float v1y = _m12*(_m2*_m11 - _m3*_m10) + _m14*(_m3*_m8 - _m0*_m11) - _m2*_m8*_m15 + _m0*_m10*_m15;
		float v1z = _m12*(m3Xm6 - m2Xm7)       - _m14*(m3Xm4   + m0Xm7)    + _m15*(m2Xm4 - m0Xm6);
		float v1w = _m8*(m2Xm7 - m3Xm6)        + _m10*(m3Xm4   - m0Xm7)    - _m11*(m2Xm4 + m0Xm6);

		float v2x = _m12*(_m5*_m11 - _m7*_m9) + _m13*(_m7*_m8 - _m4*_m11)  - _m5*_m8*_m15 + _m4*_m9*_m15;
		float v2y = _m12*(_m3*_m9 - _m1*_m11) - _m13*(_m3*_m8 + _m0*_m11)  + _m15*(_m1*_m8 - _m0*_m9);
		float v2z = _m12*(m1Xm7 - m3Xm5)      + _m13*(m3Xm4 - m0Xm7)       - m1Xm4*_m15  + m0Xm5*_m15;
		float v2w = _m8*(m3Xm5 - m1Xm7)       - _m9*(m3Xm4 + m0Xm7)        + _m11*(m1Xm4 - m0Xm5);

		float v3x = _m12*(_m6*_m9 - m5Xm10)   - _m6*_m8*_m13  + m4Xm10*_m13 + _m14*(_m5*_m8 - _m4*_m9);
		float v3y = _m12*(_m1*_m10 - _m2*_m9) + _m13*(_m2*_m8 - _m0*_m10)   - _m1*_m8*_m14 + _m0*_m9*_m14;
		float v3z = _m12*(m2Xm5 - m1Xm6)      - m2Xm4*_m13    + m0Xm6*_m13  + _m14*(m1Xm4 - m0Xm5);
		float v3w = _m8*(m1Xm6 - m2Xm5)       + _m9*(m2Xm4    - m0Xm6)      - _m1*m4Xm10 + _m0*m5Xm10;

		v0 = Vector4(
			v0x * detScalar,
			v0y * detScalar,
			v0z * detScalar,
			v0w * detScalar
		);
		v1 = Vector4(
			v1x * detScalar,
			v1y * detScalar,
			v1z * detScalar,
			v1w * detScalar
		);
		v2 = Vector4(
			v2x * detScalar,
			v2y * detScalar,
			v2z * detScalar,
			v2w * detScalar
		);
		v3 = Vector4(
			v3x * detScalar,
			v3y * detScalar,
			v3z * detScalar,
			v3w * detScalar
		);


	}
}

Matrix4 Matrix4::GetInverse() const
{
	float detScalar = Determinant();
	if (Math::IsNonZero(detScalar))
	{
		detScalar = 1.f / detScalar;

		// Caching multiplications
		float m2Xm7 = _m2*_m7;
		float m3Xm6 = _m3*_m6;
		float m3Xm5 = _m3*_m5;
		float m1Xm7 = _m1*_m7;
		float m1Xm6 = _m1*_m6;
		float m2Xm5 = _m2*_m5;
		float m3Xm4 = _m3*_m4;
		float m0Xm7 = _m0*_m7;
		float m4Xm10 = _m4*_m10;
		float m2Xm4 = _m2*_m4;
		float m0Xm6 = _m0*_m6;
		float m5Xm10 = _m5*_m10;
		float m1Xm4 = _m1*_m4;
		float m0Xm5 = _m0*_m5;

		float v0x = _m13*(_m6*_m11 - _m7*_m10) + _m14*(_m7*_m9 - _m5*_m11) - _m6*_m9*_m15 + m5Xm10*_m15;
		float v0y = _m13*(_m3*_m10 - _m2*_m11) - _m14*(_m3*_m9 + _m1*_m11) + _m15*(_m2*_m9 - _m1*_m10);
		float v0z = _m13*(m2Xm7 - m3Xm6)       + _m14*(m3Xm5   - m1Xm7)    - _m15*(m2Xm5 + m1Xm6);
		float v0w = _m9*(m3Xm6 - m2Xm7)        - _m10*(m3Xm5   + m1Xm7)    + _m11*(m2Xm5 - m1Xm6);

		float v1x = _m12*(_m7*_m10 - _m6*_m11) - _m14*(_m7*_m8 + _m4*_m11) + _m15*(_m6*_m8 - m4Xm10);
		float v1y = _m12*(_m2*_m11 - _m3*_m10) + _m14*(_m3*_m8 - _m0*_m11) - _m2*_m8*_m15 + _m0*_m10*_m15;
		float v1z = _m12*(m3Xm6 - m2Xm7)       - _m14*(m3Xm4   + m0Xm7)    + _m15*(m2Xm4 - m0Xm6);
		float v1w = _m8*(m2Xm7 - m3Xm6)        + _m10*(m3Xm4   - m0Xm7)    - _m11*(m2Xm4 + m0Xm6);

		float v2x = _m12*(_m5*_m11 - _m7*_m9) + _m13*(_m7*_m8 - _m4*_m11)  - _m5*_m8*_m15 + _m4*_m9*_m15;
		float v2y = _m12*(_m3*_m9 - _m1*_m11) - _m13*(_m3*_m8 + _m0*_m11)  + _m15*(_m1*_m8 - _m0*_m9);
		float v2z = _m12*(m1Xm7 - m3Xm5)      + _m13*(m3Xm4 - m0Xm7)       - m1Xm4*_m15  + m0Xm5*_m15;
		float v2w = _m8*(m3Xm5 - m1Xm7)       - _m9*(m3Xm4 + m0Xm7)        + _m11*(m1Xm4 - m0Xm5);

		float v3x = _m12*(_m6*_m9 - m5Xm10)   - _m6*_m8*_m13  + m4Xm10*_m13 + _m14*(_m5*_m8 - _m4*_m9);
		float v3y = _m12*(_m1*_m10 - _m2*_m9) + _m13*(_m2*_m8 - _m0*_m10)   - _m1*_m8*_m14 + _m0*_m9*_m14;
		float v3z = _m12*(m2Xm5 - m1Xm6)      - m2Xm4*_m13    + m0Xm6*_m13  + _m14*(m1Xm4 - m0Xm5);
		float v3w = _m8*(m1Xm6 - m2Xm5)       + _m9*(m2Xm4    - m0Xm6)      - _m1*m4Xm10 + _m0*m5Xm10;

		Matrix4 m;
		m.v0 = Vector4(
			v0x * detScalar,
			v0y * detScalar,
			v0z * detScalar,
			v0w * detScalar
		);
		m.v1 = Vector4(
			v1x * detScalar,
			v1y * detScalar,
			v1z * detScalar,
			v1w * detScalar
		);
		m.v2 = Vector4(
			v2x * detScalar,
			v2y * detScalar,
			v2z * detScalar,
			v2w * detScalar
		);
		m.v3 = Vector4(
			v3x * detScalar,
			v3y * detScalar,
			v3z * detScalar,
			v3w * detScalar
		);

		return m;
	}

	return Matrix4();
}

void Matrix4::Transpose()
{
	float tmp = _m1;
	_m1 = _m4;
	_m4 = tmp;

	tmp = _m2;
	_m2 = _m8;
	_m8 = tmp;

	tmp = _m3;
	_m3 = _m12;
	_m12 = tmp;

	tmp = _m13;
	_m13 = _m7;
	_m7 = tmp;

	tmp = _m14;
	_m14 = _m11;
	_m11 = tmp;

	// middle
	tmp = _m9;
	_m9 = _m6;
	_m6 = tmp;
}

Matrix4 Matrix4::GetTranspose() const
{
	Matrix4 m(*this);

	float tmp = m._m1;
	m._m1 = m._m4;
	m._m4 = tmp;

	tmp = m._m2;
	m._m2 = m._m8;
	m._m8 = tmp;

	tmp = m._m3;
	m._m3 = m._m12;
	m._m12 = tmp;

	tmp = m._m13;
	m._m13 = m._m7;
	m._m7 = tmp;

	tmp = m._m14;
	m._m14 = m._m11;
	m._m11 = tmp;

	// middle
	tmp = m._m9;
	m._m9 = m._m6;
	m._m6 = tmp;

	return m;
}

bool Matrix4::IsIdentity(float epsilon) const
{
	return Math::IsEqual(_m0, 1.f, epsilon) && Math::IsEqual(_m1, 0.f, epsilon) && 
		   Math::IsEqual(_m2, 0.f, epsilon) && Math::IsEqual(_m3, 0.f, epsilon) &&
		   Math::IsEqual(_m4, 0.f, epsilon) && Math::IsEqual(_m5, 1.f, epsilon) && 
		   Math::IsEqual(_m6, 0.f, epsilon) && Math::IsEqual(_m7, 0.f, epsilon) &&
		   Math::IsEqual(_m8, 0.f, epsilon) && Math::IsEqual(_m9, 0.f, epsilon) && 
		   Math::IsEqual(_m10, 1.f, epsilon) && Math::IsEqual(_m11, 0.f, epsilon) &&
		   Math::IsEqual(_m12, 0.f, epsilon) && Math::IsEqual(_m13, 0.f, epsilon) && 
		   Math::IsEqual(_m14, 0.f, epsilon) && Math::IsEqual(_m15, 1.f, epsilon);
}

bool Matrix4::IsEqual(const Matrix4& m) const
{
	return v0.IsEqual(m.v0) &&
		v1.IsEqual(m.v1) &&
		v2.IsEqual(m.v2) &&
		v3.IsEqual(m.v3);
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

Matrix4 Matrix4::operator*(float s) const
{
	Matrix4 m(*this);
	return m *= s;
}

Matrix4& Matrix4::operator+=(const Matrix4& m)
{
	_m0 += m._m0;
	_m1 += m._m1;
	_m2 += m._m2;
	_m3 += m._m3;
	_m4 += m._m4;
	_m5 += m._m5;
	_m6 += m._m6;
	_m7 += m._m7;
	_m8 += m._m8;
	_m9 += m._m9;
	_m10 += m._m10;
	_m11 += m._m11;
	_m12 += m._m12;
	_m13 += m._m13;
	_m14 += m._m14;
	_m15 += m._m15;

	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& m)
{
	_m0 -= m._m0;
	_m1 -= m._m1;
	_m2 -= m._m2;
	_m3 -= m._m3;
	_m4 -= m._m4;
	_m5 -= m._m5;
	_m6 -= m._m6;
	_m7 -= m._m7;
	_m8 -= m._m8;
	_m9 -= m._m9;
	_m10 -= m._m10;
	_m11 -= m._m11;
	_m12 -= m._m12;
	_m13 -= m._m13;
	_m14 -= m._m14;
	_m15 -= m._m15;

	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& m)
{
	v0 = Vector4(
		_m0*m._m0 + _m1*m._m4 + _m2*m._m8 + _m3*m._m12,
		_m0*m._m1 + _m1*m._m5 + _m2*m._m9 + _m3*m._m13,
		_m0*m._m2 + _m1*m._m6 + _m2*m._m10 + _m3*m._m14,
		_m0*m._m3 + _m1*m._m7 + _m2*m._m11 + _m3*m._m15
		);
	
	v1 = Vector4(
		_m4*m._m0 + _m5*m._m4 + _m6*m._m8+ _m7*m._m12,
		_m4*m._m1 + _m5*m._m5 + _m6*m._m9+ _m7*m._m13,
		_m4*m._m2 + _m5*m._m6 + _m6*m._m10 + _m7*m._m14,
		_m4*m._m3 + _m5*m._m7 + _m6*m._m11 + _m7*m._m15
	);

	v2 = Vector4(
		_m8*m._m0 + _m9*m._m4 + _m10*m._m8 + _m11*m._m12,
		_m8*m._m1 + _m9*m._m5 + _m10*m._m9 + _m11*m._m13,
		_m8*m._m2 + _m9*m._m6 + _m10*m._m10 + _m11*m._m14,
		_m8*m._m3 + _m9*m._m7 + _m10*m._m11 + _m11*m._m15
	);

	v3 = Vector4(
		_m12*m._m0 + _m13*m._m4 + _m14*m._m8 + _m15*m._m12,
		_m12*m._m1 + _m13*m._m5 + _m14*m._m9 + _m15*m._m13,
		_m12*m._m2 + _m13*m._m6 + _m14*m._m10 + _m15*m._m14,
		_m12*m._m3 + _m13*m._m7 + _m14*m._m11 + _m15*m._m15
	);

	return *this;
}

Matrix4& Matrix4::operator*=(float s)
{
	_m0 *= s;
	_m1 *= s;
	_m2 *= s;
	_m3 *= s;
	_m4 *= s;
	_m5 *= s;
	_m6 *= s;
	_m7 *= s;
	_m8 *= s;
	_m9 *= s;
	_m10 *= s;
	_m11 *= s;
	_m12 *= s;
	_m13 *= s;
	_m14 *= s;
	_m15 *= s;

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

float & Matrix4::m0()
{
	return _m0;
}

float & Matrix4::m1()
{
	return _m1;
}

float & Matrix4::m2()
{
	return _m2;
}

float & Matrix4::m3()
{
	return _m3;
}

float & Matrix4::m4()
{
	return _m4;
}

float & Matrix4::m5()
{
	return _m5;
}

float & Matrix4::m6()
{
	return _m6;
}

float & Matrix4::m7()
{
	return _m7;
}

float & Matrix4::m8()
{
	return _m8;
}

float & Matrix4::m9()
{
	return _m9;
}

float & Matrix4::m10()
{
	return _m10;
}

float & Matrix4::m11()
{
	return _m11;
}

float & Matrix4::m12()
{
	return _m12;
}

float & Matrix4::m13()
{
	return _m13;
}

float & Matrix4::m14()
{
	return _m14;
}

float & Matrix4::m15()
{
	return _m15;
}

const float & Matrix4::m0() const
{
	return _m0;
}

const float & Matrix4::m1() const
{
	return _m1;
}

const float & Matrix4::m2() const
{
	return _m2;
}

const float & Matrix4::m3() const
{
	return _m3;
}

const float & Matrix4::m4() const
{
	return _m4;
}

const float & Matrix4::m5() const
{
	return _m5;
}

const float & Matrix4::m6() const
{
	return _m6;
}

const float & Matrix4::m7() const
{
	return _m7;
}

const float & Matrix4::m8() const
{
	return _m8;
}

const float & Matrix4::m9() const
{
	return _m9;
}

const float & Matrix4::m10() const
{
	return _m10;
}

const float & Matrix4::m11() const
{
	return _m11;
}

const float & Matrix4::m12() const
{
	return _m12;
}

const float & Matrix4::m13() const
{
	return _m13;
}

const float & Matrix4::m14() const
{
	return _m14;
}

const float & Matrix4::m15() const
{
	return _m15;
}

float & Matrix4::operator[](m0_enum)
{
	return _m0;
}

float & Matrix4::operator[](m1_enum)
{
	return _m1;
}

float & Matrix4::operator[](m2_enum)
{
	return _m2;
}

float & Matrix4::operator[](m3_enum)
{
	return _m3;
}

float & Matrix4::operator[](m4_enum)
{
	return _m4;
}

float & Matrix4::operator[](m5_enum)
{
	return _m5;
}

float & Matrix4::operator[](m6_enum)
{
	return _m6;
}

float & Matrix4::operator[](m7_enum)
{
	return _m7;
}

float & Matrix4::operator[](m8_enum)
{
	return _m8;
}

float & Matrix4::operator[](m9_enum)
{
	return _m9;
}

float & Matrix4::operator[](m10_enum)
{
	return _m10;
}

float & Matrix4::operator[](m11_enum)
{
	return _m11;
}

float & Matrix4::operator[](m12_enum)
{
	return _m12;
}

float & Matrix4::operator[](m13_enum)
{
	return _m13;
}

float & Matrix4::operator[](m14_enum)
{
	return _m14;
}

float & Matrix4::operator[](m15_enum)
{
	return _m15;
}

const float & Matrix4::operator[](m0_enum) const
{
	return _m0;
}

const float & Matrix4::operator[](m1_enum) const
{
	return _m1;
}

const float & Matrix4::operator[](m2_enum) const
{
	return _m2;
}

const float & Matrix4::operator[](m3_enum) const
{
	return _m3;
}

const float & Matrix4::operator[](m4_enum) const
{
	return _m4;
}

const float & Matrix4::operator[](m5_enum) const
{
	return _m5;
}

const float & Matrix4::operator[](m6_enum) const
{
	return _m6;
}

const float & Matrix4::operator[](m7_enum) const
{
	return _m7;
}

const float & Matrix4::operator[](m8_enum) const
{
	return _m8;
}

const float & Matrix4::operator[](m9_enum) const
{
	return _m9;
}

const float & Matrix4::operator[](m10_enum) const
{
	return _m10;
}

const float & Matrix4::operator[](m11_enum) const
{
	return _m11;
}

const float & Matrix4::operator[](m12_enum) const
{
	return _m12;
}

const float & Matrix4::operator[](m13_enum) const
{
	return _m13;
}

const float & Matrix4::operator[](m14_enum) const
{
	return _m14;
}

const float & Matrix4::operator[](m15_enum) const
{
	return _m15;
}

Vector4 operator*(const Vector4& v, const Matrix4& m)
{
	Vector4 ret(v);
	return ret *= m;
}

Matrix4 operator*(float s, const Matrix4& m)
{
	Matrix4 ret(m);
	return ret *= s;
}

Vector4 & operator*=(Vector4& v, const Matrix4& m)
{
	v = Vector4(
		v.x * m.m0() + v.y * m.m4() + v.z * m.m8()  + v.w * m.m12(),
		v.x * m.m1() + v.y * m.m5() + v.z * m.m9()  + v.w * m.m13(),
		v.x * m.m2() + v.y * m.m6() + v.z * m.m10() + v.w * m.m14(),
		v.x * m.m3() + v.y * m.m7() + v.z * m.m11() + v.w * m.m15()
	);
	return v;
}
