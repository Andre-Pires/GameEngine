#include "Quaternions.h"

Quaternion::Quaternion(Vector4f qtrn)
{
	t = qtrn.x;
	x = qtrn.y;
	y = qtrn.z;
	z = qtrn.w;
}

Quaternion::Quaternion(float t, float x, float y, float z)
{
	this->t = t;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::Quaternion()
{
}

Quaternion Quaternion::FromAngleAxis(float theta, Vector3f axis)
{
	Vector3f axisn = axis.Normalize();
	float a = theta * (float)DEGREES_TO_RADIANS;
	float scalar = sin(a / 2.0f);

	Quaternion qtrn = Quaternion(cos(a / 2.0f), axisn.x * scalar, axisn.y * scalar, axisn.z * scalar);

	qtrn.Clean();

	return qtrn.Normalize();
}

void Quaternion::ToAngleAxis(float& theta, Vector4f& axis)
{
	Quaternion qn = this->Normalize();
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t*qn.t);

	if (s < slackThreshold) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		axis.w = 1.0f;
	}
	else {
		axis.x = qn.x / s;
		axis.y = qn.y / s;
		axis.z = qn.z / s;
		axis.w = 1.0f;
	}
}

void Quaternion::Clean()
{
	if (fabs(this->t) < slackThreshold) this->t = 0.0f;
	if (fabs(this->x) < slackThreshold) this->x = 0.0f;
	if (fabs(this->y) < slackThreshold) this->y = 0.0f;
	if (fabs(this->z) < slackThreshold) this->z = 0.0f;
}

float Quaternion::Quadrance()
{
	return this->t*this->t + this->x*this->x + this->y*this->y + this->z*this->z;
}

float Quaternion::Norm()
{
	return sqrt(this->Quadrance());
}

Quaternion Quaternion::operator*(const float& scalar)
{
	Quaternion qtrn;

	qtrn.t = scalar * this->t;
	qtrn.x = scalar * this->x;
	qtrn.y = scalar * this->y;
	qtrn.z = scalar * this->z;

	return qtrn;
}

Quaternion operator*(const Quaternion& qtrn, const float& scalar)
{
	Quaternion qtrnReturn;

	qtrnReturn.t = scalar * qtrn.t;
	qtrnReturn.x = scalar * qtrn.x;
	qtrnReturn.y = scalar * qtrn.y;
	qtrnReturn.z = scalar * qtrn.z;

	return qtrnReturn;
}

Quaternion Quaternion::operator+(const Quaternion& qtrn1)
{
	Quaternion q;

	q.t = this->t + qtrn1.t;
	q.x = this->x + qtrn1.x;
	q.y = this->y + qtrn1.y;
	q.z = this->z + qtrn1.z;

	return q;
}

Quaternion Quaternion::operator*(const Quaternion& qtrn1)
{
	Quaternion qtrn;
	qtrn.t = this->t * qtrn1.t - this->x * qtrn1.x - this->y * qtrn1.y - this->z * qtrn1.z;
	qtrn.x = this->t * qtrn1.x + this->x * qtrn1.t + this->y * qtrn1.z - this->z * qtrn1.y;
	qtrn.y = this->t * qtrn1.y + this->y * qtrn1.t + this->z * qtrn1.x - this->x * qtrn1.z;
	qtrn.z = this->t * qtrn1.z + this->z * qtrn1.t + this->x * qtrn1.y - this->y * qtrn1.x;

	return qtrn;
}

Quaternion Quaternion::Normalize()
{
	float scalar = 1 / this->Norm();

	return (*this) * scalar;
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(this->t, -this->x, -this->y, -this->z);
}

Quaternion Quaternion::Inverse()
{
	return this->Conjugate() * (1.0f / this->Quadrance());
}

Matrix4f Quaternion::ToMatrix4()
{
	Quaternion qn = this->Normalize();

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	Matrix4f matrix {
		1.0f - 2.0f * (yy + zz), 2.0f * (xy + zt), 2.0f * (xz - yt), 0.0f,
		2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + xt), 0.0f,
		2.0f * (xz + yt), 2.0f * (yz - xt), 1.0f - 2.0f * (xx + yy), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return matrix;
}

Quaternion Quaternion::Lerp(const Quaternion& qtrn1, float k)
{
	float cos_angle = this->x*qtrn1.x + this->y*qtrn1.y + this->z*qtrn1.z + this->t*qtrn1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Quaternion qi = ((*this) * k0) + (qtrn1 * k1);
	return qi.Normalize();
}

Quaternion Quaternion::Slerp(const Quaternion& qtrn1, float k)
{
	float angle = acos(this->x*qtrn1.x + this->y*qtrn1.y + this->z*qtrn1.z + this->t*qtrn1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Quaternion qi = ((*this) * k0) + (qtrn1 * k1);
	return qi.Normalize();
}

bool Quaternion::operator==(const Quaternion& qtrn1)
{
	return (fabs(this->t - qtrn1.t) < slackThreshold && fabs(this->x - qtrn1.x) < slackThreshold &&
		fabs(this->y - qtrn1.y) < slackThreshold && fabs(this->z - qtrn1.z) < slackThreshold);
}

ostream& Quaternion::operator<<(ostream& stream)
{
	return stream << "(" << this->t << ", " << this->x << ", " << this->y << ", " << this->z << ")" << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& qtrn)
{
	return stream << "(" << qtrn.t << ", " << qtrn.x << ", " << qtrn.y << ", " << qtrn.z << ")" << std::endl;
}