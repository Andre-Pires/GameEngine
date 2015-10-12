#include "Matrix4f.h"
#include <corecrt_memcpy_s.h>
#include "Vector4f.h"

Matrix4f::Matrix4f(float * SourceMatrix)
{
	memcpy(matrix, SourceMatrix, sizeof(float) * MATRIX_LENGTH);
}

Matrix4f::Matrix4f()
{
}

Matrix4f::~Matrix4f()
{
}

//NOTE - remember column major
/*
float row_major[16] =    {1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1};
float column_major[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1};

					 =

				1 0 0 x
				0 1 0 y
				0 0 1 z
				0 0 0 1

*/
float * Matrix4f::getMatrix()
{
	return matrix;
}

//Indexes used start at 1
void Matrix4f::setValue(int col, int row, float value) {
	//indexes are corrected in function to allow simpler use
	this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)] = value;
}

//Indexes used start at 1
float Matrix4f::operator()(int col, int row)
{
	//indexes are corrected in function to allow simpler use
	return this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)];
}

void Matrix4f::operator+=(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] += mat2[i];
	}
}

Matrix4f Matrix4f::operator+(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] + mat2[i];
	}

	return Matrix4f(returnMat);
}

void Matrix4f::operator=(float * newMatrix)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		matrix[i] = newMatrix[i];
	}
}

bool Matrix4f::operator==(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	bool matricesAreEqual = true;

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		if (matrix[i] != mat2[i])
		{
			matricesAreEqual = false;
			break;
		}
	}

	return matricesAreEqual;
}

bool Matrix4f::operator!=(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	bool matricesAreNotEqual = false;

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		if (matrix[i] != mat2[i])
		{
			matricesAreNotEqual = true;
			break;
		}
	}

	return matricesAreNotEqual;
}

void Matrix4f::operator-=(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] -= mat2[i];
	}
}

Matrix4f Matrix4f::operator-(Matrix4f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] - mat2[i];
	}

	return Matrix4f(returnMat);
}

void Matrix4f::operator*=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] *= scalar;
	}
}

Matrix4f Matrix4f::operator*(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] * scalar;
	}

	return Matrix4f(returnMat);
}

Matrix4f operator*(const float& scalar, Matrix4f& matrix)
{
	float * mat = matrix.getMatrix();
	float returnMat[Matrix4f::MATRIX_LENGTH];

	for (int i = 0; i < Matrix4f::MATRIX_LENGTH; i++)
	{
		returnMat[i] = mat[i] * scalar;
	}

	return Matrix4f(returnMat);
}

void Matrix4f::operator/=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] /= scalar;
	}
}

Matrix4f Matrix4f::operator/(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] / scalar;
	}

	return Matrix4f(returnMat);
}

Matrix4f Matrix4f::operator*(Matrix4f& matrix2)
{
	int row, col, k;
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (col = 0; col < MATRIX_COLUMN_LENGTH; col++) {
		for (row = 0; row < MATRIX_ROW_LENGTH; row++) {
			returnMat[col  * MATRIX_COLUMN_LENGTH + row] = 0;

			for (k = 0; k < MATRIX_COLUMN_LENGTH; k++) {
				returnMat[col  * MATRIX_COLUMN_LENGTH + row] += matrix[k  * MATRIX_COLUMN_LENGTH + row] * mat2[col  * MATRIX_COLUMN_LENGTH + k];
			}
		}
	}

	return returnMat;
}

Vector4f Matrix4f::operator*(Vector4f& vector)
{
	Matrix4f matrix = *this;
	Vector4f returnVec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

	returnVec.x = matrix(1, 1) * vector.x + matrix(2, 1) * vector.y + matrix(3, 1) * vector.z + matrix(4, 1) * vector.w;
	returnVec.y = matrix(1, 2) * vector.x + matrix(2, 2) * vector.y + matrix(3, 2) * vector.z + matrix(4, 2) * vector.w;
	returnVec.z = matrix(1, 3) * vector.x + matrix(2, 3) * vector.y + matrix(3, 3) * vector.z + matrix(4, 3) * vector.w;
	returnVec.w = matrix(1, 4) * vector.x + matrix(2, 4) * vector.y + matrix(3, 4) * vector.z + matrix(4, 4) * vector.w;

	return returnVec;
}

void Matrix4f::operator*=(Matrix4f& matrix2)
{
	int row, col, k;
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (col = 0; col < MATRIX_COLUMN_LENGTH; col++) {
		for (row = 0; row < MATRIX_ROW_LENGTH; row++) {
			returnMat[col  * MATRIX_COLUMN_LENGTH + row] = 0;

			for (k = 0; k < MATRIX_COLUMN_LENGTH; k++) {
				returnMat[col  * MATRIX_COLUMN_LENGTH + row] += matrix[k  * MATRIX_COLUMN_LENGTH + row] * mat2[col  * MATRIX_COLUMN_LENGTH + k];
			}
		}
	}

	memcpy(matrix, returnMat, sizeof(float) *MATRIX_LENGTH);
}

Matrix4f Matrix4f::Transpose()
{
	float returnMat[MATRIX_LENGTH];
	int col, row;

	for (col = 0; col < MATRIX_COLUMN_LENGTH; col++) {
		for (row = 0; row < MATRIX_ROW_LENGTH; row++) {
			returnMat[MATRIX_COLUMN_LENGTH * col + row] = matrix[col + row * MATRIX_ROW_LENGTH];
		}
	}

	return returnMat;
}

std::ostream& Matrix4f::operator<<(std::ostream& stream)
{
	stream << "{ ";

	for (int i = 0; i < Matrix4f::MATRIX_COLUMN_LENGTH; i++) {
		stream << endl << "[ ";
		for (int j = 0; j < Matrix4f::MATRIX_ROW_LENGTH; j++) {
			stream << matrix[i  * Matrix4f::MATRIX_COLUMN_LENGTH + j] << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Matrix4f matrix)
{
	for (int i = 0; i < Matrix4f::MATRIX_COLUMN_LENGTH; i++) {
		stream << endl << "[ ";
		for (int j = 0; j < Matrix4f::MATRIX_ROW_LENGTH; j++) {
			stream << matrix.getMatrix()[i  * Matrix4f::MATRIX_COLUMN_LENGTH + j] << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}