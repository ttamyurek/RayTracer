#include "Scene.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

HitData Scene::Intersect(const Ray &ray)
{
	HitData closestHitData;
	HitData hitData;
	float closest = infinity;
	for (auto primitive : primitives)
	{
		if (primitive->Intersect(ray, hitData) && closest > hitData.t)
		{
			closest = hitData.t;
			closestHitData = hitData;
		}
	}
	return closestHitData;
}

Vector Scene::ShadowRay(const Ray &shadowRay, float maxDistance) {
	Vector shadowFactor(1.0f);
	HitData hitData;
	for (auto primitive : primitives)
	{
		if (primitive->Intersect(shadowRay, hitData))
		{
			if (hitData.t > maxDistance || hitData.material->opacity > 0.98) return Vector(0.0f);
			Vector diffuse = hitData.material->diffuse;
			shadowFactor *= ((diffuse / diffuse.Max()) * (1.0 - hitData.material->opacity));
		}
	}
	return shadowFactor;
}

void Scene::add(Camera * camera)
{
	this->camera = camera;
}

void Scene::add(Light * light)
{
	lights.push_back(light);
}

void Scene::add(Primitive * primitive)
{
	primitives.push_back(primitive);
}

void Scene::add(Object * object)
{
	objects.push_back(object);
}

void Scene::loadScene(const char *path)
{
	std::ifstream inputFile(path);
	std::cout << "Parsing scene information from " << path << std::endl;

	std::string line, type;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> type;
		if (type.compare("Composer") == 0)
			continue;
		else if (type.compare("camera") == 0)
			loadCamera(inputFile);
		else if (type.compare("point_light") == 0)
			loadPointLight(inputFile);
		else if (type.compare("directional_light") == 0)
			loadDirectionalLight(inputFile);
	}
}

void Scene::loadCamera(std::ifstream &inputFile)
{
	Camera *camera = new Camera();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("position") == 0)
			is >> camera->position.x >> camera->position.y >> camera->position.z;
		else if (attribute.compare("viewDirection") == 0)
			is >> camera->viewDir.x >> camera->viewDir.y >> camera->viewDir.z;
		else if (attribute.compare("orthoUp") == 0)
			is >> camera->upDir.x >> camera->upDir.y >> camera->upDir.z;
		else if (attribute.compare("focalDistance") == 0)
			is >> camera->focalDistance;
		else if (attribute.compare("verticalFOV") == 0)
			is >> camera->verticalFOV;
		else if (attribute.compare("}") == 0) {
			this->camera = camera;
			return;
		}
		else
			std::cout << "Unknown camera attribute '" << attribute << "'." << std::endl;
	}
}

void Scene::loadPointLight(std::ifstream &inputFile)
{
	PointLight *light = new PointLight();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("position") == 0)
			is >> light->position.x >> light->position.y >> light->position.z;
		else if (attribute.compare("color") == 0)
			is >> light->color.x >> light->color.y >> light->color.z;
		else if (attribute.compare("}") == 0) {
			lights.push_back(light);
			return;
		}
		else
			std::cout << "Unknown point light attribute '" << attribute << "'." << std::endl;
	}
}

void Scene::loadDirectionalLight(std::ifstream &inputFile)
{
	DirectionalLight *light = new DirectionalLight();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("position") == 0)
			is >> light->position.x >> light->position.y >> light->position.z;
		else if (attribute.compare("direction") == 0)
			is >> light->direction.x >> light->direction.y >> light->direction.z;
		else if (attribute.compare("color") == 0)
			is >> light->color.x >> light->color.y >> light->color.z;
		else if (attribute.compare("}") == 0) {
			lights.push_back(light);
			return;
		}
		else
			std::cout << "Unknown directional light attribute '" << attribute << "'." << std::endl;
	}
}