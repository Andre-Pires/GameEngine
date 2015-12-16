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
	//std::vector<std::vector<CellStatus>> _gameCells;

	GridMap *_gridMap;
	std::vector<Bomb*> _bombs;
	Vector2f _playerPosition;
	float _playerOrientation;
	GameEntity *_playerEntity;

	bool movePlayerForward(float distance);
	void rotatePlayer(float angleDeg);
	void explode(unsigned row, unsigned col);
public:
	Bomberman(std::string& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);
	~Bomberman();

	void createSceneGraph(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);

	bool playerWalk() { return movePlayerForward(_moveStep); }
	bool playerWalkBackwards() { return movePlayerForward(-_moveStep); }

	void rotatePlayerLeft() { rotatePlayer(-90); }
	void rotatePlayerRight() { rotatePlayer(90); }

	bool placeBomb();
	bool update();

	static Vector2f angleTo2D(float angleDeg);
	void debug();
};
