#include "Triangle.h"

Triangle::Triangle(BufferObjects* buffer, Scene* scene) : GeometricObject(buffer, scene)
{
	//number of vertices and indexes needed to draw triangle
	this->indicesCount = 24;
	this->verticesCount = 24;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0 - FRONT
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2

		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 4 -  BACK
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5

		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1 - RIGHT
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 2
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1

		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5 - BOTTOM
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 4
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 6
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5

		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4 - LEFT
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 0
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4
	};

	this->Indices = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 };

	for (int i = 0; i < verticesCount; i++)
	{
		this->Vertices.push_back(tempVertices[i]);
	}

	updateBuffer();
}

void Triangle::shadeColor()
{
	float shadeStrength = 0.3;
	int faceStep = 0;
	for (int i = 6; i < verticesCount; i++, faceStep++)
	{
		if (faceStep == 6)
		{
			shadeStrength += 0.15;
			faceStep = 0;
		}
		Vertices[i].RGBA[0] -= shadeStrength;
		Vertices[i].RGBA[1] -= shadeStrength;
		Vertices[i].RGBA[2] -= shadeStrength;
	};

	updateBuffer();
}