﻿#include "Cube.h"

Cube::Cube(BufferObjects* buffer, Scene* scene) : GeometricObject(buffer, scene)
{
	//number of vertices and indexes needed to draw square
	this->indicesCount = 36;
	this->verticesCount = 36;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 1.0f, 1.0f } }, // 0 - FRONT
		{ { 1.0f, 0.0f, 1.0f, 1.0f } }, // 1
		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2
		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2
		{ { 0.0f, 1.0f, 1.0f, 1.0f } }, // 3
		{ { 0.0f, 0.0f, 1.0f, 1.0f } }, // 0

		{ { 1.0f, 0.0f, 0.0f, 1.0f } }, // 5 - BACK
		{ { 0.0f, 0.0f, 0.0f, 1.0f } }, // 4
		{ { 0.0f, 1.0f, 0.0f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 0.0f, 1.0f } }, // 7
		{ { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 0.0f, 0.0f, 1.0f } }, // 5

		{ { 1.0f, 0.0f, 1.0f, 1.0f } }, // 1 - RIGHT
		{ { 1.0f, 0.0f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2
		{ { 1.0f, 0.0f, 1.0f, 1.0f } }, // 1

		{ { 0.0f, 0.0f, 0.0f, 1.0f } }, // 4 - LEFT
		{ { 0.0f, 0.0f, 1.0f, 1.0f } }, // 0
		{ { 0.0f, 1.0f, 1.0f, 1.0f } }, // 3
		{ { 0.0f, 1.0f, 1.0f, 1.0f } }, // 3
		{ { 0.0f, 1.0f, 0.0f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 0.0f, 1.0f } }, // 4

		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2 - TOP
		{ { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
		{ { 0.0f, 1.0f, 0.0f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 0.0f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 1.0f, 1.0f } }, // 3
		{ { 1.0f, 1.0f, 1.0f, 1.0f } }, // 2

		{ { 0.0f, 0.0f, 1.0f, 1.0f }}, // 0 - BOTTOM
		{ { 0.0f, 0.0f, 0.0f, 1.0f }}, // 4
		{ { 1.0f, 0.0f, 0.0f, 1.0f }}, // 5
		{ { 1.0f, 0.0f, 0.0f, 1.0f }}, // 5
		{ { 1.0f, 0.0f, 1.0f, 1.0f }}, // 1
		{ { 0.0f, 0.0f, 1.0f, 1.0f }}  // 0
	};

	//in this case the vertices were replicated, but I'm keeping the indexes in case I want to reuse some vertices
	this->Indices = {
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 };

	for (int i = 0; i < verticesCount; i++)
	{
		this->Vertices.push_back(tempVertices[i]);
	}

	Normal tempNormals[] = {
		 { 0.0f, 0.0f, 1.0f}, // 0 - FRONT
		 { 0.0f, 0.0f, 1.0f}, // 1
		 { 0.0f, 0.0f, 1.0f}, // 2
		 { 0.0f, 0.0f, 1.0f}, // 2
		 { 0.0f, 0.0f, 1.0f}, // 3
		 { 0.0f, 0.0f, 1.0f}, // 0

		 { 0.0f, 0.0f, -1.0f}, // 5 - BACK
		 { 0.0f, 0.0f, -1.0f}, // 4
		 { 0.0f, 0.0f, -1.0f}, // 7
		 { 0.0f, 0.0f, -1.0f}, // 7
		 { 0.0f, 0.0f, -1.0f}, // 6
		 { 0.0f, 0.0f, -1.0f}, // 5

		 { 1.0f, 0.0f, 0.0f}, // 1 - RIGHT
		 { 1.0f, 0.0f, 0.0f}, // 5
		 { 1.0f, 0.0f, 0.0f}, // 6
		 { 1.0f, 0.0f, 0.0f}, // 6
		 { 1.0f, 0.0f, 0.0f}, // 2
		 { 1.0f, 0.0f, 0.0f}, // 1

		 { -1.0f, 0.0f, 0.0f}, // 4 - LEFT
		 { -1.0f, 0.0f, 0.0f}, // 0
		 { -1.0f, 0.0f, 0.0f}, // 3
		 { -1.0f, 0.0f, 0.0f}, // 3
		 { -1.0f, 0.0f, 0.0f}, // 7
		 { -1.0f, 0.0f, 0.0f}, // 4

		 { 0.0f, 1.0f, 0.0f}, // 2 - TOP
		 { 0.0f, 1.0f, 0.0f}, // 6
		 { 0.0f, 1.0f, 0.0f}, // 7
		 { 0.0f, 1.0f, 0.0f}, // 7
		 { 0.0f, 1.0f, 0.0f}, // 3
		 { 0.0f, 1.0f, 0.0f}, // 2

		 { 0.0f, -1.0f, 0.0f}, // 0 - BOTTOM
		 { 0.0f, -1.0f, 0.0f}, // 4
		 { 0.0f, -1.0f, 0.0f}, // 5
		 { 0.0f, -1.0f, 0.0f}, // 5
		 { 0.0f, -1.0f, 0.0f}, // 1
		 { 0.0f, -1.0f, 0.0f}  // 0
	};

	for (int i = 0; i < verticesCount; i++)
	{
		this->Normals.push_back(tempNormals[i]);
	}

	Texcoord tempTexture[] = {
		{ 0.0f, 1.0f }, // 0 - FRONT
		{ 0.0f, 0.0f }, // 1
		{ 1.0f, 0.0f }, // 2
		{ 1.0f, 0.0f }, // 2
		{ 1.0f, 1.0f }, // 3
		{ 0.0f, 1.0f }, // 0

		{ 0.0f, 1.0f }, // 5 - BACK
		{ 0.0f, 0.0f }, // 4
		{ 1.0f, 0.0f }, // 7
		{ 1.0f, 0.0f }, // 7
		{ 1.0f, 1.0f }, // 6
		{ 0.0f, 1.0f }, // 5

		{ 0.0f, 1.0f }, // 1 - RIGHT
		{ 0.0f, 0.0f }, // 5
		{ 1.0f, 0.0f }, // 6
		{ 1.0f, 0.0f }, // 6
		{ 1.0f, 1.0f }, // 2
		{ 0.0f, 1.0f }, // 1

		{ 0.0f, 1.0f }, // 4 - LEFT
		{ 0.0f, 0.0f }, // 0
		{ 1.0f, 0.0f }, // 3
		{ 1.0f, 0.0f }, // 3
		{ 1.0f, 1.0f }, // 7
		{ 0.0f, 1.0f }, // 4

		{ 1.0f, 0.0f }, // 2 - TOP
		{ 1.0f, 1.0f }, // 6
		{ 0.0f, 1.0f }, // 7
		{ 0.0f, 1.0f }, // 7
		{ 0.0f, 0.0f }, // 3
		{ 1.0f, 0.0f }, // 2

		{ 0.0f, 1.0f }, // 0 - BOTTOM
		{ 0.0f, 0.0f }, // 4
		{ 1.0f, 0.0f }, // 5
		{ 1.0f, 0.0f }, // 5
		{ 1.0f, 1.0f }, // 1
		{ 0.0f, 1.0f }  // 0
	};

	for (int i = 0; i < verticesCount; i++)
	{
		this->Texcoords.push_back(tempTexture[i]);
	}

	calculateTangents();

	updateBuffer();
}

void Cube::shadeColor()
{
	float shadeStrength = 0.3;

	MaterialColors.AMBIENT[0] -= shadeStrength;
	MaterialColors.AMBIENT[1] -= shadeStrength;
	MaterialColors.AMBIENT[2] -= shadeStrength;

	MaterialColors.DIFFUSE[0] -= shadeStrength;
	MaterialColors.DIFFUSE[1] -= shadeStrength;
	MaterialColors.DIFFUSE[2] -= shadeStrength;

	MaterialColors.SPECULAR[0] -= shadeStrength;
	MaterialColors.SPECULAR[1] -= shadeStrength;
	MaterialColors.SPECULAR[2] -= shadeStrength;

	updateBuffer();
}