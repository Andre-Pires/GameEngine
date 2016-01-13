#pragma once
#include <vector>
#include "SceneGraphNode.h"
#include <string>
#include <fstream>
#include <cassert>
#include "Square.h"
#include "Diamond.h"
#include "Triangle.h"
#include "GridMap.h"
#include "Bomb.h"

typedef void(*CallbackType)();

class Bomberman
{
private:
	const float _moveStep = 1;
	const unsigned WALK_ANIMATION_DURATION = 500;
	const unsigned ROTATE_ANIMATION_DURATION = 400;
	const unsigned BOMB_EXPLOSION_TIME = 3000;
	const unsigned BOMB_FALL_TIME = 500;


	GridMap *_gridMap;
	std::vector<Bomb*> _bombs;
	Vector2f _playerPosition;
	float _playerOrientation;
	GameEntity *_playerEntity;

	unsigned total_walk_time_;
	unsigned total_rotation_time_;
	int _rotationDirection;
	bool _playerActive;
	int _startingFoot;
	CallbackType _activateFlash;
	Bomb* bomb_on_player_;

	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void explode(unsigned row, unsigned col);
	void animationsUpdate(unsigned elapsedTime);
	void playerUpdate(unsigned elapsed_time);
	void wavePlayerMembers(float harmonicPercentage);
	void animateBomb(SceneGraphNode* node, float percentage);

	void dropBomb();

	void initWalk();
	bool isClearAhead() const;
public:
	Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash);
	~Bomberman();
	Vector2f getPlayerPosition();
	void playerWalk();
	void playerWalkBackwards() { movePlayerForward(-_moveStep); }

	void rotatePlayerLeft();
	void rotatePlayerRight();

	void liftArms();
	void lowerArms();

	void placeBomb();
	bool update(unsigned elapsedTime);

	static Vector2f angleTo2D(float angleDeg);
	static unsigned getCurrentTime();
	void debug();
};
