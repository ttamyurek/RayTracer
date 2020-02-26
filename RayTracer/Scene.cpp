#include "Scene.h"

HitData Scene::Intersect(const Ray &ray)
{
	HitData closestHitData;
	HitData hitData;
	float closest = infinity;
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

Vector Scene::ShadowRay(const Ray &shadowRay, float maxDistance) {
	Vector shadowFactor(1.0f);
	HitData hitData;
	for (auto primitive : primitives)
	{
		if (primitive->Intersect(shadowRay, hitData))
		{
			if (hitData.t > maxDistance || hitData.material->opacity > 0.98) return Vector(0.0f);
			Vector diffuse = hitData.material->diffuse;
			shadowFactor *= ((diffuse / diffuse.Max()) * (1.0 - hitData.material->opacity));
		}
	}
	return shadowFactor;
}