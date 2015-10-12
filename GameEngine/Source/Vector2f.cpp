#include "Vector2f.h"

Vector2f::Vector2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2f::~Vector2f()
{
}

Vector2f Vector2f::operator+ (const Vector2f& vector2) {
	Vector2f vector1 = *this;
	float x;
	float y;

	x = vector1.x + vector2.x;
	y = vector1.y + vector2.y;

	return Vector2f(x, y);
}

Vector2f Vector2f::operator- (const Vector2f& vector2) {
	Vector2f vector1 = *this;
	float x;
	float y;

	x = vector1.x - vector2.x;
	y = vector1.y - vector2.y;

	return Vector2f(x, y);
}

Vector2f Vector2f::operator* (const float& scalar) {
	Vector2f vector = *this;

	return Vector2f(vector.x * scalar, vector.y * scalar);
}

Vector2f Vector2f::operator-()
{
	Vector2f vector = *this;

	return Vector2f(vector.x, vector.y);
}

Vector2f operator* (const float& scalar, const Vector2f& vector) {
	return Vector2f(vector.x * scalar, vector.y * scalar);
}

Vector2f Vector2f::operator/ (const float& scalar) {
	Vector2f vector = *this;

	return Vector2f(vector.x / scalar, vector.y / scalar);
}

bool Vector2f::operator== (const Vector2f& vector2) {
	Vector2f vector1 = *this;

	return vector1.x == vector2.x && vector1.y == vector2.y;
}

bool Vector2f::operator!= (const Vector2f& vector2) {
	Vector2f vector1 = *this;

	return vector1.x != vector2.x || vector1.y != vector2.y;
}

void Vector2f::operator+=(const Vector2f & vector2)
{
	Vector2f *vector = this;

	vector->x += vector2.x;
	vector->y += vector2.y;
}

void Vector2f::operator-=(const Vector2f & vector2)
{
	Vector2f *vector = this;

	vector->x -= vector2.x;
	vector->y -= vector2.y;
}

void Vector2f::operator*=(const float & scalar)
{
	Vector2f *vector = this;

	vector->x *= scalar;
	vector->y *= scalar;
}

void Vector2f::operator/=(const float & scalar)
{
	Vector2f *vector = this;

	vector->x /= scalar;
	vector->y /= scalar;
}

void Vector2f::operator=(const Vector2f & vector2)
{
	Vector2f *vector = this;

	vector->x = vector2.x;
	vector->y = vector2.y;
}

float Vector2f::Dot(const Vector2f vector2) {
	Vector2f vector1 = *this;

	return (vector1.x * vector2.x) + (vector1.y * vector2.y);
}

float Vector2f::Norm() {
	Vector2f vector = *this;

	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

Vector2f Vector2f::Normalize() {
	Vector2f vector = *this;
	float length = vector.Norm();

	return Vector2f(vector.x / length, vector.y / length);
}

float Vector2f::Quadrance() {
	Vector2f vector = *this;

	return (pow(vector.x, 2) + pow(vector.y, 2));
}

std::ostream& Vector2f::operator<<(std::ostream& stream)
{
	Vector2f vector = *this;

	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n" << std::endl;

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vector2f vector)
{
	stream << std::endl
		<< "[ " << vector.x << " ] \n"
		<< "[ " << vector.y << " ] \n" << std::endl;

	return stream;
}