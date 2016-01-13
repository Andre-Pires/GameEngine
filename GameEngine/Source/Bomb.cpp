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

unsigned Bomb::getDroppedTime() const
{
	return dropped_at_;
}

GameEntity* Bomb::getEntity() const
{
	return _entity;
}

void Bomb::setDroppedTime(unsigned time)
{
	dropped_at_ = time;
}
