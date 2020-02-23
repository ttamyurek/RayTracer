#pragma once
#include "Scene.h"

class Renderer {
public:
	Scene *scene;

	bool Render(Scene *scene)
	{
		int imageWidth = scene->camera->imageWidth;
		int imageHeight = scene->camera->imageHeight;
		for (int i = 0; i < imageHeight; i++)
		{
			for (int j = 0; j < imageWidth; j++)
			{
				Ray ray = scene->camera->shootRay(j, i); // (Row, Col)
				Vector pixelColor = RenderPixel(ray);
			}
		}
	}

	Vector RenderPixel(Ray &ray)
	{
		HitData hitdata = scene->Intersect(ray);
		//TODO: shadowray (shade)
	}
};