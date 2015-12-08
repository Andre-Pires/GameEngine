#pragma once
#include <stdlib.h>
#include <list>
#include "Utilities.h"
#include "GeometricObject.h"
#include "Texture.h"

class SceneGraphNode
{
private:
	GeometricObject* object;
	std::list<SceneGraphNode*> childNodes;
	SceneGraphNode* parent;
	Matrix4f transformations;
	Shader * shader;
	Scene * scene;
	Texture * texture;
	Texture * normalMap;

public:
	SceneGraphNode(SceneGraphNode *parent, GeometricObject *object, Scene * scene, Texture * texture, Texture * normalMap);
	SceneGraphNode(SceneGraphNode* parent, GeometricObject* object, Scene* scene);
	SceneGraphNode(SceneGraphNode *parent, Scene * scene);
	SceneGraphNode(SceneGraphNode *parent, Scene * scene, Texture * tex, Texture * normalMap);
	SceneGraphNode(Scene * scene);
	void add(SceneGraphNode* sceneGraphNode);
	void draw(Matrix4f parentTransformations = MatrixFactory::Identity4());
	void translate(Vector3f translation);
	void scale(Vector3f scale);
	void rotate(float angle, Vector3f axis);
	void clearBuffer();
	void clearTransformations();
	void setShader(Shader* shader);
	void setTexture(Texture* texture, Texture * normalMap);
	Matrix4f getTransformationsMatrix();
};
