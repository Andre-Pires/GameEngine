#include "Diamond.h"

Diamond::Diamond(BufferObjects* buffer, Scene* scene) : GameObject(buffer, scene)
{
	//number of vertices and indexes needed to draw a diamond
	this->indicesCount = 6;
	this->verticesCount = 4;

	Vertex tempVertices[] = {
		{ { 0.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.5f, 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	this->Vertices = new Vertex[verticesCount];
	this->Indices = new GLubyte[indicesCount]{ 0,1,2,1,3,2 };

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, tempVertices[i].XYZW, 4 * sizeof(GLfloat));
		memcpy(Vertices[i].RGBA, tempVertices[i].RGBA, 4 * sizeof(GLfloat));
	}

	this->rotate(45, Vector3f(0, 0, 1));

	updateBuffer();
}