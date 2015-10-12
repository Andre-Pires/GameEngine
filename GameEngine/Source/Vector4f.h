#pragma once
#include <ctgmath>

class Vector4f
{
public:
	float x, y, z, w;
	Vector4f(float x, float y, float z, float w);
	Vector4f(float* vector);
	Vector4f();
	~Vector4f();
	float operator[](const int& index);
	float* getVector();
	Vector4f operator+ (const Vector4f& vector2);
	Vector4f operator- (const Vector4f& vector2);
	Vector4f operator* (const float& scalar);
	Vector4f operator- ();
	void operator+= (const Vector4f& vector2);
	void operator-= (const Vector4f& vector2);
	void operator*= (const float& scalar);
	void operator/= (const float& scalar);
	void operator= (const Vector4f& vector2);
	friend Vector4f operator* (const float scalar, const Vector4f vector);
	Vector4f operator/ (const float& scalar);
	bool operator==(const Vector4f & vector2);
	bool operator!=(const Vector4f & vector2);
	float Dot(const Vector4f vector2);
	float Norm();
	Vector4f Normalize();
	std::ostream& operator<<(std::ostream& stream);
	friend std::ostream& operator<<(std::ostream& stream, Vector4f vector);
};
