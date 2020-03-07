#include <iostream>
#include "Vector.h"
#include "Scene.h"
#include "Camera.h"
#include "Primitive.h"
#include "Renderer.h"

#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 512

using namespace std;

int main()
{
	Scene scene(IMAGE_WIDTH, IMAGE_HEIGHT);
	scene.loadOBJ("Objects/horse.obj");
	Camera *camera1 = new Camera(Vector(-13.f, -10.f, 10.f), Vector(13.0f, 10.0f, -10.0f), Vector(-0.1f, -0.1f, 1.0f), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	scene.add(camera1);
	PointLight *plight1 = new PointLight(Vector(0., 0., 1));
	scene.add(plight1);
	Renderer renderer1;
	renderer1.RenderDepth(&scene, "Render Outputs/render_horse.bmp");
	/*
	Camera *camera = new Camera(Vector(-12, 0, 0), Vector(1, 0, 0), Vector(0, 0, 1), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	scene.add(camera);
	Triangle *triangle = new Triangle(Vector(0, -3, -4), Vector(0, 0, 4), Vector(0, 3, -4));
	Object *object = new Object(triangle);
	object->material = new Material(Vector(1.0f, .5f, .5f));
	
	scene.add(triangle);
	scene.add(object);
	PointLight *light = new PointLight(Vector(-1, 0, 5));
	scene.add(light);*/
	
	//scene.loadScene("Scenes/area_light.ascii");
	Scene simpleScene(IMAGE_WIDTH, IMAGE_HEIGHT);
	simpleScene.loadOBJ("Objects/simple_scene.obj");
	AreaLight *light = new AreaLight(Vector(-5., -2., 5.), Vector(0., 0., -1.), Vector(1.0), 1, 1);
	Camera *camera = new Camera(Vector(-13.f, -10.f, 10.f), Vector(13.0f, 10.0f, -10.0f), Vector(-0.1f, -0.1f, 1.0f), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	simpleScene.add(camera);
	PointLight *plight = new PointLight(Vector(0., 0., 1));
	simpleScene.add(light);
	Object *obj = new Object;
	obj->add(&light->lightGeometry[0]);
	obj->add(&light->lightGeometry[1]);
	//obj->rotate(radian(-45), Vector(0, 1, 0));
	//obj->rotate(radian(45), Vector(1, 0, 0));
	simpleScene.add(&light->lightGeometry[0]);
	simpleScene.add(&light->lightGeometry[1]);
	Material *material = new Material(Vector(0.2f, 0.3f, 0.6f));
	material->glossiness = 0.3;
	obj->material = material;
	simpleScene.add(obj);

	float shadow = simpleScene.SampleShadow(Vector(-3.5, -1.5, 1.5), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	shadow = simpleScene.SampleShadow(Vector(-5., -2., 4.), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	shadow = simpleScene.SampleShadow(Vector(5.0, 3.0, 0.7), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	std::cout << "Shadow: " << shadow << std::endl;

	Renderer renderer;
	//renderer.RenderDepth(&simpleScene, "Render Outputs/render.bmp");

	return 0;
}