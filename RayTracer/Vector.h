#include <cmath>
#include <algorithm>

//This is a class of 3D Vector
class Vector {
public:
	// data
	float x, y, z;

	// Initialize to (0,0,0)
	Vector();

	// Initialize to (x,y,z)
	Vector(float x, float y, float z);

	// Intialize to (v[0], v[1], v[2])
	Vector(const float v[3]);

	// Random unit vector in hemisphere of normal
	static Vector SampleNormalOrientedHemisphere(const Vector& normal);

	void CreateNormalSpace(Vector& tangent, Vector& bitangent) const;

	// Addition
	Vector operator+(float scalar) const;
	Vector operator+(const Vector& vector) const;
	Vector& operator+=(const Vector& vector);

	// Subtraction
	Vector operator-(float scalar) const;
	Vector operator-(const Vector& vector) const;

	// Multiplication
	Vector operator*(float scalar) const;
	Vector operator*(const Vector& vector) const;

	// Division
	Vector operator/(float scalar) const;
	Vector operator/(const Vector& vector) const;

	// Cross Product
	Vector cross(const Vector& vector) const;

	// Dot Product
	float dot(const Vector& vector) const {
		return x * vector.x + y * vector.y + z * vector.z;
	}

	// Reflect across normal
	Vector reflect(const Vector& normal) const {
		return normal * (normal.dot(*this)) * 2 - *this;
	}

	// Refract across normal
	// Returns true if total interal reflection occurs
	// Returns false normally and puts refraction vector in refractDir
	bool refract(const Vector& normal, float n, Vector& refractDir) const {
		float cosi = this->dot(normal);
		float ni = 1;
		float nt = n;
		Vector norm = normal;
		//flip if backwards
		if (cosi < 0)
			cosi = -cosi;
		else {
			std::swap(ni, nt);
			norm = -norm;
		}
		float eta = ni / nt;
		float k = 1 - eta * eta * (1 - cosi * cosi);
		if (k < 0)
			return true;
		refractDir = (*this) * eta + norm * (eta * cosi - sqrtf(k));
		return false;
	}

	// Square Root
	Vector SquareRoot() {
		return Vector(sqrt(x), sqrt(y), sqrt(z));
	}

	// Exponential
	Vector Exp() {
		return Vector(exp(x), exp(y), exp(z));
	}

	// Luminance
	float Luminance() {
		return 0.2126f * x + 0.7152f * y + 0.0722 * z;
	}

	float Max() {
		return std::max(x, std::max(y, z));
	}
	float Min() {
		return std::min(x, std::min(y, z));
	}

	float Get(int axis) {
		if (axis == 0)
			return x;
		if (axis == 1)
			return y;
		return z;
	}

	void Set(int axis, float value) {
		if (axis == 0)
			x = value;
		else if (axis == 1)
			y = value;
		else if (axis == 2)
			z = value;
	}

	Vector operator -() const {
		return Vector(-x, -y, -z);
	}

	float length() const {
		return sqrt(x*x + y * y + z * z);
	}

	Vector normalize() const {
		return *this / length();
	}
};
