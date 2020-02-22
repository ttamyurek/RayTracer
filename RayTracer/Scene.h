#pragma once
#include "Vector.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"
#include <vector>

struct HitData
{
	Vector position;
	Vector normal;
	float t;
	UV uv;
	Material material;
	Ray ray;
	Primitive *primitive;
};

class Scene {
public:
	std::vector<Object> objects;
	std::vector<Primitive> primitives;
	std::vector<Light> lights;
	Camera *camera;

	hitData Intersect(Ray ray)
	{

	}

};