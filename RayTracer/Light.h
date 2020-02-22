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

};

class DirectionalLight : public Light {
public:
	Vector direction;

};

class AreaLight : public Light {
public:
	Vector direction;
	float width;
	float height;

};