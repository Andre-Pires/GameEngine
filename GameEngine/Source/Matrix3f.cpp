#include "Matrix3f.h"
#include <corecrt_memcpy_s.h>

Matrix3f::Matrix3f(float * SourceMatrix)
{
	memcpy(matrix, SourceMatrix, sizeof(float) * MATRIX_LENGTH);
}

Matrix3f::Matrix3f()
{
}

Matrix3f::~Matrix3f()
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
float * Matrix3f::getMatrix()
{
	return matrix;
}

//Indexes used start at 1
void Matrix3f::setValue(int col, int row, float value) {
	//indexes are corrected in function to allow simpler use

	this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)] = value;
}

//Indexes used start at 1
float Matrix3f::operator()(int col, int row)
{
	//indexes are corrected in function to allow simpler use

	return this->matrix[(col - 1)  * MATRIX_COLUMN_LENGTH + (row - 1)];
}

void Matrix3f::operator+=(Matrix3f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] += mat2[i];
	}
}

Matrix3f Matrix3f::operator+(Matrix3f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] + mat2[i];
	}

	return Matrix3f(returnMat);
}

Matrix3f Matrix3f::operator+(float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] + scalar;
	}

	return Matrix3f(returnMat);
}

Matrix3f operator+(const float& scalar, Matrix3f& matrix)
{
	float * mat = matrix.getMatrix();
	float returnMat[9];

	for (int i = 0; i < 9; i++)
	{
		returnMat[i] = mat[i] + scalar;
	}

	return Matrix3f(returnMat);
}

void Matrix3f::operator=(float * newMatrix)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		matrix[i] = newMatrix[i];
	}
}

bool Matrix3f::operator==(Matrix3f& matrix2)
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

bool Matrix3f::operator!=(Matrix3f& matrix2)
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

void Matrix3f::operator-=(Matrix3f& matrix2)
{
	float * mat2 = matrix2.getMatrix();

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] -= mat2[i];
	}
}

Matrix3f Matrix3f::operator-()
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = -matrix[i];
	}

	return returnMat;
}

Matrix3f Matrix3f::operator-(Matrix3f& matrix2)
{
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] - mat2[i];
	}

	return Matrix3f(returnMat);
}

void Matrix3f::operator*=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] *= scalar;
	}
}

Matrix3f Matrix3f::operator*(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] * scalar;
	}

	return Matrix3f(returnMat);
}

Matrix3f operator*(const float& scalar, Matrix3f& matrix)
{
	float * mat = matrix.getMatrix();
	float returnMat[Matrix3f::MATRIX_LENGTH];

	for (int i = 0; i < Matrix3f::MATRIX_LENGTH; i++)
	{
		returnMat[i] = mat[i] * scalar;
	}

	return Matrix3f(returnMat);
}

void Matrix3f::operator/=(const float& scalar)
{
	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		this->matrix[i] /= scalar;
	}
}

Matrix3f Matrix3f::operator/(const float& scalar)
{
	float returnMat[MATRIX_LENGTH];

	for (int i = 0; i < MATRIX_LENGTH; i++)
	{
		returnMat[i] = matrix[i] / scalar;
	}

	return Matrix3f(returnMat);
}

Matrix3f Matrix3f::operator*(Matrix3f& matrix2)
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

	/*Matrix3f returnMat = new float[9]{ 0,0,0,0,0,0,0,0,0 };

	returnMat.setValue(1, 1, (*this)(1, 1) * matrix2(1, 1) + (*this)(2, 1) * matrix2(1, 2) + (*this)(3, 1) * matrix2(1, 3));
	returnMat.setValue(2, 1, (*this)(1, 2) * matrix2(1, 1) + (*this)(2, 2) * matrix2(1, 2) + (*this)(3, 2) * matrix2(1, 3));
	returnMat.setValue(3, 1, (*this)(1, 3) * matrix2(1, 1) + (*this)(2, 3) * matrix2(1, 2) + (*this)(3, 3) * matrix2(1, 3));

	returnMat.setValue(1, 2, (*this)(1, 1) * matrix2(2, 1) + (*this)(2, 1) * matrix2(2, 2) + (*this)(3, 1) * matrix2(2, 3));
	returnMat.setValue(2, 2, (*this)(1, 2) * matrix2(2, 1) + (*this)(2, 2) * matrix2(2, 2) + (*this)(3, 2) * matrix2(2, 3));
	returnMat.setValue(3, 2, (*this)(1, 3) * matrix2(2, 1) + (*this)(2, 3) * matrix2(2, 2) + (*this)(3, 3) * matrix2(2, 3));

	returnMat.setValue(1, 3, (*this)(1, 1) * matrix2(3, 1) + (*this)(2, 1) * matrix2(3, 2) + (*this)(3, 1) * matrix2(3, 3));
	returnMat.setValue(2, 3, (*this)(1, 2) * matrix2(3, 1) + (*this)(2, 2) * matrix2(3, 2) + (*this)(3, 2) * matrix2(3, 3));
	returnMat.setValue(3, 3, (*this)(1, 3) * matrix2(3, 1) + (*this)(2, 3) * matrix2(3, 2) + (*this)(3, 3) * matrix2(3, 3));*/

	return returnMat;
}

Vector3f Matrix3f::operator*(Vector3f& vector)
{
	Matrix3f matrix = *this;
	Vector3f returnVec = Vector3f(0.0f, 0.0f, 0.0f);

	returnVec.x = matrix(1, 1) * vector.x + matrix(2, 1) * vector.y + matrix(3, 1) * vector.z;
	returnVec.y = matrix(1, 2) * vector.x + matrix(2, 2) * vector.y + matrix(3, 2) * vector.z;
	returnVec.z = matrix(1, 3) * vector.x + matrix(2, 3) * vector.y + matrix(3, 3) * vector.z;

	return returnVec;
}

void Matrix3f::operator*=(Matrix3f& matrix2)
{
	int row, col, k;
	float * mat1 = this->getMatrix();
	float * mat2 = matrix2.getMatrix();
	float returnMat[MATRIX_LENGTH];

	for (col = 0; col < MATRIX_COLUMN_LENGTH; col++) {
		for (row = 0; row < MATRIX_ROW_LENGTH; row++) {
			returnMat[col  * MATRIX_COLUMN_LENGTH + row] = 0;

			for (k = 0; k < MATRIX_COLUMN_LENGTH; k++) {
				returnMat[col  * MATRIX_COLUMN_LENGTH + row] += mat1[col  * MATRIX_COLUMN_LENGTH + k] * mat2[k  * MATRIX_COLUMN_LENGTH + row];
			}
		}
	}

	memcpy(matrix, returnMat, sizeof(float) *MATRIX_LENGTH);
}

Matrix3f Matrix3f::Transpose()
{
	int row, col;
	Matrix3f returnMat = new float[9]{ 0,0,0,0,0,0,0,0,0 };

	for (col = 1; col <= MATRIX_COLUMN_LENGTH; col++) {
		for (row = 1; row <= MATRIX_ROW_LENGTH; row++) {
			returnMat.setValue(col, row, (*this)(row, col));
		}
	}

	return returnMat;
}

Matrix3f Matrix3f::Adjoint()
{
	Matrix3f mat = (*this).Transpose();
	Matrix3f adjoint;

	adjoint.setValue(1, 1, (mat(2, 2) * mat(3, 3)) - (mat(3, 2) * mat(2, 3)));
	adjoint.setValue(1, 2, -((mat(2, 1) * mat(3, 3)) - (mat(3, 2) * mat(1, 3))));
	adjoint.setValue(1, 3, (mat(1, 2) * mat(2, 3)) - (mat(2, 2) * mat(1, 3)));

	adjoint.setValue(2, 1, -((mat(1, 2) * mat(3, 3)) - (mat(1, 3) * mat(3, 2))));
	adjoint.setValue(2, 2, (mat(1, 1) * mat(3, 3)) - (mat(3, 1) * mat(1, 3)));
	adjoint.setValue(2, 3, -((mat(1, 1) * mat(2, 3)) - (mat(2, 1) * mat(1, 3))));

	adjoint.setValue(3, 1, ((mat(2, 1) * mat(3, 2)) - mat(3, 1) * mat(2, 2)));
	adjoint.setValue(3, 2, -((mat(1, 1) * mat(3, 2)) - (mat(3, 1) * mat(1, 2))));
	adjoint.setValue(3, 3, (mat(1, 1) * mat(2, 2)) - (mat(2, 1) * mat(1, 2)));

	return adjoint;
}

Matrix3f Matrix3f::Inverse()
{
	Matrix3f mat = *this;
	float determinant = mat.Determinant();
	Matrix3f adjoint = mat.Adjoint();

	return (1 / determinant)*adjoint;
}

float Matrix3f::Determinant()
{
	Matrix3f mat = *this;
	float determinant =
		mat(1, 1) * (mat(2, 2) * mat(3, 3) - mat(2, 3) * mat(3, 2)) -
		mat(2, 1) * (mat(1, 2) * mat(3, 3) - mat(3, 2) * mat(1, 3)) +
		mat(3, 1) * (mat(1, 2) * mat(2, 3) - mat(2, 2) * mat(1, 3));

	return determinant;
}

std::ostream& Matrix3f::operator<<(std::ostream& stream)
{
	stream << "{ ";

	for (int row = 1; row <= Matrix3f::MATRIX_ROW_LENGTH; row++) {
		stream << endl << "[ ";
		for (int col = 1; col <= Matrix3f::MATRIX_COLUMN_LENGTH; col++) {
			stream << (*this)(col, row) << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Matrix3f matrix)
{
	for (int row = 1; row <= Matrix3f::MATRIX_ROW_LENGTH; row++) {
		stream << endl << "[ ";
		for (int col = 1; col <= Matrix3f::MATRIX_COLUMN_LENGTH; col++) {
			stream << matrix(col, row) << " ";
		}
		stream << "]";
	}

	stream << "\n" << endl;

	return stream;
}