// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Vector2.hpp"
#include "Math/MathDefinitions.hpp"

struct Quat2;

class Matrix2
{
public:
	static const Matrix2 Identity;
	static const Matrix2 Zero;

public:
	Matrix2();
	explicit Matrix2(const Vector2& row0, const Vector2& row1);
	explicit Matrix2(float angle);
	//explicit Matrix2(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	explicit Matrix2(MatrixScaleType, float32 uniformScale);
	explicit Matrix2(MatrixScaleType, const Vector2& scaleVec);
	explicit Matrix2(MatrixScaleType, float sx, float sy);
	Matrix2(const Quat2& q);

	Matrix2(const Matrix2& other);
	Matrix2(Matrix2&& other) noexcept;

	Matrix2& operator=(const Matrix2& m);
	Matrix2& operator=(Matrix2&& m) noexcept;

	// Setting specific matrix types
	void Set(float angle);
	//void Set(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	void Set(const Quat2& q);
	void Set(MatrixScaleType scaleEnum, float32 scale);
	void Set(MatrixScaleType scaleEnum, const Vector2& scaleVec);
	void Set(MatrixScaleType scaleEnum, float32 sx, float32 sy);
	void Set(const Vector2& row0, const Vector2& row1);

	// Matrix operations

	// Determinant
	float Determinant() const;

	// Inverse
	void Inverse();
	Matrix2 GetInverse() const;

	// Transpose
	void Transpose();
	Matrix2 GetTranspose() const;

	bool IsIdentity(float epsilon = Math::InternalTolerence) const;
	bool IsEqual(const Matrix2& m) const;

	// Math operators
	Matrix2 operator+(const Matrix2& m) const;
	Matrix2 operator-(const Matrix2& m) const;
	Matrix2 operator*(const Matrix2& m) const;
	Matrix2 operator*(float s) const;

	Matrix2& operator+=(const Matrix2& m);
	Matrix2& operator-=(const Matrix2& m);
	Matrix2 operator*=(const Matrix2& m);
	Matrix2& operator*=(float s);

	friend Vector4 operator*(const Vector4& v, const Matrix2& m);
	friend Matrix2 operator*(float s, const Matrix2& m);
	friend Vector4& operator*=(Vector4& v, const Matrix2& m);

	// Unary operators
	Matrix2 operator+() const;
	Matrix2 operator-() const;


	// Mutable individual getters
	float& m0();
	float& m1();
	float& m2();
	float& m3();

	// Non-mutable individual getters
	const float& m0() const;
	const float& m1() const;
	const float& m2() const;
	const float& m3() const;


	// Mutable bracket operators
	float& operator[](m0_enum);
	float& operator[](m1_enum);
	float& operator[](m2_enum);
	float& operator[](m3_enum);

	// Non-mutable bracket operators
	const float& operator[](m0_enum) const;
	const float& operator[](m1_enum) const;
	const float& operator[](m2_enum) const;
	const float& operator[](m3_enum) const;


private:

	union
	{
		struct
		{
			Vector2 v0;
			Vector2 v1;
		};

		struct
		{
			float _m0;
			float _m1;
			float _m2;
			float _m3;
		};
	};
};

