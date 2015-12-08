#include "Plane.h"

Plane::Plane(BufferObjects* buffer, Scene* scene) : GeometricObject(buffer, scene)
{
	//number of vertices and indexes needed to draw a plane
	this->indicesCount = 12;
	this->verticesCount = 8;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 1.0f, 1.0f } }, // 0
		{ { 1.0f, 0.0f, 1.0f, 1.0f } }, // 1
		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2
		{ { 0.0f, 1.0f, 1.0f, 1.0f } }, // 3
	};

	this->Indices = { 0,1,2,2,3,0, 0,3,2,2,1,0 };

	for (int i = 0; i < verticesCount; i++)
	{
		this->Vertices.push_back(tempVertices[i]);
	}

	calculateTangents();

	updateBuffer();
}