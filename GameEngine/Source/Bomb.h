#pragma once
#include "GameEntity.h"
#include <chrono>

class Bomb
{
	std::chrono::system_clock::time_point _explosionAt;
	GameEntity *_entity;
	unsigned _row;
	unsigned _col;
public:
	Bomb(GameEntity* entity, unsigned row, unsigned col);
	~Bomb();
	std::chrono::system_clock::time_point getExplosionTime() const;
	GameEntity* getEntity() const;
	unsigned getRow() const { return _row; }
	unsigned getCol() const { return _col; }
};

