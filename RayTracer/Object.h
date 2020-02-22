#pragma once
#include "Vector.h"
#include "Primitive.h"
#include "Material.h"
#include <vector>

class Object
{
	Vector position;
	Vector orientation;
	Vector scale;

	std::vector<Primitive> primitives;
	Material *material;

	Object()
	{
		position = Vector(0.0);
		orientation = Vector(0.0, 0.0, 1.0);
		scale = Vector(1.0);
	}
};