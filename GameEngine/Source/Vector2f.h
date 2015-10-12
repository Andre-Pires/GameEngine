#pragma once
#include <ctgmath>

class Vector2f
{
public:
	float x, y;
	Vector2f(float x, float y);
	~Vector2f();
	Vector2f operator+(const Vector2f & vector2);
	Vector2f operator-(const Vector2f & vector2);
	Vector2f operator*(const float & scalar);
	Vector2f operator- ();
	void operator+=(const Vector2f & vector2);
	void operator-=(const Vector2f & vector2);
	void operator*=(const float & scalar);
	void operator/=(const float & scalar);
	void operator=(const Vector2f & vector2);
	friend Vector2f operator*(const float & scalar, const Vector2f& vector);
	Vector2f operator/(const float & scalar);
	bool operator==(const Vector2f & vector2);
	bool operator!= (const Vector2f& vector2);
	float Dot(const Vector2f vector2);
	float Norm();
	Vector2f Normalize();
	float Quadrance();
	std::ostream& operator<<(std::ostream& stream);
	friend std::ostream& operator<<(std::ostream& stream, Vector2f vector);
};
