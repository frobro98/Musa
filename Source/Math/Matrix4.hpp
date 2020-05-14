// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Vector4.hpp"
#include "Math/MathDefinitions.hpp"

struct Quat;

class Matrix4
{
public:
	Matrix4();
	explicit Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
	explicit Matrix4(MatrixSpecialType specialEnum);
	explicit Matrix4(MatrixTransType transEnum, const Vector4& transVec);
	explicit Matrix4(MatrixTransType transEnum, float32 x, float32 y, float32 z);
	explicit Matrix4(RotType rotationEnum, float32 angle);
	explicit Matrix4(Rot3AxisType multiAxisEnum, float32 xAngleRad, float32 yAngleRad, float32 zAngleRad);
	explicit Matrix4(RotAxisAngleType axisEnum, const Vector4& vect, float32 angleRads);
	explicit Matrix4(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	explicit Matrix4(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	explicit Matrix4(MatrixScaleType scaleEnum, float32 sx, float32 sy, float32 sz);
	Matrix4(const Quat& q);

	// Setting specific matrix types
	void Set(MatrixTransType transEnum, const Vector4& transVec);
	void Set(MatrixTransType transEnum, float32 x, float32 y, float32 z);
	void Set(RotType rotationEnum, float32 angle);
	void Set(Rot3AxisType axisEnum, float32 xRad, float32 yRad, float32 zRad);
	void Set(RotAxisAngleType axisEnum, const Vector4& vect, float angleRads);
	void Set(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	void Set(const struct Quat& q);
	void Set(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	void Set(MatrixScaleType scaleEnum, float32 sx, float32 sy, float32 sz);
	void Set(MatrixSpecialType specialEnum);
	void Set(const Vector4 &axis, float32 angle);

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
	Matrix4 GetInverse() const;

	// Transpose
	void Transpose();
	Matrix4 GetTranspose() const;

	bool IsIdentity(float32 epsilon = Math::InternalTolerence) const;
	bool IsEqual(const Matrix4& m, float32 epsilon = Math::InternalTolerence) const;

	Matrix4 operator+() const;
	Matrix4 operator-() const;

	Matrix4 operator+(const Matrix4& m) const;
	Matrix4 operator-(const Matrix4& m) const;
	Matrix4 operator*(const Matrix4& m) const;
	Matrix4 operator*(float s) const;

	Matrix4& operator+=(const Matrix4& m);
	Matrix4& operator-=(const Matrix4& m);
	Matrix4& operator*=(const Matrix4& m);
	Matrix4& operator*=(float s);

	friend Vector4 operator*(const Vector4& v, const Matrix4& m);
	friend Matrix4 operator*(float32 s, const Matrix4& m);
	friend Vector4& operator*=(Vector4& v, const Matrix4& m);

	friend bool operator==(const Matrix4& lhs, const Matrix4& rhs)
	{
		return lhs.IsEqual(rhs);
	}
	
	friend bool operator!=(const Matrix4& lhs, const Matrix4& rhs)
	{
		return !lhs.IsEqual(rhs);
	}

	float32& m0();
	float32& m1();
	float32& m2();
	float32& m3();
	float32& m4();
	float32& m5();
	float32& m6();
	float32& m7();
	float32& m8();
	float32& m9();
	float32& m10();
	float32& m11();
	float32& m12();
	float32& m13();
	float32& m14();
	float32& m15();

	const float32& m0() const;
	const float32& m1() const;
	const float32& m2() const;
	const float32& m3() const;
	const float32& m4() const;
	const float32& m5() const;
	const float32& m6() const;
	const float32& m7() const;
	const float32& m8() const;
	const float32& m9() const;
	const float32& m10() const;
	const float32& m11() const;
	const float32& m12() const;
	const float32& m13() const;
	const float32& m14() const;
	const float32& m15() const;

	float32& operator[](m0_enum);
	float32& operator[](m1_enum);
	float32& operator[](m2_enum);
	float32& operator[](m3_enum);
	float32& operator[](m4_enum);
	float32& operator[](m5_enum);
	float32& operator[](m6_enum);
	float32& operator[](m7_enum);
	float32& operator[](m8_enum);
	float32& operator[](m9_enum);
	float32& operator[](m10_enum);
	float32& operator[](m11_enum);
	float32& operator[](m12_enum);
	float32& operator[](m13_enum);
	float32& operator[](m14_enum);
	float32& operator[](m15_enum);

	const float32& operator[](m0_enum) const;
	const float32& operator[](m1_enum) const;
	const float32& operator[](m2_enum) const;
	const float32& operator[](m3_enum) const;
	const float32& operator[](m4_enum) const;
	const float32& operator[](m5_enum) const;
	const float32& operator[](m6_enum) const;
	const float32& operator[](m7_enum) const;
	const float32& operator[](m8_enum) const;
	const float32& operator[](m9_enum) const;
	const float32& operator[](m10_enum) const;
	const float32& operator[](m11_enum) const;
	const float32& operator[](m12_enum) const;
	const float32& operator[](m13_enum) const;
	const float32& operator[](m14_enum) const;
	const float32& operator[](m15_enum) const;
	

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
