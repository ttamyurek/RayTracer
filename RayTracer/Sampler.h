#pragma once
#include "Scene.h"

class Sampler {
public:
	void SampleRayIntersection(Scene& scene, int trials, const char* outputFile);
	void SampleRayIntersectionDepth(Scene& scene, int trials, const char* outputFile);
	void SampleCameraRayIntersection(Scene& scene, Camera camera, const char* outputFile);
	void SampleCameraRayIntersection(Scene& scene, Camera camera, int gradientMultiplier, const char* outputFile);
};