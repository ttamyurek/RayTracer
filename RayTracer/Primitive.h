#pragma once
#include "Vector.h"
#include "Ray.h"
//#include "HitData.h"
//#include "Object.h"

class HitData;
class Object;

class Primitive
{
public:
	Object* parent;
	virtual bool Intersect(const Ray &ray, HitData &hitData) = 0;
	virtual void rotate(float angle, Vector axis) = 0;
};

class Sphere : public Primitive
{
public:
	Vector center;
	float radius;
	Vector xaxis;
	float xlength;
	Vector yaxis;
	float ylength;
	Vector zaxis;
	float zlength;
	Sphere(){}

	Sphere(Vector center, float radius) {
		this->center = center;
		this->radius = radius;
	}

	bool Intersect(const Ray &ray, HitData &hitData);

	void rotate(float angle, Vector axis)
	{
		xaxis.rotate(angle, axis);
		yaxis.rotate(angle, axis);
		zaxis.rotate(angle, axis);

	}
};

struct UV
{
	float u;
	float v;
};

class Triangle : public Primitive
{
public:
	Vector v0, v1, v2;
	Vector n0, n1, n2;
	UV uv0, uv1, uv2;
    bool doubleSided = false;

	// TODO: Material per Vertex?
	//Material *m0, *m1, *m2;
	Triangle(){}

	Triangle(Vector v0, Vector v1, Vector v2) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		n0 = n1 = n2 = Vector(0.0);
		uv0.u = uv0.v = uv1.u = uv1.v = uv2.u = uv2.v = 0;
		calculateNormal();
	}

	Triangle(Vector v0, Vector v1, Vector v2, Vector n0, Vector n1, Vector n2) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		this->n0 = n0;
		this->n1 = n1;
		this->n2 = n2;
		uv0.u = uv0.v = uv1.u = uv1.v = uv2.u = uv2.v = 0;
	}

	bool Intersect(const Ray &ray, HitData &hitData);

	void calculateNormal()
	{
		n0 = n1 = n2 = cross(v1 - v0, v2 - v0).normalize();
	}

	void rotate(float angle, Vector axis)
	{
		v0.rotate(angle, axis);
		v1.rotate(angle, axis);
		v2.rotate(angle, axis);

		n0.rotateNormal(angle, axis);
		n1.rotateNormal(angle, axis);
		n2.rotateNormal(angle, axis);
	}
};


constexpr float infinity = std::numeric_limits<float>::max();

