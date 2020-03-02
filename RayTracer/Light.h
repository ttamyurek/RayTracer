#pragma once
#include "Vector.h"

class Light {
public:
	Vector color;

	virtual Vector getDirection(const Vector &position) const = 0;
	virtual float getAttenuation(const Vector &position) const = 0;
	virtual float distance(const Vector &position) const = 0;
};

class PointLight : public Light {
public:
	Vector position;

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
	float getAttenuation(const Vector &position) const
	{
		float dist = distance(position);
		return std::min(1.0, 1 / (0.25 + 0.1 * dist + 0.01 * dist * dist));
	}
};

class DirectionalLight : public Light {
public:
	Vector direction;
	float maxDistance = 100.0f;
	DirectionalLight()
	{}
	DirectionalLight(Vector direction)
	{
		this->direction = direction.normalize();
	}

	DirectionalLight(Vector direction, float maxDistance)
	{
		this->direction = direction.normalize();
		this->maxDistance = maxDistance;
	}

	Vector getDirection(const Vector &position) const
	{
		return -this->direction;
	}

	float distance(const Vector &position) const
	{
		return maxDistance;
	}

	float getAttenuation(const Vector &position) const
	{
		return 1.0f;
	}
};

class AreaLight : public Light {
public:
	Vector position;
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