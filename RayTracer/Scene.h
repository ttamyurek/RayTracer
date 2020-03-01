#pragma once
#include "Vector.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"
#include "HitData.h"
#include <vector>

class Scene {
public:
	std::vector<Object*> objects;
	std::vector<Primitive*> primitives;
	std::vector<Light*> lights;
	std::vector<Material*> materials;
	Camera *camera;

	HitData Intersect(const Ray &ray);
	Vector ShadowRay(const Ray &shadowRay, float maxDistance);
	void add(Camera *camera);
	void add(Light *light);
	void add(Primitive *primitive);
	void add(Object *object);

	void loadScene(const char *path);

	Scene(int imageWidth, int imageHeight)
	{
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
	}

	int imageWidth, imageHeight;

private:
	void loadCamera(std::ifstream &inputFile);
	void loadPointLight(std::ifstream & inputFile);
	void loadDirectionalLight(std::ifstream & inputFile);
	void loadSphere(std::ifstream & inputFile);
	void loadTriangle(std::ifstream & inputFile, Object *object);
	void loadObject(std::ifstream & inputFile);
	void loadMaterial(std::ifstream & inputFile);
	void loadMaterial(std::ifstream & inputFile, Object *object);
};
