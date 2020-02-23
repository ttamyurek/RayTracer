#define _USE_MATH_DEFINES
#include "Vector.h"

Vector::Vector() {
	x = y = z = 0.0f;
}

Vector::Vector(float k) {
	x = k;
	y = k;
	z = k;
}

Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const float v[3]) {
	x = v[0];
	y = v[1];
	z = v[2];
}

Vector Vector::operator+(float scalar) const {
	return Vector(x + scalar, y + scalar, z + scalar);
}

Vector Vector::operator+(const Vector& vector) const {
	return Vector(x + vector.x, y + vector.y, z + vector.z);
}

Vector& Vector::operator+=(const Vector& vector) {
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;
	return *this;
}

Vector Vector::operator-(float scalar) const {
	return Vector(x - scalar, y - scalar, z - scalar);
}

Vector Vector::operator-(const Vector& vector) const {
	return Vector(x - vector.x, y - vector.y, z - vector.z);
}

Vector Vector::operator*(float scalar) const {
	return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator/(float scalar) const {
	return Vector(x / scalar, y / scalar, z / scalar);
}

Vector Vector::operator/(const Vector& vector) const {
	return Vector(x / vector.x, y / vector.y, z / vector.z);
}

Vector Vector::operator*(const Vector& vector) const {
	return Vector(x * vector.x, y * vector.y, z * vector.z);
}

Vector Vector::cross(const Vector& vector) const {
	// x    y    z
	//v[x] v[y] v[z]
	//o[x] o[y] o[z]
	Vector  result;
	result.x = y * vector.z - z * vector.y;
	result.y = -x * vector.z + z * vector.x;
	result.z = x * vector.y - y * vector.x;
	return result;
}

Vector Vector::SampleNormalOrientedHemisphere(const Vector& normal) {
	//random inputs
	float random1 = rand() / (float)RAND_MAX;
	float random12 = rand() / (float)RAND_MAX;

	//get uniform vector in hemisphere of (0,1,0)
	float sinTheta = sqrt(1.0f - random1 * random1);
	float phi = 2.0f * M_PI * random12;
	Vector sample = Vector(cos(phi) * sinTheta, random1, sin(phi) * sinTheta);

	//orient along normal
	//compute tangent and bitangent
	Vector tangent, bitangent;
	normal.CreateNormalSpace(tangent, bitangent);

	//transform to world space from normal space
	return Vector(sample.x * bitangent.x + sample.y * normal.x + sample.z * tangent.x,
		sample.x * bitangent.y + sample.y * normal.y + sample.z * tangent.y,
		sample.x * bitangent.z + sample.y * normal.z + sample.z * tangent.z);
}

void Vector::CreateNormalSpace(Vector& tangent, Vector& bitangent) const {
	if (abs(x) > abs(y))
		tangent = Vector(z, 0, -x) / sqrtf(x * x + z * z);
	else
		tangent = Vector(0, -z, y) / sqrtf(y * y + z * z);
	bitangent = this->cross(tangent);
}
