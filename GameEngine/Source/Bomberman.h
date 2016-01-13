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

	GridMap *_gridMap;
	std::vector<Bomb*> _bombs;
	Vector2f _playerPosition;
	float _playerOrientation;

	unsigned _totalWalkTime;
	unsigned _totalRotationTime;
	int _rotationDirection;
	bool _playerActive;
	int _startingFoot;
	CallbackType _activateFlash;

	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void explode(unsigned row, unsigned col);
	void animationsUpdate(unsigned elapsedTime);
	void wavePlayerMembers(float harmonicPercentage);
	void animateBomb(SceneGraphNode* node, float percentage);
public:
	Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash);
	~Bomberman();
	Vector2f getPlayerPosition();
	void playerWalk();
	void playerWalkBackwards() { movePlayerForward(-_moveStep); }

	void rotatePlayerLeft();
	void rotatePlayerRight();

	void placeBomb();
	bool update(unsigned elapsedTime);

	static Vector2f angleTo2D(float angleDeg);
	static unsigned getCurrentTime();
	void debug();
};
