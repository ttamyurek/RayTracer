#pragma once
#include <vector>
#include "Vector.h"
#include "Primitive.h"
#include "Ray.h"

class Light {
public:
	Vector color;

	virtual Vector getDirection(const Vector &position) const = 0;
	virtual float getAttenuation(const Vector &position) const = 0;
	virtual float distance(const Vector &position) const = 0;
	virtual bool intersect(const Ray &ray) = 0;
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
	bool intersect(const Ray &ray)
	{
		return false;
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
	bool intersect(const Ray &ray)
	{
		return false;// ray.direction() == direction;
	}
};

class AreaLight : public Light {
	std::vector<Triangle> lightGeometry;

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
		lightGeometry.push_back(Triangle(Vector(0.0, 0.0, 0.0), Vector(width, 0.0, 0.0), Vector(0.0, height, 0.0)));
		lightGeometry.push_back(Triangle(Vector(width, 0.0, 0.0), Vector(width, height, 0.0), Vector(0.0, height, 0.0)));
		Vector axis = cross(Vector(0.0, 0.0, 1.0), this->direction).normalize();
		float angle = atan(direction.x / direction.y);
		for (int i = 0; i < lightGeometry.size(); i++)
		{
			lightGeometry[i].rotate(angle, axis);
			lightGeometry[i].n0 = lightGeometry[i].n1 = lightGeometry[i].n2 = this->direction;
		}
		
	}

	Vector getDirection(const Vector &position) const
	{
		return -this->direction;
	}

	float distance(const Vector &position) const
	{
		return length(this->position - position);
	}

	float getAttenuation(const Vector &position) const
	{
		return 1.0f;
	}

	bool intersect(const Ray &ray)
	{
		return lightGeometry[0].Intersect(ray) || lightGeometry[1].Intersect(ray);
	}

};