#pragma once
#include <iostream>
#include "Vector3f.h"

using namespace std;
class Matrix3f
{
private:
	static const int MATRIX_LENGTH = 9;
	static const int MATRIX_COLUMN_LENGTH = 3;
	static const int MATRIX_ROW_LENGTH = 3;
	float matrix[MATRIX_LENGTH];
public:

	Matrix3f(float * matrix);
	Matrix3f();
	~Matrix3f();
	float* getMatrix();
	void setValue(int col, int row, float value);
	float operator()(int col, int row);
	void operator+=(Matrix3f& matrix2);
	Matrix3f operator+ (Matrix3f& matrix2);
	Matrix3f operator+(float& scalar);
	friend Matrix3f operator+(const float& scalar, Matrix3f& matrix);
	void operator=(float* newMatrix);
	bool operator==(Matrix3f& matrix2);
	bool operator!=(Matrix3f& matrix2);
	void operator-=(Matrix3f& matrix2);
	Matrix3f operator-();
	Matrix3f operator-(Matrix3f& matrix2);
	void operator*=(const float& scalar);
	Matrix3f operator* (const float& scalar);
	friend Matrix3f operator* (const float& scalar, Matrix3f& matrix);
	void operator/=(const float& scalar);
	Matrix3f operator/ (const float& scalar);
	Matrix3f operator* (Matrix3f& matrix2);
	Vector3f operator*(Vector3f& vector);
	void operator*=(Matrix3f& matrix2);
	Matrix3f Transpose();
	Matrix3f Adjoint();
	Matrix3f Inverse();
	float Determinant();
	ostream& operator<<(ostream& stream);
	friend ostream& operator<<(ostream& stream, Matrix3f matrix2);
};
