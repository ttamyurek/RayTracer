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
	//scene.loadScene("Scenes/area_light.ascii");
	scene.loadOBJ("Objects/simple_scene.obj");
	AreaLight *light = new AreaLight(Vector(-5., -2., 5.), Vector(0., 0., -1.), Vector(1.0), 1, 1);
	Camera *camera = new Camera(Vector(-13.f, -10.f, 10.f), Vector(13.0f, 10.0f, -10.0f), Vector(-0.1f, -0.1f, 1.0f), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	scene.add(camera);
	//Camera *camera = new Camera(Vector(-12.f, -0.f, 5.f), Vector(1.0, 0.0, -0.1), Vector(0.1f, 0.0f, 1.0f), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	//scene.add(camera);
	PointLight *plight = new PointLight(Vector(0., 0., 1));
	scene.add(light);
	Object *obj = new Object;
	obj->add(&light->lightGeometry[0]);
	obj->add(&light->lightGeometry[1]);
	//obj->rotate(radian(-45), Vector(0, 1, 0));
	//obj->rotate(radian(45), Vector(1, 0, 0));
	scene.add(&light->lightGeometry[0]);
	scene.add(&light->lightGeometry[1]);
	Material *material = new Material(Vector(0.2f, 0.3f, 0.6f));
	material->glossiness = 0.3;
	obj->material = material;
	scene.add(obj);

	float shadow = scene.SampleShadow(Vector(-3.5, -1.5, 1.5), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	shadow = scene.SampleShadow(Vector(-5., -2., 4.), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	shadow = scene.SampleShadow(Vector(5.0, 3.0, 0.7), 512);
	std::cout << "Shadow: " << shadow << std::endl;
	std::cout << "Shadow: " << shadow << std::endl;
	/*
	
	Camera *camera = new Camera(Vector(-13.f, -10.f, 10.f), Vector(13.0f, 10.0f, -10.0f), Vector(-0.1f, -0.1f, 1.0f), 10, radian(45), IMAGE_WIDTH, IMAGE_HEIGHT);
	scene.add(camera);
	AreaLight *light = new AreaLight(Vector(-13., 1., 10.), Vector(13., -1., -10.), 5, 5);
	DirectionalLight *dirlight = new DirectionalLight(Vector(13., 5., -10.));
	PointLight *plight = new PointLight(Vector(-13., -5., 10.));
	scene.add(plight);
	Material *material = new Material(Vector(0.2f, 0.3f, 0.6f));
	material->glossiness = 0.3;
	scene.objects[0]->material = material;
	*/
	/*Camera camera(Vector(0.0f), Vector(1.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, 1.0f), 10, 45.0 * 3.1415 / 180.0, 512, 512);
	PointLight plight(Vector(10.0f, 0.0f, 2.5f));
	scene.add(&plight);
	Object object;
	Triangle triangle(Vector(11.0f, 0.0f, 4.0f), Vector(11.0f, 3.0f, -1.0f), Vector(11.0f, -3.0f, -1.0f),
		Vector(-1.0, 0.0f, 0.0f), Vector(-1.0, 0.0f, 0.0f), Vector(-1.0, 0.0f, 0.0f));
	triangle.parent = &object;
	object.add(&triangle);
	Material material(Vector(0.2f, 0.3f, 0.6f));
	material.glossiness = 0.3;
	object.material = &material;

	scene.camera = &camera;
	scene.add(&triangle);*/
	
	Renderer renderer;

	renderer.Render(&scene, "Render Outputs/render.bmp");

	return 0;
}