#pragma once
#include "SceneGraphNode.h"
#include "GameEntity.h"
#define TEXTURE_NUMBER 3

class GameManager
{
	Scene *_scene;
	SceneGraphNode *_gameNode;
	Shader *_shader;
	BufferObjects *_bufferObjects;
	std::map<std::string, Texture *> textures;
	std::map<std::string, Texture *> normals;

	SceneGraphNode* head_;
	SceneGraphNode *_rightHand;
	SceneGraphNode *_leftHand;
	SceneGraphNode *_rightFoot;
	SceneGraphNode *_leftFoot;

	GeometricObject *_floorObject;

	const Mesh cube_mesh_;
	const Mesh quad_mesh_;
	const Mesh sphere_mesh_;
	const Mesh cylinder_mesh_;

	GameManager();

public:
	GameManager(GameManager const&) = delete;
	void operator=(GameManager const&) = delete;

	static GameManager& getInstance();

	void init(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);

	GameEntity* createEntity(GeometricObject* object);

	void createEmpty(float x, float y);
	void createFloor(float x, float y);
	GameEntity* createStaticWall(float x, float y);
	GameEntity* createDestructibleWall(float x, float y);
	GameEntity* createPlayer(float x, float y);
	GameEntity* createBomb(float x, float y);

	SceneGraphNode* getHead() const { return head_; }
	SceneGraphNode* getRightHand() const { return _rightHand; }
	SceneGraphNode* getLeftHand() const { return _leftHand; }
	SceneGraphNode* getRightFoot() const { return _rightFoot; }
	SceneGraphNode* getLeftFoot() const { return _leftFoot; }
};
