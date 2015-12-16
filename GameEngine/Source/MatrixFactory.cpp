#include "MatrixFactory.h"

MatrixFactory::MatrixFactory()
{
}

MatrixFactory::~MatrixFactory()
{
}

Matrix2f MatrixFactory::Zero2()
{
	return Matrix2f(new float[4]{
		0,0,
		0,0 });
}

Matrix3f MatrixFactory::Zero3()
{
	return Matrix3f(new float[9]{
		0,0,0,
		0,0,0,
		0,0,0 });
}

Matrix4f MatrixFactory::Zero4()
{
	return Matrix4f(new float[16]{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0 });
}

Matrix2f MatrixFactory::Identity2()
{
	return Matrix2f(new float[4]{
		1,0,
		0,1 });
}

Matrix3f MatrixFactory::Identity3()
{
	return Matrix3f(new float[9]{
		1,0,0,
		0,1,0,
		0,0,1 });
}

Matrix4f MatrixFactory::Identity4()
{
	return Matrix4f(new float[16]{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 });
}

Matrix4f  MatrixFactory::Scale4(Vector3f scale)
{
	return Matrix4f(new float[16]{
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,	0,	0,	1 });
}

Matrix4f MatrixFactory::Translation4(Vector3f translate)
{
	return Matrix4f(new float[16]{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translate.x,translate.y,translate.z,1 });
}

Matrix2f MatrixFactory::Rotation2(float angleDeg)
{
	float rad = (angleDeg * PI) / 180;
	
	return new float[4]{
		cos(rad), -sin(rad),
		sin(rad), cos(rad)};
}

Matrix4f MatrixFactory::Rotation4(float angle, Vector3f axis)
{
	Matrix3f I = Identity3();

	Matrix3f A = new float[16]{
		0,axis.z,-axis.y,
		-axis.z,0,axis.x,
		axis.y,-axis.x,0 };

	Matrix3f A2 = new float[16]{
		(-pow(axis.z, 2) - pow(axis.y, 2)),axis.x * axis.y, axis.x * axis.z,
		axis.x * axis.y,(-pow(axis.z, 2) - pow(axis.x, 2)),axis.y * axis.z,
		axis.x * axis.z, axis.y * axis.z,(-pow(axis.y, 2) - pow(axis.x, 2)) };

	Matrix3f R;

	float radians = (angle * PI) / 180;

	R = I + (sin(radians) * A) + (1 - cos(radians)) * A2;

	return Mat3to4(R);
}

//if no shear is intended along an axis pass 0
Matrix4f MatrixFactory::Shear4(float shearX, float shearY)
{
	return Matrix4f(new float[16]{
		1,shearY,0,0,
		shearX,1,0,0,
		0,0,1,0,
		0,	0,	0,	1 });
}

Matrix4f MatrixFactory::Mat3to4(Matrix3f matrix)
{
	Matrix4f returnMat = Identity4();

	for (int row = 1; row <= 3; row++) {
		for (int col = 1; col <= 3; col++) {
			returnMat.setValue(col, row, matrix(col, row));
		}
	}

	return returnMat;
}

Matrix3f MatrixFactory::Mat4to3(Matrix4f matrix)
{
	Matrix3f returnMat = Identity3();

	for (int row = 1; row <= 3; row++) {
		for (int col = 1; col <= 3; col++) {
			returnMat.setValue(col, row, matrix(col, row));
		}
	}

	return returnMat;
}

GLfloat * MatrixFactory::Mat4toGLfloat(Matrix4f matrix)
{
	GLfloat * returnMat = new GLfloat[16];

	for (int i = 0; i < 16; i++)
	{
		returnMat[i] = matrix.getMatrix()[i];
	}

	return returnMat;
}