// Copyright 2020, Nathan Blane

#pragma once

#include "Math/Vector4.hpp"
#include "Math/MathDefinitions.hpp"
#include "Math/MathAPI.hpp"

struct Quat;

class MATH_API Matrix4
{
public:
	Matrix4();
	explicit Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
	explicit Matrix4(MatrixSpecialType specialEnum);
	explicit Matrix4(MatrixTransType transEnum, const Vector4& transVec);
	explicit Matrix4(MatrixTransType transEnum, f32 x, f32 y, f32 z);
	explicit Matrix4(RotType rotationEnum, f32 angle);
	explicit Matrix4(Rot3AxisType multiAxisEnum, f32 xAngleRad, f32 yAngleRad, f32 zAngleRad);
	explicit Matrix4(RotAxisAngleType axisEnum, const Vector4& vect, f32 angleRads);
	explicit Matrix4(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	explicit Matrix4(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	explicit Matrix4(MatrixScaleType scaleEnum, f32 sx, f32 sy, f32 sz);
	Matrix4(const Quat& q);

	// Setting specific matrix types
	void Set(MatrixTransType transEnum, const Vector4& transVec);
	void Set(MatrixTransType transEnum, f32 x, f32 y, f32 z);
	void Set(RotType rotationEnum, f32 angle);
	void Set(Rot3AxisType axisEnum, f32 xRad, f32 yRad, f32 zRad);
	void Set(RotAxisAngleType axisEnum, const Vector4& vect, float angleRads);
	void Set(RotOrientType orientEnum, const Vector4& dof, const Vector4& up);
	void Set(const struct Quat& q);
	void Set(MatrixScaleType scaleEnum, const Vector4& scaleVec);
	void Set(MatrixScaleType scaleEnum, f32 sx, f32 sy, f32 sz);
	void Set(MatrixSpecialType specialEnum);
	void Set(const Vector4 &axis, f32 angle);

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

	bool IsIdentity(f32 epsilon = Math::InternalTolerence) const;
	bool IsEqual(const Matrix4& m, f32 epsilon = Math::InternalTolerence) const;

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

	friend MATH_API Vector4 operator*(const Vector4& v, const Matrix4& m);
	friend MATH_API Matrix4 operator*(f32 s, const Matrix4& m);
	friend MATH_API Vector4& operator*=(Vector4& v, const Matrix4& m);

	friend MATH_API bool operator==(const Matrix4& lhs, const Matrix4& rhs)
	{
		return lhs.IsEqual(rhs);
	}
	
	friend MATH_API bool operator!=(const Matrix4& lhs, const Matrix4& rhs)
	{
		return !lhs.IsEqual(rhs);
	}

	f32& m0();
	f32& m1();
	f32& m2();
	f32& m3();
	f32& m4();
	f32& m5();
	f32& m6();
	f32& m7();
	f32& m8();
	f32& m9();
	f32& m10();
	f32& m11();
	f32& m12();
	f32& m13();
	f32& m14();
	f32& m15();

	const f32& m0() const;
	const f32& m1() const;
	const f32& m2() const;
	const f32& m3() const;
	const f32& m4() const;
	const f32& m5() const;
	const f32& m6() const;
	const f32& m7() const;
	const f32& m8() const;
	const f32& m9() const;
	const f32& m10() const;
	const f32& m11() const;
	const f32& m12() const;
	const f32& m13() const;
	const f32& m14() const;
	const f32& m15() const;

	f32& operator[](m0_enum);
	f32& operator[](m1_enum);
	f32& operator[](m2_enum);
	f32& operator[](m3_enum);
	f32& operator[](m4_enum);
	f32& operator[](m5_enum);
	f32& operator[](m6_enum);
	f32& operator[](m7_enum);
	f32& operator[](m8_enum);
	f32& operator[](m9_enum);
	f32& operator[](m10_enum);
	f32& operator[](m11_enum);
	f32& operator[](m12_enum);
	f32& operator[](m13_enum);
	f32& operator[](m14_enum);
	f32& operator[](m15_enum);

	const f32& operator[](m0_enum) const;
	const f32& operator[](m1_enum) const;
	const f32& operator[](m2_enum) const;
	const f32& operator[](m3_enum) const;
	const f32& operator[](m4_enum) const;
	const f32& operator[](m5_enum) const;
	const f32& operator[](m6_enum) const;
	const f32& operator[](m7_enum) const;
	const f32& operator[](m8_enum) const;
	const f32& operator[](m9_enum) const;
	const f32& operator[](m10_enum) const;
	const f32& operator[](m11_enum) const;
	const f32& operator[](m12_enum) const;
	const f32& operator[](m13_enum) const;
	const f32& operator[](m14_enum) const;
	const f32& operator[](m15_enum) const;
	

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
