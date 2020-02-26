#pragma once
#include "Scene.h"

class Renderer {
public:
	Scene *scene;

	bool Render(Scene *scene);

	Vector RenderPixel(int row, int col);
};