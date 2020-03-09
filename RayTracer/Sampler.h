#pragma once
#include "Scene.h"

class Sampler {
public:
	void SampleRayIntersection(Scene & scene, int trials, const char * outputFile);
	void SampleRayIntersectionDepth(Scene & scene, int trials, const char * outputFile);
};