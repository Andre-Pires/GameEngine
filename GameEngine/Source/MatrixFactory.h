#pragma once
#include "Matrix4f.h"
#include "Matrix3f.h"
#include "Matrix2f.h"

#define  PI 3.14159265

class MatrixFactory
{
public:
	MatrixFactory();
	~MatrixFactory();
	static Matrix2f Zero2();
	static Matrix3f Zero3();
	static Matrix4f Zero4();
	static Matrix2f Identity2();
	static Matrix3f Identity3();
	static Matrix4f Identity4();
	static Matrix4f Scale4(Vector3f scale);
	static Matrix4f Translation4(Vector3f translate);
	static Matrix2f Rotation2(float angleDeg);
	static Matrix4f Rotation4(float angle, Vector3f axis);
	static Matrix4f Shear4(float shearX, float shearY);
	static Matrix4f Mat3to4(Matrix3f matrix);
	static Matrix3f Mat4to3(Matrix4f matrix);
};
