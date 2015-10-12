#pragma once
#include <iostream>
#include "Vector2f.h"

using namespace std;
class Matrix2f
{
private:
	static const int MATRIX_LENGTH = 4;
	static const int MATRIX_COLUMN_LENGTH = 2;
	static const int MATRIX_ROW_LENGTH = 2;
	float matrix[MATRIX_LENGTH];
public:

	Matrix2f(float * matrix);
	Matrix2f(float val1, float val2, float val3, float val4);
	Matrix2f();
	~Matrix2f();
	float* getMatrix();
	void setValue(int col, int row, float value);
	float operator()(int col, int row);
	void operator+=(Matrix2f& matrix2);
	Matrix2f operator+ (Matrix2f& matrix2);
	void operator=(float* newMatrix);
	bool operator==(Matrix2f& matrix2);
	bool operator!=(Matrix2f& matrix2);
	void operator-=(Matrix2f& matrix2);
	Matrix2f operator-(Matrix2f& matrix2);
	void operator*=(const float& scalar);
	Matrix2f operator* (const float& scalar);
	friend Matrix2f operator* (const float& scalar, Matrix2f& matrix);
	void operator/=(const float& scalar);
	Matrix2f operator/ (const float& scalar);
	Vector2f operator*(Vector2f & vector);
	Matrix2f operator* (Matrix2f& matrix2);
	void operator*=(Matrix2f& matrix2);
	Matrix2f Transpose();
	Matrix2f Adjoint();
	Matrix2f Inverse();
	float Determinant();
	ostream& operator<<(ostream& stream);
	friend ostream& operator<<(ostream& stream, Matrix2f matrix2);
};
