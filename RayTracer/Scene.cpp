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
		else if (type.compare("sphere") == 0)
			loadSphere(inputFile);
		else if (type.compare("poly_set") == 0)
			loadObject(inputFile);
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

void Scene::loadSphere(std::ifstream &inputFile)
{
	Sphere *sphere = new Sphere();
	Object *object = new Object(sphere);
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("name") == 0)
			continue;
		else if (attribute.compare("numMaterials") == 0)
			continue;
		else if (attribute.compare("material") == 0)
			loadMaterial(inputFile, object);
		else if (attribute.compare("origin") == 0)
			is >> sphere->center.x >> sphere->center.y >> sphere->center.z;
		else if (attribute.compare("radius") == 0)
			is >> sphere->radius;
		else if (attribute.compare("xaxis") == 0)
			is >> sphere->xaxis.x >> sphere->xaxis.y >> sphere->xaxis.z;
		else if (attribute.compare("xlength") == 0)
			is >> sphere->xlength;
		else if (attribute.compare("yaxis") == 0)
			is >> sphere->yaxis.x >> sphere->yaxis.y >> sphere->yaxis.z;
		else if (attribute.compare("ylength") == 0)
			is >> sphere->ylength;
		else if (attribute.compare("zaxis") == 0)
			is >> sphere->zaxis.x >> sphere->zaxis.y >> sphere->zaxis.z;
		else if (attribute.compare("zlength") == 0)
			is >> sphere->zlength;
		else if (attribute.compare("}") == 0) {
			primitives.push_back(sphere);
			return;
		}
		else
			std::cout << "Unknown sphere attribute '" << attribute << "'." << std::endl;
	}
}

void Scene::loadMaterial(std::ifstream &inputFile)
{
	Material *material = new Material();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("diffColor") == 0)
			is >> material->diffuse.x >> material->diffuse.y >> material->diffuse.z;
		else if (attribute.compare("ambColor") == 0)
			is >> material->ambient.x >> material->ambient.y >> material->ambient.z;
		else if (attribute.compare("specColor") == 0)
			is >> material->specular.x >> material->specular.y >> material->specular.z;
		else if (attribute.compare("emisColor") == 0)
			is >> material->emissive.x >> material->emissive.y >> material->emissive.z;
		else if (attribute.compare("shininess") == 0)
			is >> material->glossiness;
		else if (attribute.compare("ktran") == 0) {
			float ktran; is >> ktran; material->opacity = 1.0f - ktran;
		}
		else if (attribute.compare("}") == 0) {
			materials.push_back(material);
			return;
		}
		else
			std::cout << "Unknown material attribute '" << attribute << "'." << std::endl;
	}
}


void Scene::loadMaterial(std::ifstream &inputFile, Object *object)
{
	Material *material = new Material();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("diffColor") == 0)
			is >> material->diffuse.x >> material->diffuse.y >> material->diffuse.z;
		else if (attribute.compare("ambColor") == 0)
			is >> material->ambient.x >> material->ambient.y >> material->ambient.z;
		else if (attribute.compare("specColor") == 0)
			is >> material->specular.x >> material->specular.y >> material->specular.z;
		else if (attribute.compare("emisColor") == 0)
			is >> material->emissive.x >> material->emissive.y >> material->emissive.z;
		else if (attribute.compare("shininess") == 0)
			is >> material->glossiness;
		else if (attribute.compare("ktran") == 0) {
			float ktran; is >> ktran; material->opacity = 1.0f - ktran;
		}
		else if (attribute.compare("}") == 0) {
			object->material = material;
			return;
		}
		else
			std::cout << "Unknown material attribute '" << attribute << "'." << std::endl;
	}
}
