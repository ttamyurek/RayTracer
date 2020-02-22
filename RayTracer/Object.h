#pragma once
#include "Vector.h"
#include "Primitive.h"
#include "Material.h"
#include <vector>

class Object
{
	std::vector<Primitive> primitives;
	Material *material;
};