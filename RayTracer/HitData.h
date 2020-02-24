#pragma once
#include "Vector.h"
#include "Ray.h"
#include "Material.h"
#include "Primitive.h"

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

	HitData();

	HitData(
		bool hit,
		Vector position,
		Vector normal,
		float t,
		UV uv,
		Material *material,
		Ray ray,
		Primitive *primitive);
};