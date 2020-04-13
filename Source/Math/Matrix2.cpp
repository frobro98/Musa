#include "Matrix2.hpp"
#include "Quat2.hpp"
#include "MathFunctions.hpp"

Matrix2::Matrix2()
	: _m0(0), _m1(0),
	_m2(0), _m3(0)
{
}

Matrix2::Matrix2(const Vector2& row0, const Vector2& row1)
	: v0(row0), v1(row1)
{
}

Matrix2::Matrix2(float angle)
{
	float32 cosTheta = Math::Cos(angle);
	float32 sinTheta = Math::Sin(angle);
	_m0 = cosTheta;
	_m1 = sinTheta;
	_m2 = -sinTheta;
	_m3 = cosTheta;
}

Matrix2::Matrix2(MatrixScaleType, float32 uniformScale)
	: _m0(uniformScale), _m1(0),
	_m2(0), _m3(uniformScale)
{
}

Matrix2::Matrix2(MatrixScaleType, const Vector2& scaleVec)
	: _m0(scaleVec.x), _m1(0),
	_m2(0), _m3(scaleVec.y)
{
}

Matrix2::Matrix2(MatrixScaleType, float sx, float sy)
	: _m0(sx), _m1(0),
	_m2(0), _m3(sy)
{
}

Matrix2::Matrix2(const Quat2& q)
	: _m0(q.real), _m1(q.imaginary),
	_m2(-q.imaginary), _m3(q.real)
{
}

Matrix2::Matrix2(const Matrix2& other)
	: _m0(other._m0), _m1(other._m1),
	_m2(other._m2), _m3(other._m3)
{
}

Matrix2::Matrix2(Matrix2&& other) noexcept
	: _m0(other._m0), _m1(other._m1),
	_m2(other._m2), _m3(other._m3)
{
}

Matrix2& Matrix2::operator=(const Matrix2& m)
{
	if (this != &m)
	{
		_m0 = m._m0;
		_m1 = m._m1;
		_m2 = m._m2;
		_m3 = m._m3;
	}
	return *this;
}

Matrix2& Matrix2::operator=(Matrix2&& m) noexcept
{
	if(this != &m)
	{
		_m0 = m._m0;
		_m1 = m._m1;
		_m2 = m._m2;
		_m3 = m._m3;
	}
	return *this;

}

void Matrix2::Set(float angle)
{
	float32 cosTheta = Math::Cos(angle);
	float32 sinTheta = Math::Sin(angle);
	_m0 = cosTheta;
	_m1 = sinTheta;
	_m2 = -sinTheta;
	_m3 = cosTheta;
}

void Matrix2::Set(const Quat2& q)
{
	_m0 = q.real;
	_m1 = q.imaginary;
	_m2 = -q.imaginary;
	_m3 = q.real;
}

void Matrix2::Set(MatrixScaleType, float32 scale)
{
	_m0 = scale;
	_m1 = 0;
	_m2 = 0;
	_m3 = scale;
}

void Matrix2::Set(MatrixScaleType, const Vector2& scaleVec)
{
	_m0 = scaleVec.x;
	_m1 = 0;
	_m2 = 0;
	_m3 = scaleVec.y;
}

void Matrix2::Set(MatrixScaleType, float32 sx, float32 sy)
{
	_m0 = sx;
	_m1 = 0;
	_m2 = 0;
	_m3 = sy;
}

void Matrix2::Set(const Vector2& row0, const Vector2& row1)
{
	v0 = row0;
	v1 = row1;
}

float Matrix2::Determinant() const
{
	return _m0 * _m3 - _m1 * _m2;
}

void Matrix2::Inverse()
{
	float32 detScalar = Determinant();
	if (Math::IsNonZero(detScalar))
	{
		float32 invDetScalar = 1.f / detScalar;

		// Transposed cofactor of this matrix
		float32 cofM0 = _m3;
		float32 cofM3 = _m0;
		float32 cofM1 = -_m2;
		float32 cofM2 = -_m1;

		v0 = Vector2(
			cofM0 * invDetScalar,
			cofM1 * invDetScalar
		);
		v1 = Vector2(
			cofM2 * invDetScalar,
			cofM3 * invDetScalar
		);
	}
}

Matrix2 Matrix2::GetInverse() const
{
	Matrix2 m(*this);
	m.Inverse();
	return m;
}

void Matrix2::Transpose()
{
	float32 tmp = _m1;
	_m1 = _m2;
	_m2 = tmp;
}

Matrix2 Matrix2::GetTranspose() const
{
	Matrix2 m(*this);
	m.Transpose();
	return m;
}

bool Matrix2::IsIdentity(float epsilon) const
{
	return Math::IsEqual(_m0, 1.f, epsilon) && Math::IsEqual(_m1, 0.f, epsilon) &&
		Math::IsEqual(_m2, 0.f, epsilon) && Math::IsEqual(_m3, 0.f, epsilon);
}

bool Matrix2::IsEqual(const Matrix2& m) const
{
	return v0.IsEqual(m.v0) &&
		v1.IsEqual(m.v1);
}
