#include "Renderer.h"
#include "Image.h"
#include <iostream>
#include <random>

bool Renderer::Render(Scene *scene, const char *outputFile)
{
	this->scene = scene;
	int imageWidth = scene->camera->imageWidth;
	int imageHeight = scene->camera->imageHeight;
	Image frameBuffer(imageWidth, imageHeight);

	for (int i = 0; i < imageHeight; i++)
	{
		for (int j = 0; j < imageWidth; j++)
		{
			Vector pixelColor = RayCast(i, j, 256);
			frameBuffer.setPixel(i, j, pixelColor);
			if(j == 0) std::cout << "Pixel #" << i << ", " << j << std::endl;
		}
	}

	frameBuffer.save(outputFile);

	return true;
}

Vector Renderer::RenderPixel(int row, int col)
{
	Ray ray = scene->camera->shootRay(row, col); // (Row, Col)
	HitData hitData = scene->Intersect(ray);
	//TODO: shadowray (shade)
	Material *material = hitData.material;
	Vector shadowFactor(1.0);
	if (hitData.hit)
	{
		Vector color(0.0f);
		Vector directIllumination(0.0f);
		for (auto &light : scene->lights)
		{
			Vector lightDir = light->getDirection(hitData.position);
			Vector reflectedDir = lightDir.reflect(hitData.normal).normalize();

			Vector diffComp = material->diffuse * std::max(dot(lightDir, hitData.normal), 0.0f) * material->opacity;
			Vector specComp = material->specular * pow(std::max(dot(reflectedDir, -ray.direction()), 0.0f), material->glossiness * 128.0f);
			
			Ray shadowRay(hitData.position, lightDir);
			shadowFactor = scene->ShadowRay(shadowRay, light->distance(hitData.position));
			directIllumination += shadowFactor * light->color * light->getAttenuation(hitData.position) * ( diffComp + specComp );
		}
		//Vector reflectedRayDir = ray.direction().reflect(hitData.position).normalize();
		//ray = Ray(hitData.position, reflectedRayDir);
		//hitData = scene->Intersect(ray);
		//Vector refRadiance = hitData.hit ? hitData.material->diffuse: Vector(0.0);
		//Vector refComp = material->specular * refRadiance;
		Vector ambComp = material->ambient * material->diffuse * material->opacity;
		color = ambComp + directIllumination;
		return color;
	}
	else
		return Vector(0.0);
}

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

Vector Renderer::RayCast(int row, int col, int SPP)
{
	Ray ray = scene->camera->shootRay(row, col); // (Row, Col)
	HitData hitData = scene->Intersect(ray);
	//TODO: shadowray (shade)
	Material *material = hitData.material;
	Vector shadow(0.0);
	if (hitData.hit)
	{
		Vector color(0.0f);
		Vector directIllumination(0.0f);
		for (auto &light : scene->lights)
		{
			for (int i = 0; i < SPP; i++)
			{
				Vector rayDir = SampleNormalOrientedHemisphere(hitData.normal);
				Ray shadowRay(hitData.position, rayDir);
				if (light->intersect(shadowRay))
					shadow += scene->ShadowRay(shadowRay, light->distance(hitData.position));
			}
			shadow /= SPP;
			color += shadow;
		}
		return color * 5;
	}
	else
		return Vector(0.0);
}