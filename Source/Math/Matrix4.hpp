// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Vector4.hpp"
#include "Math/MathDefinitions.hpp"

class Matrix4
{
public:
	Matrix4();
	explicit Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
	explicit Matrix4(MatrixSpecialType specialEnum);
	explicit Matrix4(MatrixTransType transEnum, const Vector4& transVec);
	explicit Matrix4(MatrixTransType transEnum, float x, float y, float z);
	explicit Matrix4(RotType rotationEnum, float angle);
	explicit Matrix4(Rot3AxisType multiAxisEnum, float xAngleRad, float yAngleRad, float zAngleRad);
	explicit Matrix4(RotAxisAngleType axisEnum, const Vector4& vect, float angleRads);
	explicit Matrix4(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	explicit Matrix4(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	explicit Matrix4(MatrixScaleType scaleEnum, float sx, float sy, float sz);
	Matrix4(const struct Quat& q);

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

	// TODO - Need to figure out why my inverse is incorrect in some specific cases...
	// Inverse
	void Inverse();
	Matrix4 GetInverse() const;

	// Transpose
	void Transpose();
	Matrix4 GetTranspose() const;

	bool IsIdentity(float epsilon = Math::InternalTolerence) const;
	bool IsEqual(const Matrix4& m) const;

	// Math operators
	Matrix4 operator+(const Matrix4& m) const;
	Matrix4 operator-(const Matrix4& m) const;
	Matrix4 operator*(const Matrix4& m) const;
	Matrix4 operator*(float s) const;

	Matrix4& operator+=(const Matrix4& m);
	Matrix4& operator-=(const Matrix4& m);
	Matrix4& operator*=(const Matrix4& m);
	Matrix4& operator*=(float s);

	friend Vector4 operator*(const Vector4& v, const Matrix4& m);
	friend Matrix4 operator*(float s, const Matrix4& m);
	friend Vector4& operator*=(Vector4& v, const Matrix4& m);

	// Unary operators
	Matrix4 operator+() const;
	Matrix4 operator-() const;


	// Mutable individual getters
	float& m0();
	float& m1();
	float& m2();
	float& m3();
	float& m4();
	float& m5();
	float& m6();
	float& m7();
	float& m8();
	float& m9();
	float& m10();
	float& m11();
	float& m12();
	float& m13();
	float& m14();
	float& m15();

	// Non-mutable individual getters
	const float& m0() const;
	const float& m1() const;
	const float& m2() const;
	const float& m3() const;
	const float& m4() const;
	const float& m5() const;
	const float& m6() const;
	const float& m7() const;
	const float& m8() const;
	const float& m9() const;
	const float& m10() const;
	const float& m11() const;
	const float& m12() const;
	const float& m13() const;
	const float& m14() const;
	const float& m15() const;


	// Mutable bracket operators
	float& operator[](m0_enum);
	float& operator[](m1_enum);
	float& operator[](m2_enum);
	float& operator[](m3_enum);
	float& operator[](m4_enum);
	float& operator[](m5_enum);
	float& operator[](m6_enum);
	float& operator[](m7_enum);
	float& operator[](m8_enum);
	float& operator[](m9_enum);
	float& operator[](m10_enum);
	float& operator[](m11_enum);
	float& operator[](m12_enum);
	float& operator[](m13_enum);
	float& operator[](m14_enum);
	float& operator[](m15_enum);

	// Non-mutable bracket operators
	const float& operator[](m0_enum) const;
	const float& operator[](m1_enum) const;
	const float& operator[](m2_enum) const;
	const float& operator[](m3_enum) const;
	const float& operator[](m4_enum) const;
	const float& operator[](m5_enum) const;
	const float& operator[](m6_enum) const;
	const float& operator[](m7_enum) const;
	const float& operator[](m8_enum) const;
	const float& operator[](m9_enum) const;
	const float& operator[](m10_enum) const;
	const float& operator[](m11_enum) const;
	const float& operator[](m12_enum) const;
	const float& operator[](m13_enum) const;
	const float& operator[](m14_enum) const;
	const float& operator[](m15_enum) const;
	

private:
	// 00, 01, 02, 03
	Vector4 v0;
	// 10, 11, 12, 13
	Vector4 v1;
	// 20, 21, 22, 23
	Vector4 v2;
	// 30, 31, 32, 33
	Vector4 v3;

};
