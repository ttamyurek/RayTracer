#include "Renderer.h"

bool Renderer::Render(Scene *scene)
{
	int imageWidth = scene->camera->imageWidth;
	int imageHeight = scene->camera->imageHeight;
	int **image = new int*[imageWidth];
	for (int i = 0; i < imageWidth; i++)
		image[i] = new int[imageHeight];

	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			Ray ray = scene->camera->shootRay(j, i); // (Row, Col)
			Vector pixelColor = RenderPixel(ray);
		}
	}

	//TODO: Save Image

	//TODO: Delete Image
}

Vector Renderer::RenderPixel(Ray &ray)
{
	HitData hitData = scene->Intersect(ray);
	//TODO: shadowray (shade)
	if (hitData.hit)
	{
		return Vector(0.0);
	}
}