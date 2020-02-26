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
			Vector pixelColor = RenderPixel(i, j);
			frameBuffer.setPixel(i, j, pixelColor);
			if(j == 0) std::cout << "Pixel #" << i << ", " << j << std::endl;
		}
	}

	//TODO: Save Image
	frameBuffer.save((const char*)"render.bmp");
	//TODO: Delete Image
	return true;
}

Vector Renderer::RenderPixel(int row, int col)
{
	Ray ray = scene->camera->shootRay(row, col); // (Row, Col)
	HitData hitData = scene->Intersect(ray);
	//TODO: shadowray (shade)
	if (hitData.hit)
	{
		Vector color(0.0f);
		for (auto light : scene->lights)
		{
			Vector diffComp = hitData.material->diffuse * std::max(dot((light->position - hitData.position).normalize(), hitData.normal), 0.0f) * hitData.material->opacity;
			color += diffComp;
		}
		
		//scene->ShadowRay(hitData.position);
		return color;
	}
	else
		return Vector(0.0);
}