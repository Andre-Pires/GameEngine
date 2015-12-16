#pragma once
#include "GameEntity.h"

class Bomb
{
	unsigned _explosionAt;
	GameEntity *_entity;
	unsigned _row;
	unsigned _col;

public:
	Bomb(GameEntity* entity, unsigned explosionAt, unsigned row, unsigned col);
	~Bomb();

	unsigned getExplosionTime() const;
	GameEntity* getEntity() const;
	unsigned getRow() const { return _row; }
	unsigned getCol() const { return _col; }
};

