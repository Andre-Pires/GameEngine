#include "GameManager.h"
#include "GameEntity.h"
#include "Cube.h"

#include <cassert>

GameManager::GameManager() : _gameNode(nullptr), cube_mesh_(Mesh("Assets/mesh/cube-original.obj")), quad_mesh_(Mesh("Assets/mesh/quad.obj")), sphere_mesh_(Mesh("Assets/mesh/sphere.obj")), cylinder_mesh_(Mesh("Assets/mesh/cylinder.obj"))
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

	_floorObject = new GeometricObject(_bufferObjects, _scene, quad_mesh_);
	_floorObject->scale(Vector3f(0.5f, 0.5f, 0.5f));
	_floorObject->rotate(90, Vector3f(0, 0, 1));
	_floorObject->changeColor(GREY);
	_floorObject->changeShininess(0.05);

	//Floor
	this->normals["marble"] = new Texture(_shader, "Assets/textures/floorNormal.png");

	//Walls
	this->textures["stone"] = new Texture(_shader, "Assets/textures/stone_blocks.jpg");
	this->normals["stone"] = new Texture(_shader, "Assets/textures/stone_blocks_normal.png");

	//For noise calculation
	int size = 32;
	float persistence = 0.5;
	int octaves = 4;

	//NOTE: creating "TEXTURE_NUMBER" different noise textures for wood and marble to introduce variety in scene
	for (int i = 0; i < TEXTURE_NUMBER; i++)
	{
		//Calculate noise
		PerlinNoise* tempNoise = new PerlinNoise(persistence, octaves);
		tempNoise->calculateNoise(size);
		this->textures[std::string("wood" + i)] = new Texture(_shader); // perlin noise texture
		this->textures[std::string("wood" + i)]->setTextureType(2);

		this->textures[std::string("marble" + i)] = new Texture(_shader);
		this->textures[std::string("marble" + i)]->setTextureType(3);

		this->channels[std::string("moss" + i)] = new Texture(_shader);
		this->channels[std::string("moss" + i)]->setTextureType(1);

		for (int j = 0; j < size * size * size; j++) {
			this->textures[std::string("wood" + i)]->setTextureNoise(tempNoise->noise[j], j);
			this->textures[std::string("marble" + i)]->setTextureNoise(tempNoise->noise[j], j);
			this->channels[std::string("moss" + i)]->setTextureNoise(tempNoise->noise[j], j);
		}
		this->textures[std::string("wood" + i)]->prepareTexture(GL_TEXTURE_3D, size, 0);
		this->textures[std::string("marble" + i)]->prepareTexture(GL_TEXTURE_3D, size, 0);
		this->channels[std::string("moss" + i)]->prepareTexture(GL_TEXTURE_3D, size, 0);
	}
	//Boxes
	this->normals[std::string("wood")] = new Texture(_shader, "Assets/textures/boxNormal.png");
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
	int randomTexture = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / TEXTURE_NUMBER));
	auto floorNode = new SceneGraphNode(_gameNode, _floorObject, _scene, this->textures[std::string("marble" + randomTexture)], this->normals[std::string("marble")]);
	floorNode->translate(Vector3f(x + 0.5f, y + 0.5f, 0));
	_gameNode->add(floorNode);
}

GameEntity* GameManager::createStaticWall(float x, float y)
{
	int randomTexture = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / TEXTURE_NUMBER));
	auto object = new Cube(_bufferObjects, _scene);
	object->changeColor(GREY);
	object->changeShininess(0.3f);
	auto node = new SceneGraphNode(_gameNode, object, _scene, this->textures["stone"], this->normals["stone"], this->channels[std::string("moss" + randomTexture)]);
	node->translate(Vector3f(x, y, 0));
	_gameNode->add(node);

	return new GameEntity(node, false);
}

GameEntity* GameManager::createDestructibleWall(float x, float y)
{
	int randomTexture = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / TEXTURE_NUMBER));
	createFloor(x, y);

	auto scale = 0.8f;

	auto object = new GeometricObject(_bufferObjects, _scene, cube_mesh_);
	object->scale(Vector3f(scale));
	object->rotate(90, Vector3f(0.0, 1.0, 0.0));
	object->translate(Vector3f(0, 0, -(1 - scale)));
	object->changeColor(BROWN);
	object->changeShininess(0.7f);
	auto node = new SceneGraphNode(_gameNode, object, _scene, this->textures[std::string("wood" + randomTexture)], this->normals[std::string("wood")]);
	node->translate(Vector3f(x + 0.5f, y + 0.5f, 0.5f));
	_gameNode->add(node);

	return new GameEntity(node, true);
}

GameEntity* GameManager::createPlayer(float x, float y)
{
	createFloor(x, y);

	auto character = new SceneGraphNode(_gameNode, _scene);
	body_ = new SceneGraphNode(_scene);
	
	const auto cube = new Cube(_bufferObjects, _scene);
	cube->changeColor(BLUE);
	cube->changeShininess(0.3f);
	cube->translate(Vector3f(-0.5f, -0.5f, -0.5f));

	{
		head_ = new SceneGraphNode(_gameNode, cube, _scene);
		head_->rotate(5, Vector3f(1, 0, 0));
		auto head_wrapper = new SceneGraphNode(_scene);
		head_wrapper->scale(Vector3f(0.2f, 0.2f, 0.2f));
		head_wrapper->translate(Vector3f(-0.15f, -0.175f, 0.1f));
		head_wrapper->add(head_);
		body_->add(head_wrapper);
	}

	{
		auto object = new Cube(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->changeShininess(0.3f);
		object->scale(Vector3f(0.15f, 0.15f, 0.4f));
		object->rotate(5, Vector3f(1, 0, 0));
		object->translate(Vector3f(-0.225f, -0.225f, -0.5));
		SceneGraphNode * body = new SceneGraphNode(_gameNode, object, _scene);
		body_->add(body);
	}

	{
		auto object = new Cube(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->changeShininess(0.3f);
		object->scale(Vector3f(0.125f, 0.125f, 0.125f));
		object->rotate(10, Vector3f(1, 0, 0));
		object->translate(Vector3f(-0.45f, -0.235f, -0.35));
		_rightHand = new SceneGraphNode(_gameNode, object, _scene);
		body_->add(_rightHand);
	}

	{
		auto object = new Cube(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->changeShininess(0.3f);
		object->scale(Vector3f(0.125f, 0.125f, 0.125f));
		object->rotate(10, Vector3f(1, 0, 0));
		object->translate(Vector3f(0.05f, -0.235f, -0.35));
		_leftHand = new SceneGraphNode(_gameNode, object, _scene);
		body_->add(_leftHand);
	}

	{
		auto object = new Cube(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->changeShininess(0.3f);
		object->scale(Vector3f(0.13f, 0.13f, 0.13f));
		object->translate(Vector3f(-0.35f, -0.235f, -0.8));
		_rightFoot = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_rightFoot);
	}

	{
		auto object = new Cube(_bufferObjects, _scene);
		object->changeColor(BLUE);
		object->changeShininess(0.3f);
		object->scale(Vector3f(0.13f, 0.13f, 0.13f));
		object->translate(Vector3f(-0.05f, -0.235f, -0.8));
		_leftFoot = new SceneGraphNode(_gameNode, object, _scene);
		character->add(_leftFoot);
	}

	character->add(body_);
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
	auto bomb = new SceneGraphNode(_gameNode, _scene);

	{
		auto object = new GeometricObject(_bufferObjects, _scene, sphere_mesh_);
		object->scale(Vector3f(0.25f, 0.25f, 0.25f));
		object->changeColor(BLACK);
		object->changeShininess(0.2f);
		auto node = new SceneGraphNode(_gameNode, object, _scene);
		bomb->add(node);
	}

	{
		auto object = new GeometricObject(_bufferObjects, _scene, cylinder_mesh_);
		object->scale(Vector3f(0.07f, 0.15f, 0.07f));
		object->rotate(90, Vector3f(1.0f, 0.0f, 0.0f));
		object->translate(Vector3f(0, 0, 0.2f));
		object->changeColor(BLACK);
		object->changeShininess(0.2f);
		auto node = new SceneGraphNode(_gameNode, object, _scene);
		bomb->add(node);
	}

	auto position_fix_node = new SceneGraphNode(_scene);
	position_fix_node->add(bomb);
	position_fix_node->translate(Vector3f(x + 0.5f, y + 0.5f, 1.5f));

	auto translation_node_wrapper = new SceneGraphNode(_scene);
	translation_node_wrapper->add(position_fix_node);

	_gameNode->add(translation_node_wrapper);

	return new GameEntity(bomb, false, bomb, translation_node_wrapper);
}