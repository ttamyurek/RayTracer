#include "Sampler.h"
#include "RandomGenerator.h"
#include <fstream>
//#include <iomanip>

void Sampler::SampleRayIntersection(Scene & scene, int trials, const char *outputFile)
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
		//Vector rayOrigin(0, 1, 0);
		Vector rayDirection(X, Y, Z);
		Ray randomRay(rayOrigin, rayDirection);
		//<< std::setprecision(8)
		rayIntersectionFile  << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << "," << u << "," << v << "," << scene.Intersect(randomRay) << std::endl;
		if(i % 1000 == 0)
			std::cout << "Sample #" <<  i << " out of " << trials << std::endl;
	}
	rayIntersectionFile.close();
}

void Sampler::SampleRayIntersectionDepth(Scene & scene, int trials, const char *outputFile)
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
