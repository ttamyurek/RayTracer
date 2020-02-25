#pragma once
#include "Vector.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"
#include "HitData.h"
#include <vector>

class Scene {
public:
	std::vector<Object*> objects;
	std::vector<Primitive*> primitives;
	std::vector<Light> lights;
	Camera *camera;

	HitData Intersect(const Ray &ray);

};