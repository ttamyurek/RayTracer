#pragma once
#include "Vector.h"
#include "Primitive.h"
#include "Material.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

class Object
{
public:
	std::vector<Primitive*> primitives;
	Vector position;
	Vector orientation;
	Vector scale;

	Material *material;

	Object()
	{
		position = Vector(0.0);
		orientation = Vector(0.0, 0.0, 1.0);
		scale = Vector(1.0);
	}
	Object(Primitive *primitive)
	{
		position = Vector(0.0);
		orientation = Vector(0.0, 0.0, 1.0);
		scale = Vector(1.0);
		primitives.push_back(primitive);
		primitive->parent = this;
	}
	void add(Primitive *primitive)
	{
		primitives.push_back(primitive);
		primitive->parent = this;
	}
	void rotate(float angle, Vector axis)
	{
		for (auto &primitive : primitives)
			primitive->rotate(angle, axis);
	}
};


static Object* loadOBJ(const char * path)
{
	Object *object = new Object;
	object->material = new Material;

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
			Triangle *triangle = new Triangle;
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
	return object;
}
