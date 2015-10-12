#include "Matrix2f.h"
#include <corecrt_memcpy_s.h>

Matrix2f::Matrix2f(float * SourceMatrix)
{
	memcpy(matrix, SourceMatrix, sizeof(float) * MATRIX_LENGTH);
}

Matrix2f::Matrix2f(float val1, float val2, float val3, float val4)
{
	matrix[0] = val1;
	matrix[1] = val2;
	matrix[2] = val3;
	matrix[3] = val4;
}

Matrix2f::Matrix2f()
{
}

Matrix2f::~Matrix2f()
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
float * Matrix2f::getMatrix()
{
	return matrix;
}

//Indexes used start at 1
void Matrix2f::setValue(int col, int row, float value) {
	//indexes are corrected in function to allow simpler use
	this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)] = value;
}

//Indexes used start at 1
float Matrix2f::operator()(int col, int row)
{
	//indexes are corrected in function to allow simpler use
	return this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)];
}

void Matrix2f::operator+=(Matrix2f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] += mat2[i];
	}
}

Matrix2f Matrix2f::operator+(Matrix2f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] + mat2[i];
	}

	return Matrix2f(returnMat);
}

void Matrix2f::operator=(float * newMatrix)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		matrix[i] = newMatrix[i];
	}
}

bool Matrix2f::operator==(Matrix2f& matrix2)
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

bool Matrix2f::operator!=(Matrix2f& matrix2)
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

void Matrix2f::operator-=(Matrix2f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] -= mat2[i];
	}
}

Matrix2f Matrix2f::operator-(Matrix2f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] - mat2[i];
	}

	return Matrix2f(returnMat);
}

void Matrix2f::operator*=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] *= scalar;
	}
}

Matrix2f Matrix2f::operator*(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] * scalar;
	}

	return Matrix2f(returnMat);
}

Matrix2f operator*(const float& scalar, Matrix2f& matrix)
{
	float * mat = matrix.getMatrix();
	float returnMat[Matrix2f::MATRIX_LENGTH];

	for (int i = 0; i < Matrix2f::MATRIX_LENGTH; i++)
	{
		returnMat[i] = mat[i] * scalar;
	}

	return Matrix2f(returnMat);
}

void Matrix2f::operator/=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] /= scalar;
	}
}

Matrix2f Matrix2f::operator/(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] / scalar;
	}

	return Matrix2f(returnMat);
}

Vector2f Matrix2f::operator*(Vector2f& vector)
{
	Matrix2f matrix = *this;
	Vector2f returnVec = Vector2f(0.0f, 0.0f);

	returnVec.x = matrix(1, 1) * vector.x + matrix(2, 1) * vector.y;
	returnVec.y = matrix(1, 2) * vector.x + matrix(2, 2) * vector.y;

	return returnVec;
}

Matrix2f Matrix2f::operator*(Matrix2f& matrix2)
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

void Matrix2f::operator*=(Matrix2f& matrix2)
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

Matrix2f Matrix2f::Transpose()
{
	int col, row;
	Matrix2f returnMat = new float[4]{ 0,0,0,0 };

	for (col = 1; col <= MATRIX_COLUMN_LENGTH; col++) {
		for (row = 1; row <= MATRIX_ROW_LENGTH; row++) {
			returnMat.setValue(col, row, (*this)(row, col));
		}
	}

	return returnMat;
}

Matrix2f Matrix2f::Adjoint()
{
	Matrix2f mat = *this;
	Matrix2f adjoint;

	adjoint.setValue(1, 1, mat(2, 2));
	adjoint.setValue(2, 1, -mat(2, 1));
	adjoint.setValue(1, 2, -mat(1, 2));
	adjoint.setValue(2, 2, mat(1, 1));

	return adjoint;
}

Matrix2f Matrix2f::Inverse()
{
	Matrix2f mat = *this;
	float determinant = mat.Determinant();
	Matrix2f adjoint = mat.Adjoint();

	return (1 / determinant)*adjoint;
}

float Matrix2f::Determinant()
{
	Matrix2f mat = *this;

	return mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1);
}

std::ostream& Matrix2f::operator<<(std::ostream& stream)
{
	for (int row = 1; row <= Matrix2f::MATRIX_ROW_LENGTH; row++) {
		stream << endl << "[ ";
		for (int col = 1; col <= Matrix2f::MATRIX_COLUMN_LENGTH; col++) {
			stream << (*this)(col, row) << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Matrix2f matrix)
{
	for (int row = 1; row <= Matrix2f::MATRIX_ROW_LENGTH; row++) {
		stream << endl << "[ ";
		for (int col = 1; col <= Matrix2f::MATRIX_COLUMN_LENGTH; col++) {
			stream << matrix(col, row) << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}