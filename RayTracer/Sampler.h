#pragma once
#include "Scene.h"

class Sampler {
public:
	void SampleSceneRayIntersection(Scene & scene, int trials, const char * outputFile);
};