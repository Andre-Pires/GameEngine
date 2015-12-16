#include "Bomb.h"


Bomb::Bomb(GameEntity* entity, unsigned explosionAt, unsigned row, unsigned col): _entity(entity), _explosionAt(explosionAt), _row(row), _col(col)
{
}

Bomb::~Bomb()
{
}


unsigned Bomb::getExplosionTime() const
{
	return _explosionAt;
}

GameEntity* Bomb::getEntity() const
{
	return _entity;
}