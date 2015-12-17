#include "GridMap.h"
#include <string>
#include "GameEntity.h"
#include <cassert>
#include "GameManager.h"


GridMap::GridMap(std::string& filename)
{
	parseFile(filename);
}


GridMap::~GridMap()
{
}

bool GridMap::isValid(unsigned row, unsigned col) const
{
	return _grid.size() > row && _grid[row].size() > col;
}

bool GridMap::isClear(unsigned row, unsigned col) const
{
	return isValid(row, col) && _grid[row][col] == nullptr;
}

void GridMap::clear(unsigned row, unsigned col)
{
	delete _grid[row][col];
	_grid[row][col] = nullptr;
}

bool GridMap::isDestructible(unsigned row, unsigned col)
{
	return _grid[row][col] != nullptr && _grid[row][col]->isDestructible();
}

void GridMap::setEntity(unsigned row, unsigned col, GameEntity* gameEntity)
{
	_grid[row][col] = gameEntity;
}

void GridMap::moveEntity(unsigned row, unsigned col, unsigned newRow, unsigned newCol)
{
	assert(isClear(newRow, newCol));

	_grid[newRow][newCol] = _grid[row][col];
	_grid[row][col] = nullptr;
}

void GridMap::parseFile(std::string filename)
{
	std::ifstream ifile(filename);
	auto row = 0;
	
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		_grid.push_back(parseLine(line, row++));
	}
}

std::vector<GameEntity*> GridMap::parseLine(std::string line, unsigned row)
{
	std::vector<GameEntity*> gameRow;
	auto col = 0;

	for (auto& c : line)
	{
		gameRow.push_back(parseChar(c, row, col++));
	}

	return gameRow;
}

GameEntity* GridMap::parseChar(char c, unsigned row, unsigned col)
{
	switch (c)
	{
	case ' ':
		GameManager::getInstance().createEmpty(col, -float(row));
		return nullptr;
	case '#':
		return GameManager::getInstance().createStaticWall(col, -float(row));
	case '+':
		return GameManager::getInstance().createDestructibleWall(col, -float(row));
	case '1':
		_playerRow = row;
		_playerCol = col;
		_playerEntity = GameManager::getInstance().createPlayer(col, -float(row));
		return _playerEntity;
	case 'x':
		return GameManager::getInstance().createBomb(col, -float(row));
	default:
		assert(false);
		return nullptr;
	}
}