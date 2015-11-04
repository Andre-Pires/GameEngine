#pragma once
#include "Scene.h"
#include "BufferObjects.h"
#include "Utilities.h"
#include "Matrix4f.h"

class GeometricObject
{
protected:
	BufferObjects * bufferObjects;
	Scene * scene;
	Vertex * Vertices;
	GLubyte *Indices;
	int indicesCount;
	int verticesCount;
	Matrix4f transformations;

	//Usados apenas como forma de nomear os diferentes buffers
	GLuint VaoId;
	GLuint *VboId;
	GeometricObject(BufferObjects* buffer, Scene* scene);
public:

	GeometricObject(BufferObjects* buffer, Scene* scene, Vertex* Vertices, int verticesSize, GLubyte* Indices, int indicesSize);
	void draw(Matrix4f parentNodeTransformations);
	void updateBuffer();
	void translate(Vector3f translation);
	void rotate(float angle, Vector3f rotation);
	void scale(Vector3f scale);
	void shear(float shearX, float shearY);
	void changeColor(Color color);
	void clearObjectFromBuffer();
	virtual void shadeColor();
	ostream& operator<<(std::ostream& stream);
	friend ostream& operator<< (ostream& stream, GeometricObject object);
};
