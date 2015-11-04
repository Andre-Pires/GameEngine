#include "Vector3f.h"
#include <iostream>

Vector3f::Vector3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f::Vector3f()
{
}

Vector3f::~Vector3f()
{
}

//Indexes start at 0, like a regular array
float Vector3f::operator[] (const int& index) {
	Vector3f vector = *this;
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

	return value;
}

Vector3f Vector3f::operator+ (const Vector3f& vector2) {
	Vector3f vector1 = *this;
	float x;
	float y;
	float z;

	x = vector1.x + vector2.x;
	y = vector1.y + vector2.y;
	z = vector1.z + vector2.z;

	return Vector3f(x, y, z);
}

Vector3f Vector3f::operator- (const Vector3f& vector2) {
	Vector3f vector1 = *this;
	float x;
	float y;
	float z;

	x = vector1.x - vector2.x;
	y = vector1.y - vector2.y;
	z = vector1.z - vector2.z;

	return Vector3f(x, y, z);
}

Vector3f Vector3f::operator* (const float& scalar) {
	Vector3f vector = *this;

	return Vector3f(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector3f operator* (float scalar, const Vector3f vector) {
	return Vector3f(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vector3f Vector3f::operator/ (const float& scalar) {
	Vector3f vector = *this;

	return Vector3f(vector.x / scalar, vector.y / scalar, vector.z / scalar);
}

Vector3f Vector3f::operator-()
{
	Vector3f vector = *this;

	return Vector3f(-vector.x, -vector.y, -vector.z);
}

bool Vector3f::operator== (const Vector3f& vector2) {
	Vector3f vector1 = *this;

	return vector1.x == vector2.x && vector1.y == vector2.y && vector1.z == vector2.z;
}

bool Vector3f::operator!= (const Vector3f& vector2) {
	Vector3f vector1 = *this;

	return vector1.x != vector2.x || vector1.y != vector2.y || vector1.z != vector2.z;
}

void Vector3f::operator+=(const Vector3f & vector2)
{
	Vector3f *vector = this;

	vector->x += vector2.x;
	vector->y += vector2.y;
	vector->z += vector2.z;
}

void Vector3f::operator-=(const Vector3f & vector2)
{
	Vector3f *vector = this;

	vector->x -= vector2.x;
	vector->y -= vector2.y;
	vector->z -= vector2.z;
}

void Vector3f::operator*=(const float & scalar)
{
	Vector3f *vector = this;

	vector->x *= scalar;
	vector->y *= scalar;
	vector->z *= scalar;
}

void Vector3f::operator/=(const float & scalar)
{
	Vector3f *vector = this;

	vector->x /= scalar;
	vector->y /= scalar;
	vector->z /= scalar;
}

void Vector3f::operator=(const Vector3f & vector2)
{
	Vector3f *vector = this;

	vector->x = vector2.x;
	vector->y = vector2.y;
	vector->z = vector2.z;
}

Vector3f Vector3f::Cross(const Vector3f vector2) {
	Vector3f vector1 = *this;
	float x;
	float y;
	float z;

	x = vector1.y * vector2.z - vector1.z * vector2.y;
	y = vector1.z * vector2.x - vector1.x * vector2.z;
	z = vector1.x * vector2.y - vector1.y * vector2.x;

	return Vector3f(x, y, z);
}

float Vector3f::Dot(const Vector3f vector2) {
	Vector3f vector1 = *this;

	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

float Vector3f::Norm() {
	Vector3f vector = *this;

	return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

Vector3f Vector3f::Normalize() {
	Vector3f vector = *this;
	float length = vector.Norm();

	return Vector3f(vector.x / length, vector.y / length, vector.z / length);
}

float Vector3f::Quadrance() {
	Vector3f vector = *this;

	return (pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

Vector3f Vector3f::lerp(Vector3f& A, Vector3f& B, float t) {
	return A*t + B*(1.0f - t);
}

std::ostream& Vector3f::operator<<(std::ostream& stream)
{
	Vector3f vector = *this;

	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n"
		<< "[ " << vector.z << " ] \n" << std::endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vector3f vector)
{
	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n"
		<< "[ " << vector.z << " ] \n" << std::endl;

	return stream;
}