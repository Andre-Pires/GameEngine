#pragma once
#include "Scene.h"
#include "BufferObjects.h"
#include "Utilities.h"
#include "Matrix4f.h"

class GameObject
{
protected:
	BufferObjects * bufferObjects;
	Scene * scene;
	Vertex * Vertices;
	GLubyte *Indices;
	int indicesCount;
	int verticesCount;
	bool hasBeenModified;

	//Usados apenas como forma de nomear os diferentes buffers
	GLuint VaoId;
	GLuint *VboId;
	GameObject(BufferObjects* buffer, Scene* scene);
public:

	GameObject(BufferObjects* buffer, Scene* scene, Vertex* Vertices, int verticesSize, GLubyte* Indices, int indicesSize);
	void draw();
	void updateBuffer();
	void translate(Vector3f translation);
	void rotate(float angle, Vector3f rotation);
	void scale(Vector3f scale);
	void shear(float shearX, float shearY);
	void changeColor(Color color);
	void clearObjectFromBuffer();
	ostream& operator<<(std::ostream& stream);
	friend ostream& operator<< (ostream& stream, GameObject object);
};
