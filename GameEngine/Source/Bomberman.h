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

//enum class CellStatus { clear, wall, player, destructible, bomb };

class Bomberman
{
private:
	const float _moveStep = 1;
	const unsigned WALK_ANIMATION_DURATION = 500;
	const unsigned ROTATE_ANIMATION_DURATION = 400;
	//std::vector<std::vector<CellStatus>> _gameCells;

	GridMap *_gridMap;
	std::vector<Bomb*> _bombs;
	Vector2f _playerPosition;
	float _playerOrientation;
	GameEntity *_playerEntity;

	unsigned _totalWalkTime;
	unsigned _totalRotationTime;
	int _rotationDirection;
	bool _playerActive;


	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void explode(unsigned row, unsigned col);
	void animationsUpdate(unsigned elapsedTime);
public:
	Bomberman(std::string& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);
	~Bomberman();

	void createSceneGraph(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);

	void playerWalk();
	void playerWalkBackwards() { movePlayerForward(-_moveStep); }

	void rotatePlayerLeft();
	void rotatePlayerRight();

	void placeBomb();
	bool update(unsigned elapsedTime);

	static Vector2f angleTo2D(float angleDeg);
	void debug();
};
