#pragma once
#include "Scene.h"
#include "BufferObjects.h"
#include "Utilities.h"
#include "Matrix4f.h"
#include "Mesh.h"
#include "Texture.h"

class GeometricObject
{
protected:
	BufferObjects * bufferObjects;
	Scene * scene;
	std::vector<Vertex> Vertices;
	Material MaterialColors;
	std::vector <Texcoord> Texcoords;
	std::vector <Normal> Normals;
	std::vector <GLuint> Indices;
	std::vector <Tangent> Tangents;
	int indicesCount;
	int verticesCount;
	Matrix4f transformations;

	//Usados apenas como forma de nomear os diferentes buffers
	GLuint VaoId;
	GLuint *VboId;

	GeometricObject(BufferObjects* buffer, Scene* scene);
public:

	GeometricObject(BufferObjects* buffer, Scene* scene, Mesh mesh);
	GeometricObject(BufferObjects* buffer, Scene* scene, Vertex* Vertices, int verticesSize, GLuint* Indices, int indicesSize);
	void draw(Matrix4f parentNodeTransformations, Texture* texture = nullptr, Texture* normalMap = nullptr);
	void updateBuffer();
	void translate(Vector3f translation);
	void rotate(float angle, Vector3f rotation);
	void scale(Vector3f scale);
	void shear(float shearX, float shearY);
	void changeColor(Color color);
	void clearObjectFromBuffer();
	virtual void shadeColor();
	void repeatTexture(float repeat);
	void calculateTangents();
	ostream& operator<<(std::ostream& stream);
	friend ostream& operator<< (ostream& stream, GeometricObject object);
};
