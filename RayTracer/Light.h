#pragma once
#include "Vector.h"

class Light {
public:
	Vector position;
	Vector color;
};

class PointLight : public Light {
public:
	float attenutationDistance = 200.0f;

	PointLight(Vector position)
	{
		this->position = position;
	}
};

class DirectionalLight : public Light {
public:
	Vector direction;

	DirectionalLight(Vector position, Vector direction)
	{
		this->position = position;
		this->direction = direction.normalize();
	}
};

class AreaLight : public Light {
public:
	Vector direction;
	float width;
	float height;

	AreaLight(Vector position, Vector direction, float width, float height)
	{
		this->position = position;
		this->direction = direction.normalize();
		this->width = width;
		this->height = height;
	}

};