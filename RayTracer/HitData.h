#pragma once
#include "Ray.h"
#include "Material.h"
#include "Vector.h"
#include "Primitive.h"

float maxDistance = std::numeric_limits<float>::max();

class HitData
{
public:
	bool hit;
	Vector position;
	Vector normal;
	float t;
	UV uv;
	Material *material;
	Ray ray;
	Primitive *primitive;

	HitData()
	{
		hit = false;
		position = Vector(0.0);
		normal = Vector(0.0);
		t = 0.0;
		uv.u = uv.v = 0.0;
		material = nullptr;
		primitive = nullptr;
	}

	HitData(
		bool hit,
		Vector position,
		Vector normal,
		float t,
		UV uv,
		Material *material,
		Ray ray,
		Primitive *primitive) : hit(hit), position(position), normal(normal), t(t), uv(uv), material(material), ray(ray), primitive(primitive)
	{}
};