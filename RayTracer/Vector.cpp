#include "Vector.h"
#include <random>

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

Vector& Vector::operator*=(const Vector& vector) {
	this->x *= vector.x;
	this->y *= vector.y;
	this->z *= vector.z;
	return *this;
}

Vector& Vector::operator/=(const Vector& vector) {
	this->x /= vector.x;
	this->y /= vector.y;
	this->z /= vector.z;
	return *this;
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

void Vector::rotate(float angle, Vector u)
{
	// Rodrigues' rotation formula?
	float rotationMatrix[3][3];
	rotationMatrix[0][0] = cos(angle) + u.x * u.x * (1 - cos(angle));
	rotationMatrix[1][0] = u.y * u.x * (1 - cos(angle)) + u.z * sin(angle);
	rotationMatrix[2][0] = u.z * u.x * (1 - cos(angle)) - u.y * sin(angle);

	rotationMatrix[0][1] = u.x * u.y * (1 - cos(angle)) - u.z * sin(angle);
	rotationMatrix[1][1] = cos(angle) + u.y * u.y * (1 - cos(angle));
	rotationMatrix[2][1] = u.z * u.y * (1 - cos(angle)) - u.x * sin(angle);

	rotationMatrix[0][2] = u.x * u.z * (1 - cos(angle)) + u.y * sin(angle);
	rotationMatrix[1][2] = u.y * u.z * (1 - cos(angle)) - u.x * sin(angle);
	rotationMatrix[2][2] = cos(angle) + u.z * u.z * (1 - cos(angle));

	float rx = rotationMatrix[0][0] * x + rotationMatrix[1][0] * y + rotationMatrix[2][0] * z;
	float ry = rotationMatrix[0][1] * x + rotationMatrix[1][1] * y + rotationMatrix[2][1] * z;
	float rz = rotationMatrix[0][2] * x + rotationMatrix[1][2] * y + rotationMatrix[2][2] * z;

	x = rx;
	y = ry;
	z = rz;
}

void Vector::rotateNormal(float angle, Vector u)
{
	float rotationMatrix[3][3];
	rotationMatrix[0][0] = cos(angle) + u.x * u.x * (1 - cos(angle));
	rotationMatrix[1][0] = u.y * u.x * (1 - cos(angle)) + u.z * sin(angle);
	rotationMatrix[2][0] = u.z * u.x * (1 - cos(angle)) - u.y * sin(angle);

	rotationMatrix[0][1] = u.x * u.y * (1 - cos(angle)) - u.z * sin(angle);
	rotationMatrix[1][1] = cos(angle) + u.y * u.y * (1 - cos(angle));
	rotationMatrix[2][1] = u.z * u.y * (1 - cos(angle)) - u.x * sin(angle);

	rotationMatrix[0][2] = u.x * u.z * (1 - cos(angle)) + u.y * sin(angle);
	rotationMatrix[1][2] = u.y * u.z * (1 - cos(angle)) - u.x * sin(angle);
	rotationMatrix[2][2] = cos(angle) + u.z * u.z * (1 - cos(angle));

	float rx = rotationMatrix[0][0] * x + rotationMatrix[0][1] * y + rotationMatrix[0][2] * z;
	float ry = rotationMatrix[1][0] * x + rotationMatrix[1][1] * y + rotationMatrix[1][2] * z;
	float rz = rotationMatrix[2][0] * x + rotationMatrix[2][1] * y + rotationMatrix[2][2] * z;

	x = rx;
	y = ry;
	z = rz;

	normalize();
}

Vector SampleNormalOrientedHemisphere(const Vector& normal) {
	//random inputs
	float random1 = rand() / (float)RAND_MAX;
	float random12 = rand() / (float)RAND_MAX;

	//get uniform vector in hemisphere of (0,1,0)
	float sinTheta = sqrt(1.0f - random1 * random1);
	float phi = 2.0f * pi * random12;
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

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1.0, 1.0);

Vector SampleRandomDirection()
{
	/*float R = 1.0f;
	float theta = distribution(generator) * pi;
	float phi = distribution(generator) * 2.0 * pi;
	float X = R * sin(theta) * cos(phi);
	float Y = R * sin(theta) * sin(phi);
	float Z = R * cos(theta);*/
	float X = distribution(generator);
	float Y = distribution(generator);
	float Z = distribution(generator);
	return Vector(X, Y, Z).normalize();
}

void Vector::CreateNormalSpace(Vector& tangent, Vector& bitangent) const {
	if (abs(x) > abs(y))
		tangent = Vector(z, 0, -x) / sqrtf(x * x + z * z);
	else
		tangent = Vector(0, -z, y) / sqrtf(y * y + z * z);
	bitangent = this->cross(tangent);
}
