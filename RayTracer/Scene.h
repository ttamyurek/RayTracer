#pragma once
#include "Vector.h"
#include "Camera.h"
#include "Object.h"
#include "Ray.h"
#include <vector>

class Scene {

	std::vector<Object> objects;
	std::vector<Primitive> primitives;

};