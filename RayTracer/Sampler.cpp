#include "Sampler.h"
#include "RandomGenerator.h"
#include <fstream>
#include "Image.h"

void Sampler::SampleRayIntersection(Scene& scene, int trials, const char* outputFile)
{
	std::ofstream rayIntersectionFile;
	rayIntersectionFile.open(outputFile);
	rayIntersectionFile << "X,Y,Z,Theta,Phi,Intersection" << std::endl;
	for (int i = 0; i < trials; i++)
	{
		float u = randf();
		float v = randf();
		float theta = 2 * pi * u;
		float phi = acos(2 * v - 1);

		float X = sin(theta) * cos(phi);
		float Y = sin(theta) * sin(phi);
		float Z = cos(theta);

		Vector rayOrigin(randf(-1, 1), randf(-1, 1), randf(-1, 1));
		Vector rayDirection(X, Y, Z);
		Ray randomRay(rayOrigin, rayDirection);
		//<< std::setprecision(8)
		rayIntersectionFile << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << "," << u << "," << v << "," << scene.Intersect(randomRay) << std::endl;
		if (i % 1000 == 0)
			std::cout << "Sample #" << i << " out of " << trials << std::endl;
	}
	rayIntersectionFile.close();
}

void Sampler::SampleRayIntersectionDepth(Scene& scene, int trials, const char* outputFile)
{
	std::ofstream rayIntersectionFile;
	rayIntersectionFile.open(outputFile);
	rayIntersectionFile << "X,Y,Z,Theta,Phi,IntersectionDepth" << std::endl;
	for (int i = 0; i < trials; i++)
	{
		float u = randf();
		float v = randf();
		float theta = 2 * pi * u;
		float phi = acos(2 * v - 1);

		float X = sin(theta) * cos(phi);
		float Y = sin(theta) * sin(phi);
		float Z = cos(theta);

		Vector rayOrigin(randf(-1, 1), randf(-1, 1), randf(-1, 1));
		Vector rayDirection(X, Y, Z);
		Ray randomRay(rayOrigin, rayDirection);

		rayIntersectionFile << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << "," << u << "," << v << "," << scene.ClosestIntersection(randomRay).t << std::endl;
		if (i % 1000 == 0)
			std::cout << "Sample #" << i << " out of " << trials << std::endl;
	}
	rayIntersectionFile.close();
}

void Sampler::SampleCameraRayIntersection(Scene& scene, Camera camera, const char* outputFile)
{
	std::ofstream rayIntersectionFile(outputFile);
	rayIntersectionFile << "X,Y,Z,Theta,Phi,Intersection" << std::endl;
	for (int i = 0; i < camera.imageHeight; i++)
	{
		for (int j = 0; j < camera.imageWidth; j++)
		{
			Ray ray = camera.shootJitteredRay(i, j, 1.0, 0.0);
			Vector rayDirection = ray.direction().sphericalCoordinates();
			float u = cos(rayDirection.y);
			float v = rayDirection.z / pi;
			rayIntersectionFile << ray.origin().x << "," << ray.origin().y << "," << ray.origin().z
				<< "," << u << "," << v << "," << scene.Intersect(ray) << std::endl;
			if (j == 0)
				std::cout << "Row #" << i << " out of " << camera.imageHeight << std::endl;
		}
	}
	rayIntersectionFile.close();
}

void Sampler::SampleCameraRayIntersection(Scene& scene, Camera camera, int gradientMultiplier, const char* outputFile)
{
	std::ofstream rayIntersectionFile(outputFile);
	rayIntersectionFile << "X,Y,Z,Theta,Phi,Intersection" << std::endl;

	std::vector<bool> intersection;
	intersection.resize(camera.imageWidth * camera.imageHeight);
	Image samplingDensity(camera.imageWidth, camera.imageHeight);

	for (int j = 0; j < camera.imageWidth; j++)
	{
		Ray ray = camera.shootJitteredRay(0, j, 1.0, 0.0);
		Vector rayDirection = ray.direction().sphericalCoordinates();
		float u = cos(rayDirection.y);
		float v = rayDirection.z / pi;
		bool intersect = scene.Intersect(ray);
		intersection[j] = intersect;
		rayIntersectionFile << ray.origin().x << "," << ray.origin().y << "," << ray.origin().z
			<< "," << u << "," << v << "," << intersect << std::endl;
		samplingDensity.setPixel(0, j, Vector(1.0 / gradientMultiplier));
	}

	for (int i = 0; i < camera.imageHeight; i++)
	{
		Ray ray = camera.shootJitteredRay(i, 0, 1.0, 0.0);
		Vector rayDirection = ray.direction().sphericalCoordinates();
		float u = cos(rayDirection.y);
		float v = rayDirection.z / pi;
		bool intersect = scene.Intersect(ray);
		intersection[i * camera.imageWidth] = intersect;
		rayIntersectionFile << ray.origin().x << "," << ray.origin().y << "," << ray.origin().z
			<< "," << u << "," << v << "," << intersect << std::endl;
		samplingDensity.setPixel(i, 0, Vector(1.0 / gradientMultiplier));
	}


	for (int i = 1; i < camera.imageHeight; i++)
	{
		for (int j = 1; j < camera.imageWidth; j++)
		{
			Ray ray = camera.shootJitteredRay(i, j, 1.0, 0.0);
			Vector rayDirection = ray.direction().sphericalCoordinates();
			float u = cos(rayDirection.y);
			float v = rayDirection.z / pi;
			bool intersect = scene.Intersect(ray);
			intersection[i * camera.imageWidth + j] = intersect;
			int gradient = 2 * intersect - intersection[(i - 1) * camera.imageWidth + j] - intersection[i * camera.imageWidth + j - 1];
			rayIntersectionFile << ray.origin().x << "," << ray.origin().y << "," << ray.origin().z
				<< "," << u << "," << v << "," << intersect << std::endl;
			samplingDensity.setPixel(i, j, Vector(abs(gradient) / 2.0));
			for (int k = 0; k < abs(gradient) * gradientMultiplier / 2; k++)
			{
				ray = camera.shootJitteredRay(i, j, 2.0, -0.5);
				rayDirection = ray.direction().sphericalCoordinates();
				u = cos(rayDirection.y);
				v = rayDirection.z / pi;
				rayIntersectionFile << ray.origin().x << "," << ray.origin().y << "," << ray.origin().z
					<< "," << u << "," << v << "," << intersect << std::endl;
			}
			if (j == 1)
				std::cout << "Row #" << i << " out of " << camera.imageHeight << std::endl;
		}
	}
	rayIntersectionFile.close();
	samplingDensity.save("RayTracer/Samples/samplingDensity.bmp");
}