#pragma once
#include "Scene.h"

class Renderer {
public:
	Scene* scene;

	bool Render(Scene* scene, const char* outputFile);
	Vector RenderPixel(int row, int col);
	Vector TracePath(int row, int col, int SPP);
	Vector TraceLight(int row, int col, int SPP);
	Vector TraceRayFromPoint(Vector point);
	bool RenderRandom(Scene* scene, const char* outputFile);
	bool RenderDepth(Scene* scene, const char* outputFile);
};