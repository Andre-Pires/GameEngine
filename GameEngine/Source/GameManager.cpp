#include "GameManager.h"
#include "GameEntity.h"
#include "Square.h"

#include <cassert>


GameManager::GameManager(): _gameNode(nullptr)
{
}


GameManager& GameManager::getInstance()
{
	static GameManager instance;

	return instance;
}


void GameManager::init(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader)
{
	_scene = scene;
	_gameNode = gameNode;
	_bufferObjects = bufferObjects;
	_shader = shader;

	Mesh quadMesh = Mesh(std::string("Assets/mesh/quad.obj"));
	_floorObject = new GeometricObject(_bufferObjects, _scene, quadMesh);
	_floorObject->scale(Vector3f(0.5f, 0.5f, 0.5f));
	_floorObject->rotate(90, Vector3f(0, 0, 1));
	_floorObject->changeColor(GREEN);

	/*auto floorNode = new SceneGraphNode(_gameNode, _floorObject, _scene);
	floorNode->translate(Vector3f(0.5f, -0.5f, 0));
	floorNode->scale(Vector3f(9, 7, 0));
	floorNode->translate(Vector3f(1, 0, 0));
	_gameNode->add(floorNode);*/
}

GameEntity* GameManager::createEntity(GeometricObject *object)
{
	assert(_gameNode != nullptr);

	auto entityNode = new SceneGraphNode(object);
	auto entity = new GameEntity(entityNode, false);
	_gameNode->add(entityNode);

	return entity;
}

void GameManager::createEmpty(float x, float y)
{
	createFloor(x, y);
}

void GameManager::createFloor(float x, float y)
{
	auto floorNode = new SceneGraphNode(_gameNode, _floorObject, _scene);
	floorNode->translate(Vector3f(x + 0.5f, y + 0.5f, 0));
	_gameNode->add(floorNode);
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
	createFloor(x, y);

	auto destructibleTexture = new Texture(_shader);
	auto normalMap = new Texture(_shader, "Assets/textures/boxNormal.png");

	auto object = new Square(_bufferObjects, _scene);
	object->changeColor(BROWN);
	//object->repeatTexture(3.0);
	auto node = new SceneGraphNode(_gameNode, object, _scene, destructibleTexture, normalMap);
	node->translate(Vector3f(x, y, 0));
	_gameNode->add(node);

	return new GameEntity(node, true);
}

GameEntity* GameManager::createPlayer(float x, float y)
{
	createFloor(x, y);
	
	SceneGraphNode * character = new SceneGraphNode(_gameNode, _scene);

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.2f, 0.2f, 0.2f));
		object->rotate(5, Vector3f(1, 0, 0));
		object->translate(Vector3f(-0.25f, -0.275f, 0));
		SceneGraphNode * head = new SceneGraphNode(_gameNode, object, _scene);
		character->add(head);
	}

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.15f, 0.15f, 0.4f));
		object->rotate(5, Vector3f(1, 0, 0));
		object->translate(Vector3f(-0.225f, -0.225f, -0.5));
		SceneGraphNode * body = new SceneGraphNode(_gameNode, object, _scene);
		character->add(body);
	}

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.125f, 0.125f, 0.125f));
		object->rotate(10, Vector3f(1, 0, 0));
		object->translate(Vector3f(-0.45f, -0.235f, -0.35));
		_rightHand = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_rightHand);
	}

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.125f, 0.125f, 0.125f));
		object->rotate(10, Vector3f(1, 0, 0));
		object->translate(Vector3f(0.05f, -0.235f, -0.35));
		_leftHand = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_leftHand);
	}

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.13f, 0.13f, 0.13f));
		object->translate(Vector3f(-0.35f, -0.235f, -0.8));
		_rightFoot = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_rightFoot);
	}

	{
		auto object = new Square(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->scale(Vector3f(0.13f, 0.13f, 0.13f));
		object->translate(Vector3f(-0.05f, -0.235f, -0.8));
		_leftFoot = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_leftFoot);
	}

	// Fix anchor point and rotation
	character->translate(Vector3f(0.1f, 0.175f, 0.8));
	character->rotate(90, Vector3f(0, 0, 1));

	SceneGraphNode *characterWrapper = new SceneGraphNode(_scene);
	characterWrapper->translate(Vector3f(x + 0.5f, y + 0.5f, 0));

	characterWrapper->add(character);
	_gameNode->add(characterWrapper);

	return new GameEntity(characterWrapper, false);
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
