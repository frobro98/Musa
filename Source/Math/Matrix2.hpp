#pragma once

#include "Math/Vector2.hpp"
#include "Math/MathDefinitions.h"

class Matrix2
{
public:
	Matrix2();
	explicit Matrix2(const Vector2& row0, const Vector2& row1);
	explicit Matrix2(MatrixSpecialType specialEnum);
	explicit Matrix2(MatrixTransType transEnum, const Vector4& transVec);
	explicit Matrix2(MatrixTransType transEnum, float x, float y, float z);
	explicit Matrix2(RotType rotationEnum, float angle);
	explicit Matrix2(Rot3AxisType multiAxisEnum, float xAngleRad, float yAngleRad, float zAngleRad);
	explicit Matrix2(RotAxisAngleType axisEnum, const Vector4& vect, float angleRads);
	explicit Matrix2(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	explicit Matrix2(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	explicit Matrix2(MatrixScaleType scaleEnum, float sx, float sy, float sz);
	Matrix2(const struct Quat& q);

	Matrix2(const Matrix2& other);
	Matrix2(Matrix2&& other) noexcept;
	~Matrix2();

	Matrix2& operator=(const Matrix2& m);
	Matrix2& operator=(Matrix2&& m) noexcept;

	// Setting specific matrix types
	void Set(MatrixTransType transEnum, const Vector4& transVec);
	void Set(MatrixTransType transEnum, float x, float y, float z);
	void Set(RotType rotationEnum, float angle);
	void Set(Rot3AxisType axisEnum, float xRad, float yRad, float zRad);
	void Set(RotAxisAngleType axisEnum, const Vector4& vect, float angleRads);
	void Set(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	void Set(const struct Quat& q);
	void Set(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	void Set(MatrixScaleType scaleEnum, float sx, float sy, float sz);
	void Set(MatrixSpecialType specialEnum);
	void Set(const Vector4 &axis, float angle);

	// Setting matrix
	void Set(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
	void Set(MatrixRowType rowEnum, const Vector4& rowVec);
	// Get row of matrix
	Vector4 Get(MatrixRowType rowEnum) const;

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

