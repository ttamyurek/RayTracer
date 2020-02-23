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

	HitData Intersect(const Ray &ray)
	{
		HitData closestHitData;
		HitData hitData;
		float closest = maxDistance;
		for (auto primitive : primitives)
		{
			if (primitive->Intersect(ray, hitData) && closest > hitData.t)
			{
				closest = hitData.t;
				closestHitData = hitData;
			}
		}
		return closestHitData;
	}

};