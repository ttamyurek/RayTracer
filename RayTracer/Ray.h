#pragma once
#include "Vector.h"

class Ray {
private:
	Vector origin;
	Vector direction;
public:
	Ray(Vector origin, Vector direction)
	{
		this->origin = origin;
		this->direction = direction;
	}
	Vector origin() {
		return origin;
	}
	Vector direction() {
		return direction;
	}
};