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
	

public:
	std::vector<Triangle> lightGeometry;
	Vector position;
	Vector direction;
	Vector color;
	float width;
	float height;
	
	AreaLight()
	{}

	AreaLight(Vector position, Vector direction, Vector color, float width, float height)
	{
		this->position = position;
		this->direction = direction.normalize();
		this->width = width;
		this->height = height;
		this->color = color;
		//Vector up = cross(Vector(1, 0, 0), direction).normalize() * height / 2;
		//Vector right = cross(Vector(0,0,1), direction).normalize() * width / 2;
		//lightGeometry.push_back(Triangle(position + Vector(-width / 2.0, -height / 2.0, 0.0), position + Vector(width / 2.0, -height / 2.0, 0.0), position + Vector(-width / 2.0, height / 2.0, 0.0)));
		//lightGeometry.push_back(Triangle(position + Vector(width / 2.0, -height / 2.0, 0.0), position + Vector(width / 2.0, height / 2.0, 0.0), position + Vector(-width / 2.0, height / 2.0, 0.0)));
		
		lightGeometry.push_back(Triangle(position + Vector(-width / 2.0, height / 2.0, 0.0), position + Vector(width / 2.0, -height / 2.0, 0.0), position + Vector(-width / 2.0, -height / 2.0, 0.0)));
		lightGeometry.push_back(Triangle(position + Vector(-width / 2.0, height / 2.0, 0.0), position + Vector(width / 2.0, height / 2.0, 0.0), position + Vector(width / 2.0, -height / 2.0, 0.0)));

		//lightGeometry.push_back(Triangle(position - up - right, position - right + up, position + right - up));
		//lightGeometry.push_back(Triangle(position - right + up, position + right + up, position + right - up));
		/*
		for (int i = 0; i < lightGeometry.size(); i++)
		{
			Vector axis = cross(Vector(0.0, 0.0, 1.0), this->direction).normalize();
			float angle = direction.x == 0. && direction.y == 0. ? 0.0 : atan(direction.x / direction.y);
			lightGeometry[i].rotate(angle, axis);
			//axis = cross(Vector(0.0, 1.0, 0.0), this->direction).normalize();
			//angle = direction.x == 0. && direction.z == 0. ? 0.0 : atan(direction.x / direction.z);
			//lightGeometry[i].rotate(angle, axis);
			//axis = cross(Vector(1.0, 0.0, 0.0), this->direction).normalize();
			//angle = direction.y == 0. && direction.z == 0. ? 0.0 : atan(direction.y / direction.z);
			//lightGeometry[i].rotate(angle, axis);
			lightGeometry[i].n0 = lightGeometry[i].n1 = lightGeometry[i].n2 = this->direction;
		}
		*/
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
		float dist = distance(position);
		return std::min(1.0, 1 / (0.25 + 0.1 * dist + 0.01 * dist * dist));
	}

	bool intersect(const Ray &ray)
	{
		return lightGeometry[0].Intersect(ray) || lightGeometry[1].Intersect(ray);
	}

	Vector sample(int index) //TODO: Make it random
	{
		return lightGeometry[index].sample().point;
	}

};