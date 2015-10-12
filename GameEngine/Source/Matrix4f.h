#pragma once
#include <iostream>
#include "../Dependencies/glew/glew.h"
#include "Vector4f.h"

class Vector3f;
using namespace std;
class Matrix4f
{
private:
	static const int MATRIX_LENGTH = 16;
	static const int MATRIX_COLUMN_LENGTH = 4;
	static const int MATRIX_ROW_LENGTH = 4;
	GLfloat matrix[MATRIX_LENGTH];
public:

	Matrix4f(float * matrix);
	Matrix4f();
	~Matrix4f();
	float* getMatrix();
	void setValue(int col, int row, float value);
	float operator()(int col, int row);
	void operator+=(Matrix4f& matrix2);
	Matrix4f operator+ (Matrix4f& matrix2);
	void operator=(float* newMatrix);
	bool operator==(Matrix4f& matrix2);
	bool operator!=(Matrix4f& matrix2);
	void operator-=(Matrix4f& matrix2);
	Matrix4f operator-(Matrix4f& matrix2);
	void operator*=(const float& scalar);
	Matrix4f operator* (const float& scalar);
	friend Matrix4f operator* (const float& scalar, Matrix4f& matrix);
	void operator/=(const float& scalar);
	Matrix4f operator/ (const float& scalar);
	Matrix4f operator* (Matrix4f& matrix2);
	Vector4f operator*(Vector4f& vector);
	void operator*=(Matrix4f& matrix2);
	Matrix4f Transpose();
	ostream& operator<<(ostream& stream);
	friend ostream& operator<<(ostream& stream, Matrix4f matrix2);
};
