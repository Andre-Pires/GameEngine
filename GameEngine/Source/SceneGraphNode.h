#pragma once
#include <stdlib.h>
#include <list>
#include "Utilities.h"
#include "GeometricObject.h"

class SceneGraphNode
{
private:
	GeometricObject* object;
	std::list<SceneGraphNode*> childNodes;
	void drawChildren(Matrix4f parentTransformations);
	SceneGraphNode* parent;
	Matrix4f transformations;
public:
	SceneGraphNode(SceneGraphNode *parent, GeometricObject *object);
	SceneGraphNode(SceneGraphNode *parent);
	SceneGraphNode();
	void add(SceneGraphNode* sceneGraphNode);
	void draw();
	void translate(Vector3f translation);
	void scale(Vector3f scale);
	void rotate(float angle, Vector3f axis);
	void clearBuffer();
};
