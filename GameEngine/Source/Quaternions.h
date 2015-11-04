#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4f.h"
#include "Utilities.h"
#include <iostream>

class Quaternion
{
public:
	float t, x, y, z;

	Quaternion(Vector4f qtrn);
	Quaternion(float t, float x, float y, float z);
	Quaternion();
	static Quaternion FromAngleAxis(float theta, Vector3f axis);
	void ToAngleAxis(float& theta, Vector4f& axis);
	void Clean();
	float Quadrance();
	float Norm();
	void operator*=(const float& scalar);
	Quaternion operator* (const float& scalar);
	friend Quaternion operator* (const Quaternion& qtrn, const float& scalar);
	Quaternion operator+(const Quaternion& qtrn1);
	Quaternion operator* (const Quaternion& qtrn1);
	Quaternion Normalize();
	Quaternion Conjugate();
	Quaternion Inverse();
	Matrix4f ToMatrix4();
	Quaternion Lerp(const Quaternion& qtrn1, float k);
	Quaternion Slerp(const Quaternion& qtrn1, float k);
	bool operator==(const Quaternion& qtrn1);
	ostream& operator<<(ostream& stream);
	friend ostream& operator<<(ostream& stream, const Quaternion& qtrn);
};
