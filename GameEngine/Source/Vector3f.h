#pragma once
#include <ctgmath>

class Vector3f
{
public:
	float x, y, z;
	Vector3f(float x, float y, float z);
	Vector3f();
	~Vector3f();
	float operator[](const int& index);
	Vector3f operator+ (const Vector3f& vector2);
	Vector3f operator- (const Vector3f& vector2);
	Vector3f operator* (const float& scalar);
	Vector3f operator- ();
	void operator+= (const Vector3f& vector2);
	void operator-= (const Vector3f& vector2);
	void operator*= (const float& scalar);
	void operator/= (const float& scalar);
	void operator= (const Vector3f& vector2);
	friend Vector3f operator* (const float scalar, const Vector3f vector);
	Vector3f operator/ (const float& scalar);
	bool operator==(const Vector3f & vector2);
	bool operator!=(const Vector3f & vector2);
	Vector3f Cross(const Vector3f vector2);
	float Dot(const Vector3f vector2);
	float Norm();
	Vector3f Normalize();
	float Quadrance();
	static Vector3f lerp(Vector3f& A, Vector3f& B, float t);
	std::ostream& operator<<(std::ostream& stream);
	friend std::ostream& operator<<(std::ostream& stream, Vector3f vector);
};
