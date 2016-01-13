#pragma once
#include <vector>
#include "GameEntity.h"

class GridMap
{
	std::vector<std::vector<GameEntity*>> _grid;
	unsigned _playerRow;
	unsigned _playerCol;

	void parseFile(std::string filename);
	std::vector<GameEntity*> parseLine(std::string line, unsigned row);
	GameEntity* parseChar(char c, unsigned row, unsigned col);
public:
	GridMap(std::string& filename);
	~GridMap();

	bool isValid(unsigned row, unsigned col) const;
	bool isClear(unsigned, unsigned) const;

	void clear(unsigned, unsigned);
	bool isDestructible(unsigned row, unsigned col);
	void setEntity(unsigned row, unsigned col, GameEntity* gameEntity);

	void moveEntity(unsigned row, unsigned col, unsigned newRow, unsigned newCol);


	unsigned getPlayerRow() const { return _playerRow; }
	unsigned getPlayerCol() const { return _playerCol; }
	
	void setPlayerRow(unsigned row) { _playerRow = row; }
	void setPlayerCol(unsigned col) { _playerCol = col; }
};

