#pragma once
#include <vector>
#include "GameEntity.h"

class GridMap
{
	std::vector<std::vector<GameEntity*>> grid_;
	unsigned player_row_;
	unsigned player_col_;
	GameEntity *player_entity_;

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


	unsigned getPlayerRow() const { return player_row_; }
	unsigned getPlayerCol() const { return player_col_; }
	
	void setPlayerRow(unsigned row) { player_row_ = row; }
	void setPlayerCol(unsigned col) { player_col_ = col; }

	GameEntity* getPlayerEntity() const { return player_entity_; }
};

