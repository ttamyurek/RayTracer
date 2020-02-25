#include "Scene.h"

HitData Scene::Intersect(const Ray &ray)
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