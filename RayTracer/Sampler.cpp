#include "Sampler.h"
#include "RandomGenerator.h"
#include <fstream>
//#include <iomanip>

void Sampler::SampleSceneRayIntersection(Scene & scene, int trials, const char *outputFile)
{
	std::ofstream rayIntersectionFile;
	rayIntersectionFile.open(outputFile);
	rayIntersectionFile << "X,Y,Z,Theta,Phi,Intersection" << std::endl;
	for (int i = 0; i < trials; i++)
	{
		float R = 1.0f;
		float theta = randf();
		float phi = randf();
		float X = R * sin(theta * pi) * cos(phi * 2 * pi);
		float Y = R * sin(theta * pi) * sin(phi * 2 * pi);
		float Z = R * cos(theta * pi);
		Vector rayOrigin(randf(-1, 1), randf(-1, 1), randf(-1, 1));
		Vector rayDirection(X, Y, Z);
		Ray randomRay(rayOrigin, rayDirection);
		//<< std::setprecision(8)
		rayIntersectionFile  << rayOrigin.x << "," << rayOrigin.y << "," << rayOrigin.z << "," << theta << "," << phi << "," << scene.Intersect(randomRay) << std::endl;
		if(i % 1000 == 0)
			std::cout << "Sample #" <<  i << " out of " << trials << std::endl;
	}
	rayIntersectionFile.close();
}
