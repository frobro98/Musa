#pragma once

#include "Vector.h"

class Matrix
{
public:
	Matrix();
	explicit Matrix(const Vector& row0, const Vector& row1, const Vector& row2, const Vector& row3);
	explicit Matrix(MatrixSpecialType specialEnum);
	explicit Matrix(MatrixTransType transEnum, const Vector& transVec);
	explicit Matrix(MatrixTransType transEnum, float x, float y, float z);
	explicit Matrix(RotType rotationEnum, float angle);
	explicit Matrix(Rot3AxisType multiAxisEnum, float xAngleRad, float yAngleRad, float zAngleRad);
	explicit Matrix(RotAxisAngleType axisEnum, const Vector& vect, float angleRads);
	explicit Matrix(RotOrientType orientEnum, const Vector& dof, const Vector& up);
	explicit Matrix(MatrixScaleType scaleEnum, const Vector& scaleVec);
	explicit Matrix(MatrixScaleType scaleEnum, float sx, float sy, float sz);
	Matrix(const struct Quat& q);

	Matrix(const Matrix& other);
	Matrix(Matrix&& other) noexcept;
	~Matrix();

	Matrix& operator=(const Matrix& m);
	Matrix& operator=(Matrix&& m) noexcept;

	// Setting specific matrix types
	void Set(MatrixTransType transEnum, const Vector& transVec);
	void Set(MatrixTransType transEnum, float x, float y, float z);
	void Set(RotType rotationEnum, float angle);
	void Set(Rot3AxisType axisEnum, float xRad, float yRad, float zRad);
	void Set(RotAxisAngleType axisEnum, const Vector& vect, float angleRads);
	void Set(RotOrientType orientEnum, const Vector& dof, const Vector& up);
	void Set(const struct Quat& q);
	void Set(MatrixScaleType scaleEnum, const Vector& scaleVec);
	void Set(MatrixScaleType scaleEnum, float sx, float sy, float sz);
	void Set(MatrixSpecialType specialEnum);
	void Set(const Vector &axis, float angle);

	// Setting matrix
	void Set(const Vector& row0, const Vector& row1, const Vector& row2, const Vector& row3);
	void Set(MatrixRowType rowEnum, const Vector& rowVec);
	// Get row of matrix
	Vector Get(MatrixRowType rowEnum) const;
	
	// Matrix operations

	// Determinant
	float Determinant() const;

	// Inverse
	void Inverse();
	Matrix GetInverse() const;

	// Transpose
	void Transpose();
	Matrix GetTranspose() const;

	bool IsIdentity(float epsilon = Math::InternalTolerence) const;
	bool IsEqual(const Matrix& m) const;

	// Math operators
	Matrix operator+(const Matrix& m) const;
	Matrix operator-(const Matrix& m) const;
	Matrix operator*(const Matrix& m) const;
	Matrix operator*(float s) const;

	Matrix& operator+=(const Matrix& m);
	Matrix& operator-=(const Matrix& m);
	Matrix& operator*=(const Matrix& m);
	Matrix& operator*=(float s);

	friend Vector operator*(const Vector& v, const Matrix& m);
	friend Matrix operator*(float s, const Matrix& m);
	friend Vector& operator*=(Vector& v, const Matrix& m);

	// Unary operators
	Matrix operator+() const;
	Matrix operator-() const;


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

	union
	{
		struct
		{
			Vector v0;
			Vector v1;
			Vector v2;
			Vector v3;
		};

		struct
		{
			float _m0;
			float _m1;
			float _m2;
			float _m3;
			float _m4;
			float _m5;
			float _m6;
			float _m7;
			float _m8;
			float _m9;
			float _m10;
			float _m11;
			float _m12;
			float _m13;
			float _m14;
			float _m15;
		};
	};
};
