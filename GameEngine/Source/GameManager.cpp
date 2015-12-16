#include "GameManager.h"
#include "GameEntity.h"
#include "Square.h"

#include <cassert>


class SceneGraphNode;

GameManager::GameManager(): _gameNode(nullptr)
{
}


GameManager& GameManager::getInstance()
{
	static GameManager instance;

	return instance;
}

GameEntity* GameManager::createEntity(GeometricObject *object)
{
	assert(_gameNode != nullptr);

	auto entityNode = new SceneGraphNode(object);
	auto entity = new GameEntity(entityNode, false);
	_gameNode->add(entityNode);

	return entity;
}

GameEntity* GameManager::createStaticWall(float x, float y)
{
	auto wallTexture = new Texture(_shader, "Assets/textures/stone_wall_texture.jpg");
	auto normalMap = new Texture(_shader, "Assets/textures/stone_wall_texture_normal_map.jpg");

	auto object = new Square(_bufferObjects, _scene);
	object->changeColor(GREY);
	object->repeatTexture(3.0);
	auto node = new SceneGraphNode(_gameNode, object, _scene, wallTexture, normalMap);
	node->translate(Vector3f(x, y, 0));
	_gameNode->add(node);

	return new GameEntity(node, false);
}

GameEntity* GameManager::createDestructibleWall(float x, float y)
{
	auto destructibleTexture = new Texture(_shader, "Assets/textures/text.jpg");
	auto normalMap = new Texture(_shader, "Assets/textures/stone_wall_texture_normal_map.jpg");

	auto object = new Square(_bufferObjects, _scene);
	object->changeColor(BROWN);
	object->repeatTexture(3.0);
	auto node = new SceneGraphNode(_gameNode, object, _scene, destructibleTexture, normalMap);
	node->translate(Vector3f(x, y, 0));
	_gameNode->add(node);

	return new GameEntity(node, true);
}

GameEntity* GameManager::createPlayer(float x, float y)
{
	auto object = new Square(_bufferObjects, _scene);
	object->changeColor(BLUE);
	object->translate(Vector3f(-0.5f, -0.5f, 0));
	object->scale(Vector3f(0.5f, 0.5f, 1));

	auto node = new SceneGraphNode(_gameNode, object, _scene);
	node->rotate(0, Vector3f(0, 0, 1));
	node->translate(Vector3f(x+0.5f, y + 0.5f, 0));
	_gameNode->add(node);

	return new GameEntity(node, false);
}

GameEntity* GameManager::createBomb(float x, float y)
{
	auto object = new Square(_bufferObjects, _scene);
	object->changeColor(BLACK);
	object->translate(Vector3f(0.5f, 0.5f, 0));
	object->scale(Vector3f(0.5f, 0.5f, 1));

	auto node = new SceneGraphNode(_gameNode, object, _scene);
	node->translate(Vector3f(x, y, 0));
	_gameNode->add(node);

	return new GameEntity(node, false);
}
