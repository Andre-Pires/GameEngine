#include "SceneGraphNode.h"

//standard node - with parent and draw content
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, GeometricObject *object)
{
	this->parent = parent;
	this->object = object;
	this->transformations = MatrixFactory::Identity4();
}

//struture node - no object associated (draws childs only)
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent)
{
	this->parent = parent;
	this->object = nullptr;
	this->transformations = MatrixFactory::Identity4();
}

//root node - no parent or objects
SceneGraphNode::SceneGraphNode()
{
	this->parent = nullptr;
	this->object = nullptr;
	this->transformations = MatrixFactory::Identity4();
}

void SceneGraphNode::add(SceneGraphNode *sceneGraphNode)
{
	(*sceneGraphNode).parent = this;
	this->childNodes.push_back(sceneGraphNode);
}

void SceneGraphNode::draw(Matrix4f parentTransformations)
{
	Matrix4f finalTransformation;

	if (parentTransformations != MatrixFactory::Identity4())
		finalTransformation = parentTransformations * this->transformations;
	else
		finalTransformation = this->transformations;

	if (object != NULL)
		object->draw(finalTransformation);

	int i = 0;
	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		(*it)->draw(finalTransformation);
	}
}

void SceneGraphNode::translate(Vector3f translation)
{
	this->transformations = MatrixFactory::Translation4(translation) * transformations;
}

void SceneGraphNode::scale(Vector3f scale)
{
	this->transformations = MatrixFactory::Scale4(scale) * transformations;
}

void SceneGraphNode::rotate(float angle, Vector3f axis)
{
	this->transformations = MatrixFactory::Rotation4(angle, axis) * transformations;
}

void SceneGraphNode::clearBuffer()
{
	if (object != NULL)
		object->clearObjectFromBuffer();

	int i = 0;
	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		(*it)->clearBuffer();
	}
}

void SceneGraphNode::clearTransformations()
{
	this->transformations = MatrixFactory::Identity4();
}

Matrix4f SceneGraphNode::getTransformationsMatrix()
{
	return this->transformations;
}