#include "Vector4f.h"
#include <iostream>

Vector4f::Vector4f(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4f::Vector4f(float * vector)
{
	this->x = vector[0];
	this->y = vector[1];
	this->z = vector[2];
	this->w = vector[3];
}

Vector4f::Vector4f()
{
}

Vector4f::~Vector4f()
{
}

//Indexes start at 0, like a regular array
float Vector4f::operator[] (const int& index) {
	Vector4f vector = *this;
	float value;

	if (index == 0)
	{
		value = vector.x;
	}
	else if (index == 1)
	{
		value = vector.y;
	}
	else if (index == 2)
	{
		value = vector.z;
	}
	else if (index == 3)
	{
		value = vector.w;
	}

	return value;
}

float* Vector4f::getVector()
{
	return new float[4]{ x,y,z,w };
}

Vector4f Vector4f::operator+ (const Vector4f& vector2) {
	Vector4f vector1 = *this;
	float x;
	float y;
	float z;
	float w;

	x = vector1.x + vector2.x;
	y = vector1.y + vector2.y;
	z = vector1.z + vector2.z;
	w = vector1.w + vector2.w;

	return Vector4f(x, y, z, w);
}

Vector4f Vector4f::operator- (const Vector4f& vector2) {
	Vector4f vector1 = *this;
	float x;
	float y;
	float z;
	float w;

	x = vector1.x - vector2.x;
	y = vector1.y - vector2.y;
	z = vector1.z - vector2.z;
	w = vector1.w - vector2.w;

	return Vector4f(x, y, z, w);
}

Vector4f Vector4f::operator* (const float& scalar) {
	Vector4f vector = *this;

	return Vector4f(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
}

Vector4f operator* (float scalar, const Vector4f vector) {
	return Vector4f(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
}

Vector4f Vector4f::operator/ (const float& scalar) {
	Vector4f vector = *this;

	return Vector4f(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar);
}

Vector4f Vector4f::operator-()
{
	Vector4f vector = *this;

	return Vector4f(-vector.x, -vector.y, -vector.z, -vector.w);
}

bool Vector4f::operator== (const Vector4f& vector2) {
	Vector4f vector1 = *this;

	return vector1.x == vector2.x && vector1.y == vector2.y && vector1.z == vector2.z && vector1.w == vector2.w;
}

bool Vector4f::operator!= (const Vector4f& vector2) {
	Vector4f vector1 = *this;

	return vector1.x != vector2.x || vector1.y != vector2.y || vector1.z != vector2.z || vector1.w != vector2.w;
}

void Vector4f::operator+=(const Vector4f & vector2)
{
	Vector4f *vector = this;

	vector->x += vector2.x;
	vector->y += vector2.y;
	vector->z += vector2.z;
	vector->w += vector2.w;
}

void Vector4f::operator-=(const Vector4f & vector2)
{
	Vector4f *vector = this;

	vector->x -= vector2.x;
	vector->y -= vector2.y;
	vector->z -= vector2.z;
	vector->w -= vector2.w;
}

void Vector4f::operator*=(const float & scalar)
{
	Vector4f *vector = this;

	vector->x *= scalar;
	vector->y *= scalar;
	vector->z *= scalar;
	vector->w *= scalar;
}

void Vector4f::operator/=(const float & scalar)
{
	Vector4f *vector = this;

	vector->x /= scalar;
	vector->y /= scalar;
	vector->z /= scalar;
	vector->w /= scalar;
}

void Vector4f::operator=(const Vector4f & vector2)
{
	Vector4f *vector = this;

	vector->x = vector2.x;
	vector->y = vector2.y;
	vector->z = vector2.z;
	vector->w = vector2.w;
}

float Vector4f::Dot(const Vector4f vector2) {
	Vector4f vector1 = *this;

	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z) + (vector1.w * vector2.w);
}

float Vector4f::Norm() {
	Vector4f vector = *this;

	return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2) + pow(vector.w, 2));
}

Vector4f Vector4f::Normalize() {
	Vector4f vector = *this;
	float length = vector.Norm();

	return Vector4f(vector.x / length, vector.y / length, vector.z / length, 1);
}

Vector4f Vector4f::lerp(Vector4f& A, Vector4f& B, float t) {
	return A*t + B*(1.0f - t);
}

std::ostream& Vector4f::operator<<(std::ostream& stream)
{
	Vector4f vector = *this;

	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n"
		<< "[ " << vector.z << " ] \n"
		<< "[ " << vector.w << " ] \n" << std::endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vector4f vector)
{
	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n"
		<< "[ " << vector.z << " ] \n"
		<< "[ " << vector.w << " ] \n" << std::endl;

	return stream;
}