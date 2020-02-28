#pragma once
#include "Vector.h"

class Light {
public:
	Vector position;
	Vector color;

	virtual Vector getDirection(const Vector &position) const = 0;
	virtual float distance(const Vector &position) const = 0;
};

class PointLight : public Light {
public:
	float attenutationDistance = 200.0f;

	PointLight()
	{}

	PointLight(Vector position)
	{
		this->position = position;
	}

	Vector getDirection(const Vector &position) const
	{
		return normalize(this->position - position);
	}

	float distance(const Vector &position) const
	{
		return length(this->position - position);
	}
};

class DirectionalLight : public Light {
public:
	Vector direction;
	DirectionalLight()
	{}
	DirectionalLight(Vector position, Vector direction)
	{
		this->position = position;
		this->direction = direction.normalize();
	}

	Vector getDirection(const Vector &position) const
	{
		return -this->direction;
	}

	float distance(const Vector &position) const
	{
		return length(this->position - position);
	}
};

class AreaLight : public Light {
public:
	Vector direction;
	float width;
	float height;

	AreaLight()
	{}

	AreaLight(Vector position, Vector direction, float width, float height)
	{
		this->position = position;
		this->direction = direction.normalize();
		this->width = width;
		this->height = height;
	}

	Vector getDirection(const Vector &position) const
	{
		return -this->direction;
	}

	float distance(const Vector &position) const
	{
		return length(this->position - position);
	}

};