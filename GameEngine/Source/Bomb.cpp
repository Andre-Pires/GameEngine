#include "Bomb.h"



Bomb::Bomb(GameEntity* entity, unsigned row, unsigned col): _entity(entity), _row(row), _col(col)
{
	_explosionAt = std::chrono::system_clock::now() + std::chrono::seconds(3);
}

Bomb::~Bomb()
{
}


std::chrono::system_clock::time_point Bomb::getExplosionTime() const
{
	return _explosionAt;
}

GameEntity* Bomb::getEntity() const
{
	return _entity;
}