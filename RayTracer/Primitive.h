#pragma once
#include "Vector.h"
#include "Material.h"

class Primitive
{

};

class Sphere : public Primitive
{
	Vector center;
	float radius;
	Vector xaxis;
	float xlength;
	Vector yaxis;
	float ylength;
	Vector zaxis;
	float zlength;

	Sphere(Vector center, float radius) {
		this->center = center;
		this->radius = radius;
	}
};

struct UV
{
	float u;
	float v;
};

class Triangle : public Primitive
{
	Vector v0, v1, v2;
	Vector n0, n1, n2;
	UV uv0, uv1, uv2;
	Material *m0, *m1, *m2;

	Triangle(Vector v0, Vector v1, Vector v2) {
		this->v0 = v0;
		this->v1 = v1;
		this->v2 = v2;
		n0 = n1 = n2 = 0;
		uv0.u = uv0.v = uv1.u = uv1.v = uv2.u = uv2.v = 0;
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
};