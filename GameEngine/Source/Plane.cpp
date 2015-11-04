#include "Plane.h"

Plane::Plane(BufferObjects* buffer, Scene* scene) : GeometricObject(buffer, scene)
{
	//number of vertices and indexes needed to draw a plane
	this->indicesCount = 12;
	this->verticesCount = 8;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 1
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 2
		{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 3
	};

	this->Vertices = new Vertex[verticesCount];
	this->Indices = new GLubyte[indicesCount]{ 0,1,2,2,3,0,
											   0,3,2,2,1,0 };

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, tempVertices[i].XYZW, 4 * sizeof(GLfloat));
		memcpy(Vertices[i].RGBA, tempVertices[i].RGBA, 4 * sizeof(GLfloat));
	}

	updateBuffer();
}