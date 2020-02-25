#include <iostream>
#include "Vector.h"
#include "Scene.h"
#include "Camera.h"
#include "Primitive.h"
#include "Renderer.h"

using namespace std;

int main()
{
	Scene scene;
	Camera camera(Vector(0.0f), Vector(0.0f, 0.0f, 1.0f), Vector(0.0f, 1.0f, 0.0f), 0.5, 45.0 * 3.1415 / 180.0, 512, 512);
	Triangle triangle(Vector(0.0f, 0.0f, 1.0f), Vector(0.5f, 1.0f, 1.0f), Vector(1.0f, 0.0f, 1.0f));

	scene.camera = &camera;
	scene.primitives.push_back(&triangle);
	
	Renderer renderer;

	renderer.Render(&scene);

	return 0;
}