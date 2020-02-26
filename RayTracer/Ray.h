#pragma once
#include "Vector.h"

class Ray {
private:
	Vector orig;
	Vector direc;
public:
	Ray()
	{

	}
	Ray(Vector origin, Vector direction)
	{
		this->orig = origin;
		this->direc = direction.normalize();
	}

	Vector origin() const {
		return orig;
	}
	Vector direction() const {
		return direc;
	}

	Vector at(float t) const {
		return orig + direc * t;
	}
};