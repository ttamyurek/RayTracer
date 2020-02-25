#include "Renderer.h"
#include "Image.h"
#include <iostream>

bool Renderer::Render(Scene *scene)
{
	this->scene = scene;
	int imageWidth = scene->camera->imageWidth;
	int imageHeight = scene->camera->imageHeight;
	Image frameBuffer(imageWidth, imageHeight);

	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			Ray ray = scene->camera->shootRay(j, i); // (Row, Col)
			Vector pixelColor = RenderPixel(ray);
			frameBuffer.setPixel(i, j, pixelColor);
			if(j == 0) std::cout << "Pixel #" << i << ", " << j << std::endl;
		}
	}

	//TODO: Save Image
	frameBuffer.save((const char*)"render.bmp");
	//TODO: Delete Image
	return true;
}

Vector Renderer::RenderPixel(Ray &ray)
{
	HitData hitData = scene->Intersect(ray);
	//TODO: shadowray (shade)
	if (hitData.hit)
	{
		return Vector(1.0);
	}
}