#pragma once
#include "SceneGraphNode.h"
#include "GameEntity.h"

class GameManager
{
	Scene *_scene;
	SceneGraphNode *_gameNode;
	Shader *_shader;
	BufferObjects *_bufferObjects;

	SceneGraphNode *_rightHand;
	SceneGraphNode *_leftHand;
	SceneGraphNode *_rightFoot;
	SceneGraphNode *_leftFoot;

	GameManager();

public:
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;
	static GameManager& getInstance();

	GameEntity* createEntity(GeometricObject* object);
	

	void setScene(Scene* scene) { _scene = scene; }
	void setGameNode(SceneGraphNode* gameNode) { _gameNode = gameNode; }
	void setShader(Shader* shader) { _shader = shader; }
	void setBufferObjects(BufferObjects* bufferObjects) { _bufferObjects = bufferObjects; }

	GameEntity* createStaticWall(float x, float y);
	GameEntity* createDestructibleWall(float x, float y);
	GameEntity* createPlayer(float x, float y);
	GameEntity* createBomb(float x, float y);

	SceneGraphNode* getRightHand() const { return _rightHand; }
	SceneGraphNode* getLeftHand() const { return _leftHand; }
	SceneGraphNode* getRightFoot() const { return _rightFoot; }
	SceneGraphNode* getLeftFoot() const { return _leftFoot; }
};

