#pragma once
#include <vector>
#include "SceneGraphNode.h"
#include <string>
#include <fstream>
#include <cassert>
#include "Square.h"
#include "Diamond.h"
#include "Triangle.h"

enum class CellStatus { clear, wall, player, destructible };

class Bomberman
{
private:
	const float _moveStep = 1;
	std::vector<std::vector<CellStatus>> _gameCells;
	GeometricObject *_playerObject;
	float _playerX;
	float _playerY;

	void parseFile(std::string filename);
	static std::vector<CellStatus> parseLine(std::string line);
	static CellStatus parseCharacter(char c);

	static CellStatus fromCharToCS(char c);
	static char fromCSToChar(CellStatus cs);
	bool movePlayer(float dx, float dy);
public:
	Bomberman(std::string& filename);
	~Bomberman();

	void createSceneGraph(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader);

	bool isCellClear(unsigned row, unsigned col) const;

	bool setPlayerPos(float x, float y);

	bool movePlayerLeft() { return movePlayer(-_moveStep, 0); }
	bool movePlayerRight() { return movePlayer(_moveStep, 0); }
	bool movePlayerUp() { return movePlayer(0, _moveStep); }
	bool movePlayerDown() { return movePlayer(0, -_moveStep); }

	std::string dump() const;
};
