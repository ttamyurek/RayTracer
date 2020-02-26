#pragma once
#include "Vector.h"

class Material
{
public:
	Vector ambient;
	Vector diffuse;
	Vector specular;
	Vector emissive; // Self Illumination
	float glossiness;
	float opacity;
	// TODO: Texture Maps

	Material()
	{
		ambient = Vector(1.0f);
		diffuse = Vector(0.5f);
		specular = Vector(1.0f);
		emissive = Vector(0.0f);
		glossiness = 0.0f;
		opacity = 1.0f;
	}

	Material(Vector diffColor)
	{
		ambient = Vector(1.0f);
		diffuse = diffColor;
		specular = Vector(1.0f);
		emissive = Vector(0.0f);
		glossiness = 0.0f;
		opacity = 1.0f;
	}

	Material(Vector ambColor, Vector diffColor, Vector specColor, Vector emissColor, float glossLevel, float opacLevel)
	{
		ambient = ambColor;
		diffuse = diffColor;
		specular = specColor;
		emissive = emissColor;
		glossiness = glossLevel;
		opacity = opacLevel;
	}
};