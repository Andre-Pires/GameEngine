#include "SceneGraphNode.h"

// standard node - with parent and draw content
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

void SceneGraphNode::draw()
{
	if (object != NULL)
		object->draw(this->transformations);

	int i = 0;
	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		(*it)->drawChildren(this->transformations);
	}
}

void SceneGraphNode::drawChildren(Matrix4f parentTransformations)
{
	if (object != NULL)
		object->draw(parentTransformations * this->transformations);

	int i = 0;
	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		(*it)->drawChildren(parentTransformations * this->transformations);
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