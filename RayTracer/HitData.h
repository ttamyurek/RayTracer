#pragma once
#include "Ray.h"
#include "Material.h"
#include "Vector.h"
#include "Primitive.h"

class HitData
{
public:
	Vector position;
	Vector normal;
	float t;
	UV uv;
	Material material;
	Ray ray;
	Primitive *primitive;

	HitData(
		Vector position,
		Vector normal,
		float t,
		UV uv,
		Material *material,
		Ray ray,
		Primitive *primitive) : position(position), normal(normal), t(t), uv(uv), material(material), ray(ray), primitive(primitive)
	{}
};