#include "Square.h"

Square::Square(BufferObjects* buffer, Scene* scene) : GameObject(buffer, scene)
{
	//number of vertices and indexes needed to draw square
	this->indicesCount = 36;
	this->verticesCount = 36;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0 - FRONT
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 1
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 2
		{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 3
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.0f, 1.0f } }, // 0

		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5 - BACK
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 4
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 7
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 6
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.9f, 1.0f } }, // 5

		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1 - RIGHT
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 6
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 2
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.9f, 0.0f, 1.0f } }, // 1

		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4 - LEFT
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 0
		{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
		{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 3
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.0f, 0.9f, 1.0f } }, // 4

		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 2 - TOP
		{ { 1.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 6
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 7
		{ { 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 3
		{ { 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 0.9f, 1.0f } }, // 2

		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 0 - BOTTOM
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 4
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 0.0f, 0.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 5
		{ { 1.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }, // 1
		{ { 0.0f, 0.0f, 1.0f, 1.0f },{ 0.9f, 0.9f, 0.0f, 1.0f } }  // 0
	};

	this->Vertices = new Vertex[verticesCount];
	//in this case the vertices were replicated, but I'm keeping the indexes in case I want to reuse some vertices
	this->Indices = new GLubyte[indicesCount]{
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35 };

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, tempVertices[i].XYZW, 4 * sizeof(GLfloat));
		memcpy(Vertices[i].RGBA, tempVertices[i].RGBA, 4 * sizeof(GLfloat));
	}

	updateBuffer();
}

//TODO - used only for this lab and makes no sense, should be removed afterwards
void Square::shadeColor()
{
	float shadeStrength = 0.3;
	int faceStep = 0;
	for (int i = 12; i < verticesCount; i++, faceStep++)
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