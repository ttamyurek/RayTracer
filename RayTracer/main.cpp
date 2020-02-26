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
	Camera camera(Vector(0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), 10, 45.0 * 3.1415 / 180.0, 512, 512);
	PointLight plight(Vector(10.0f, 0.0f, 2.5f));
	scene.lights.push_back(&plight);
	Object object;
	Triangle triangle(Vector(11.0f, 0.0f, 4.0f), Vector(11.0f, 3.0f, -1.0f), Vector(11.0f, -3.0f, -1.0f),
		Vector(-1.0, 0.0f, 0.0f), Vector(-1.0, 0.0f, 0.0f), Vector(-1.0, 0.0f, 0.0f));
	triangle.parent = &object;
	object.primitives.push_back(&triangle);
	Material material(Vector(0.2f, 0.3f, 0.6f));
	material.glossiness = 0.3;
	object.material = &material;

	scene.camera = &camera;
	scene.primitives.push_back(&triangle);
	
	Renderer renderer;

	renderer.Render(&scene);

	return 0;
}