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
			if (hitData.t < maxDistance)
			{
				if (hitData.material->opacity > 0.98) return Vector(0.0f);
				Vector diffuse = hitData.material->diffuse;
				shadowFactor *= ((diffuse / diffuse.Max()) * (1.0 - hitData.material->opacity));
			}
			
		}
	}
	return shadowFactor;
}

float Scene::SampleShadow(Vector position, int SPP)
{
	Vector shadow = 0.0;
	Vector color = 0.0;
	for (auto &light : lights)
	{
		for (int i = 0; i < SPP; i++)
		{
			Ray ray = Ray(position, SampleRandomDirection());
			if (light->intersect(ray))
				shadow += ShadowRay(ray, light->distance(position));
		}
		shadow /= SPP;
		color += shadow;
	}
	return (color.x + color.y + color.z ) / 3.0;
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
		else if (type.compare("area_light") == 0)
			loadAreaLight(inputFile);
		else if (type.compare("sphere") == 0)
			loadSphere(inputFile);
		else if (type.compare("poly_set") == 0)
			loadObject(inputFile);
		else
			std::cout << "Unknown type '" << type << "'." << std::endl;
	}
	std::cout << "Scene loaded successful." << std::endl;
	return;
}

void Scene::loadCamera(std::ifstream &inputFile)
{
	Vector position, viewDir, upDir;
	float focalDistance, verticalFOV;

	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("position") == 0)
			is >> position.x >> position.y >> position.z;
		else if (attribute.compare("viewDirection") == 0)
			is >> viewDir.x >> viewDir.y >> viewDir.z;
		else if (attribute.compare("orthoUp") == 0)
			is >> upDir.x >> upDir.y >> upDir.z;
		else if (attribute.compare("focalDistance") == 0)
			is >> focalDistance;
		else if (attribute.compare("verticalFOV") == 0)
			is >> verticalFOV;
		else if (attribute.compare("}") == 0) {
			Camera *camera = new Camera(position, viewDir, upDir, focalDistance, verticalFOV, imageWidth, imageHeight);
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
		if (attribute.compare("direction") == 0)
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

void Scene::loadAreaLight(std::ifstream &inputFile)
{
	AreaLight light;
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("position") == 0)
			is >> light.position.x >> light.position.y >> light.position.z;
		else if (attribute.compare("direction") == 0)
			is >> light.direction.x >> light.direction.y >> light.direction.z;
		else if (attribute.compare("size") == 0)
			is >> light.width >> light.height;
		else if (attribute.compare("color") == 0)
			is >> light.color.x >> light.color.y >> light.color.z;
		else if (attribute.compare("}") == 0) {
			AreaLight *areaLight = new AreaLight(light.position, light.direction, light.width, light.height);
			lights.push_back(areaLight);
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
			objects.push_back(object);
			return;
		}
		else
			std::cout << "Unknown sphere attribute '" << attribute << "'." << std::endl;
	}
}

void Scene::loadObject(std::ifstream &inputFile)
{
	Object *object = new Object();
	std::string line, attribute;
	while (std::getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> attribute;
		if (attribute.compare("poly") == 0)
			loadTriangle(inputFile, object);
		else if (attribute.compare("name") == 0)
			continue;
		else if (attribute.compare("numMaterials") == 0)
			continue;
		else if (attribute.compare("material") == 0)
			loadMaterial(inputFile, object);
		else if (attribute.compare("type") == 0)
			continue;
		else if (attribute.compare("normType") == 0)
			continue;
		else if (attribute.compare("materialBinding") == 0)
			continue;
		else if (attribute.compare("hasTextureCoords") == 0)
			continue;
		else if (attribute.compare("rowSize") == 0)
			continue;
		else if (attribute.compare("numPolys") == 0)
			continue;
		else if (attribute.compare("}") == 0) {
			objects.push_back(object);
			return;
		}
		else
			std::cout << "Unknown object attribute '" << attribute << "'." << std::endl;
	}
}

void Scene::loadTriangle(std::ifstream &inputFile, Object *object)
{
	Triangle *triangle = new Triangle();
	std::string line, attribute;
	int numVertices;
	while (std::getline(inputFile, line))
	{
		std::stringstream is(line);
		is >> attribute;
		if (attribute.compare("numVertices") == 0)
			is >> numVertices;
		else if (attribute.compare("pos") == 0)
		{
			is >> triangle->v0.x >> triangle->v0.y >> triangle->v0.z;
			std::getline(inputFile, line);
			is = std::stringstream(line);
			is >> attribute >> triangle->v1.x >> triangle->v1.y >> triangle->v1.z;
			std::getline(inputFile, line);
			is = std::stringstream(line);
			is >> attribute >> triangle->v2.x >> triangle->v2.y >> triangle->v2.z;
			triangle->calculateNormal();
		}
		else if (attribute.compare("}") == 0) {
			primitives.push_back(triangle);
			object->add(triangle);
			return;
		}
		else
			std::cout << "Unknown triangle attribute '" << attribute << "'." << std::endl;
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
			float ktran; is >> ktran; material->opacity = 1.0f - ktran; // TODO: Fix
		}
		else if (attribute.compare("}") == 0) {
			object->material = material;
			materials.push_back(material);
			return;
		}
		else
			std::cout << "Unknown material attribute '" << attribute << "'." << std::endl;
	}
}

bool Scene::loadOBJ(const char * path)
{
	Object *object = new Object();
	Material *material = new Material();
	object->material = material;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< Vector > temp_vertices;
	std::vector< UV > temp_uvs;
	std::vector< Vector > temp_normals;

	std::ifstream inputFile(path);
	std::cout << "Parsing Vertex Data from " << path << std::endl;
	std::string line, lineHeader;
	while (getline(inputFile, line))
	{
		std::istringstream is(line);
		is >> lineHeader;
		if (line[0] == '#') continue; //Skip Comments
		if (lineHeader == "v") // Parse Vertices
		{
			Vector vertex;
			is >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") // Parse Texture Coordinates
		{
			UV uv;
			is >> uv.u >> uv.v;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn") // Parse Vertex Normals
		{
			Vector normal;
			is >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f") // Parse Face Data
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			std::string index;
			for (int i = 0; i < 3; i++)
			{
				getline(is, index, '/');
				vertexIndex[i] = stoi(index);
				getline(is, index, '/');
				uvIndex[i] = stoi(index);
				getline(is, index, ' ');
				normalIndex[i] = stoi(index);
			}
			Triangle *triangle = new Triangle();
			triangle->v0 = temp_vertices[vertexIndex[0] - 1];
			triangle->v1 = temp_vertices[vertexIndex[1] - 1];
			triangle->v2 = temp_vertices[vertexIndex[2] - 1];
			triangle->uv0 = temp_uvs[uvIndex[0] - 1];
			triangle->uv1 = temp_uvs[uvIndex[1] - 1];
			triangle->uv2 = temp_uvs[uvIndex[2] - 1];
			triangle->n0 = temp_normals[normalIndex[0] - 1];
			triangle->n1 = temp_normals[normalIndex[1] - 1];
			triangle->n2 = temp_normals[normalIndex[2] - 1];

			triangle->parent = object;
			object->add(triangle);
			primitives.push_back(triangle);
			objects.push_back(object);
			
			/*vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);*/

		}
		else continue;
	}
	return true;
	/*
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		Vector vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		UV uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		Vector normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return 1;
	*/
}
