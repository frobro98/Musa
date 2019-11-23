#pragma once

#include "Math/Matrix2.hpp"
#include "Math/MathConstants.hpp"

//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 2-dimensional
//  Quaternion.  The quaternions are internally stored in terms of the
//  imaginary component and the real component.
//
//	This version of quaternion is based on Euler's formula:
//		a + i * b = cos(theta) + i * sin(theta)
// 
//------------------------------------------------------------------------

struct Quat2 final
{
	static const Quat2 Identity;
	static const Quat2 Zero;

public:
	float32 real = 1;
	float32 imaginary = 0;

	Quat2() = default;
	Quat2(float32 real, float32 imag);
	//explicit Quat(RotOrientType, const Vector4& dof, const Vector4& up);
	explicit Quat2(float32 angle);
	explicit Quat2(const Matrix2& mat);

	//Quat2& operator=(const Matrix2& m);

	// Accessors
	void Set(const Matrix2& mat);
	void Set(float32 theta);
	//void Set(RotOrientType, const Vector4& dof, const Vector4& up);

	float GetAngle() const;

	// Quaternion Functions
	Quat2& Normalize();
	Quat2 GetNormalized() const;
	Quat2 GetInverse() const;
	float Dot(const Quat2& q) const;
	void Inverse();
	void Conjugate();
	Quat2 GetConjugate() const;
	void Transpose();
	Quat2 GetTranspose() const;
	float Magnitude() const;
	float MagnitudeSqr() const;
	float InverseMagnitude() const;

	// Boolean tests
	bool IsEqual(const Quat2& q, float tolerance = Math::InternalTolerence) const;
	bool IsNegativeEqual(const Quat2& q, float tolerance = Math::InternalTolerence) const;
	bool IsEquivalent(const Quat2& q, float tolerance = Math::InternalTolerence) const;
	bool IsConjugateEqual(const Quat2& q, float tolerance = Math::InternalTolerence) const;
	bool IsIdentity(float tolerance = Math::InternalTolerence) const;
	bool IsNormalized(float tolerance = Math::InternalTolerence) const;
	bool IsZero(float tolerance = Math::InternalTolerence) const;

	Quat2 operator+() const;
	Quat2 operator-() const;

	Quat2& operator+=(const Quat2& q);
	Quat2& operator-=(const Quat2& q);
	Quat2& operator*=(const Quat2& q);

	Quat2& operator*=(float32 val);
	Quat2& operator/=(float32 val);

	friend Quat2 operator+(const Quat2& q0, const Quat2& q1);
	friend Quat2 operator-(const Quat2& q0, const Quat2& q1);

	friend Matrix2& operator*=(Matrix2& m, const Quat2& q);
	friend Quat2& operator*=(Quat2& q, const Matrix2& m_);
	friend Matrix2 operator*(const Matrix2& m, const Quat2& q);
	friend Matrix2 operator*(const Quat2& q, const Matrix2& m_);

	friend Vector2& operator*=(Vector2& v, const Quat2& q);
	friend Vector2 operator*(const Vector2& v, const Quat2& q);

	friend Quat2 operator*(const Quat2& q, float32 val);
	friend Quat2 operator*(float32 val, const Quat2& q);
	friend Quat2 operator/(const Quat2& q, float32 val);
	friend Quat2 operator/(float32 val, const Quat2& q);

};

