#pragma once
#include "Scene.h"

class Renderer {
public:
	Scene *scene;

	bool Render(Scene *scene, const char *outputFile);

	Vector RenderPixel(int row, int col);
	Vector rayCast(int row, int col, int SPP);
};