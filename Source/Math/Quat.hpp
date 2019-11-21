#pragma once

#include "Matrix4.hpp"

//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 4-dimensional
//  Quaternions.  The quaternions are internally stored interms of the
//  imaginary components (x,y,z) and the real components (real).
// 
//  In the Math library, quaternions can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with quaternions.  Such as quaternion concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
// 
//------------------------------------------------------------------------

struct Quat final
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;

	Quat() = default;
	explicit Quat(float x, float y, float z, float w);
	explicit Quat(RotType, float radAngle);
	explicit Quat(Rot3AxisType, float radX, float radY, float radZ);
	explicit Quat(RotAxisAngleType, const Vector4& axis, float angleRad);
	explicit Quat(RotOrientType, const Vector4& dof, const Vector4& up);
	explicit Quat(const Vector4& v, float angle);
	explicit Quat(MatrixSpecialType);
	explicit Quat(const Matrix4& mat);

	Quat& operator=(const Matrix4& m);

	// Accessors
	void Set(float x, float y, float z, float w);
	void Set(const Matrix4& mat);
	void Set(const Vector4& vect, float real);
	void Set(MatrixSpecialType);
	void Set(RotType, float radAngle);
	void Set(Rot3AxisType, float radX, float radY, float radZ);
	void Set(RotAxisAngleType, const Vector4& axis, float angleRad);
	void Set(RotOrientType, const Vector4& dof, const Vector4& up);

	void SetVector(const Vector4& v);
	Vector4 GetVector() const;
	Vector4 GetAxis() const;

	float GetAngle() const;	

	// Quaternion Functions
	Quat& Normalize();
	Quat GetNormalized() const;
	Quat GetInverse() const;
	float Dot(const Quat& q) const;
	void Inverse();
	void Conjugate();
	Quat GetConjugate() const;
	void Transpose();
	Quat GetTranspose() const;
	float Magnitude() const;
	float MagnitudeSqr() const;
	float InverseMagnitude() const;
	void Lqcvq(const Vector4& v, Vector4& vOut) const;
	void Lqvqc(const Vector4& v, Vector4& vOut) const;

	// Boolean tests
	bool IsEqual(const Quat& q, float tolerance = Math::InternalTolerence) const;
	bool IsNegativeEqual(const Quat& q, float tolerance = Math::InternalTolerence) const;
	bool IsEquivalent(const Quat& q, float tolerance = Math::InternalTolerence) const;
	bool IsConjugateEqual(const Quat& q, float tolerance = Math::InternalTolerence) const;
	bool IsIdentity(float tolerance = Math::InternalTolerence) const;
	bool IsNormalized(float tolerance = Math::InternalTolerence) const;
	bool IsZero(float tolerance = Math::InternalTolerence) const;

	// Math operations
	Quat MultiplyByElement(const Quat& q) const;

	Quat operator+() const;
	Quat operator-() const;

	Quat& operator+=(const Quat& q);
	Quat& operator-=(const Quat& q);
	Quat& operator*=(const Quat& q);
	Quat& operator/=(const Quat& q);

	Quat operator+(const Quat& q) const;
	Quat operator-(const Quat& q) const;
	Quat operator*(const Quat& q) const;
	Quat operator/(const Quat& q) const;

	friend Matrix4& operator*=(Matrix4& m, const Quat& q);
	friend Quat& operator*=(Quat& q, const Matrix4& m_);
	friend Matrix4 operator*(const Matrix4& m, const Quat& q);
	friend Matrix4 operator*(const Quat& q, const Matrix4& m_);

	friend Vector4& operator*=(Vector4& v, const Quat& q);
	friend Vector4 operator*(const Vector4& v, const Quat& q);
	friend Vector4 operator*(const Quat& q, const Vector4& v);

	friend Quat& operator+=(Quat& q, float val);
	friend Quat& operator-=(Quat& q, float val);
	friend Quat& operator*=(Quat& q, float val);
	friend Quat& operator/=(Quat& q, float val);

	friend Quat operator+(const Quat& q, float val);
	friend Quat operator+(float val, const Quat& q);
	friend Quat operator-(const Quat& q, float val);
	friend Quat operator-(float val, const Quat& q);
	friend Quat operator*(const Quat& q, float val);
	friend Quat operator*(float val, const Quat& q);
	friend Quat operator/(const Quat& q, float val);
	friend Quat operator/(float val, const Quat& q);

};
