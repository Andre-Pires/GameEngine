#include "SceneGraphNode.h"

//standard node - with parent, draw content and texture
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, GeometricObject *object, Scene * scene, Texture * texture, Texture * normalMap)
{
	this->parent = parent;
	this->object = object;
	this->transformations = MatrixFactory::Identity4();
	this->scene = scene;
	this->shader = nullptr;
	this->texture = texture;
	this->normalMap = normalMap;
}

//standard node - with parent and draw content
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, GeometricObject *object, Scene * scene)
{
	this->parent = parent;
	this->object = object;
	this->transformations = MatrixFactory::Identity4();
	this->scene = scene;
	this->shader = nullptr;
	this->texture = parent->texture;
	this->normalMap = parent->normalMap;
}

//struture node - no object associated (draws childs only)
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, Scene * scene)
{
	this->parent = parent;
	this->object = nullptr;
	this->transformations = MatrixFactory::Identity4();
	this->scene = scene;
	this->shader = nullptr;
	this->texture = nullptr;
	this->normalMap = nullptr;
}

//struture node - no object associated (draws childs only) with texture associated
SceneGraphNode::SceneGraphNode(SceneGraphNode *parent, Scene * scene, Texture * texture, Texture * normalMap)
{
	this->parent = parent;
	this->object = nullptr;
	this->transformations = MatrixFactory::Identity4();
	this->scene = scene;
	this->shader = nullptr;
	this->texture = texture;
	this->normalMap = normalMap;
}

//root node - no parent or objects
SceneGraphNode::SceneGraphNode(Scene * scene)
{
	this->parent = nullptr;
	this->object = nullptr;
	this->transformations = MatrixFactory::Identity4();
	this->scene = scene;
	this->shader = nullptr;
	this->texture = nullptr;
	this->normalMap = nullptr;
}

void SceneGraphNode::add(SceneGraphNode *sceneGraphNode)
{
	(*sceneGraphNode).parent = this;
	this->childNodes.push_back(sceneGraphNode);
}

void SceneGraphNode::draw(Matrix4f parentTransformations)
{
	Matrix4f finalTransformation;

	//if node has no shader, uses active shader in scene
	if (shader != nullptr)
		scene->setActiveShader(shader);

	//uses identity matrix in case there's no parent tranformation (first case only)
	finalTransformation = parentTransformations * this->transformations;

	if (object != NULL)
		object->draw(finalTransformation, texture, normalMap);

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

	for (auto it = childNodes.begin(); it != childNodes.end(); ++it)
	{
		(*it)->clearBuffer();
	}
}

void SceneGraphNode::clearTransformations()
{
	this->transformations = MatrixFactory::Identity4();
}

void SceneGraphNode::setShader(Shader* shader)
{
	this->shader = shader;
}

void SceneGraphNode::setTexture(Texture* texture, Texture * normalMap)
{
	this->texture = texture;
	this->normalMap = normalMap;
}

Matrix4f SceneGraphNode::getTransformationsMatrix()
{
	return this->transformations;
}