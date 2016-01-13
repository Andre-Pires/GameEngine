#include "Bomberman.h"
#include "GameManager.h"
#include "Bomb.h"

Bomberman::Bomberman(std::string&& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader, CallbackType activateFlash) : _playerPosition(1, 1), _playerOrientation(0), _totalWalkTime(WALK_ANIMATION_DURATION), _totalRotationTime(ROTATE_ANIMATION_DURATION), _rotationDirection(0), _playerActive(false), _startingFoot(-1)
{
	GameManager::getInstance().init(scene, gameNode, bufferObjects, shader);

	_gridMap = new GridMap(filename);
	_playerPosition = Vector2f(_gridMap->getPlayerCol(), _gridMap->getPlayerRow());
	_activateFlash = activateFlash;
}

Bomberman::~Bomberman()
{
}

Vector2f Bomberman::getPlayerPosition()
{
	return _playerPosition;
}

void Bomberman::playerWalk()
{
	if (_playerActive) return;

	auto playerOrientation2D = angleTo2D(-_playerOrientation);
	unsigned rowAhead = _gridMap->getPlayerRow() - round(playerOrientation2D.y);
	unsigned colAhead = _gridMap->getPlayerCol() + round(playerOrientation2D.x);

	if (_gridMap->isClear(rowAhead, colAhead))
	{
		_playerActive = true;
		_startingFoot = -_startingFoot;
		_totalWalkTime = 0;
	}
}

void Bomberman::rotatePlayerLeft()
{
	if (_playerActive) return;

	_playerActive = true;
	_totalRotationTime = 0;
	_rotationDirection = -1;
}

void Bomberman::rotatePlayerRight()
{
	if (_playerActive) return;

	_playerActive = true;
	_totalRotationTime = 0;
	_rotationDirection = 1;
}

bool Bomberman::update(unsigned elapsedTime)
{
	for (auto bomb = _bombs.begin(); bomb < _bombs.end(); )
	{
		if ((*bomb)->getExplosionTime() <= getCurrentTime())
		{
			explode((*bomb)->getRow(), (*bomb)->getCol());
			delete (*bomb)->getEntity();
			_gridMap->setEntity((*bomb)->getRow(), (*bomb)->getCol(), nullptr);
			bomb = _bombs.erase(bomb);
		}
		else
		{
			++bomb;
		}
	}

	animationsUpdate(elapsedTime);

	return true;
}

Vector2f Bomberman::angleTo2D(float angleDeg)
{
	float rad = (angleDeg * PI) / 180;

	return Vector2f(cos(rad), sin(rad));
}

unsigned Bomberman::getCurrentTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void Bomberman::placeBomb()
{
	if (_playerActive) return;

	auto playerOrientation2D = angleTo2D(-_playerOrientation);
	unsigned bombRow = _gridMap->getPlayerRow() - round(playerOrientation2D.y);
	unsigned bombCol = _gridMap->getPlayerCol() + round(playerOrientation2D.x);

	if (_gridMap->isClear(bombRow, bombCol))
	{
		auto bombEntity = GameManager::getInstance().createBomb(bombCol, -float(bombRow));
		_gridMap->setEntity(bombRow, bombCol, bombEntity);

		_bombs.push_back(new Bomb(bombEntity, getCurrentTime() + BOMB_EXPLOSION_TIME, bombRow, bombCol));
	}
}

bool Bomberman::movePlayerForward(float distance)
{
	auto playerDirection = angleTo2D(-_playerOrientation);
	auto deltaPos = playerDirection * distance;
	deltaPos.y = -deltaPos.y;
	auto newPlayerPos = _playerPosition + deltaPos;

	unsigned newRow = round(newPlayerPos.y);
	unsigned newCol = round(newPlayerPos.x);
	unsigned oldRow = _gridMap->getPlayerRow();
	unsigned oldCol = _gridMap->getPlayerCol();

	bool valid = true;

	if (newRow != oldRow || newCol != oldCol)
	{
		valid = _gridMap->isClear(newRow, newCol);

		if (valid)
		{
			_gridMap->moveEntity(oldRow, oldCol, newRow, newCol);
			_gridMap->setPlayerRow(newRow);
			_gridMap->setPlayerCol(newCol);
		}
	}

	if (valid)
	{
		_playerEntity->getNode()->translate(Vector3f(deltaPos.x, -deltaPos.y, 0));
		_playerPosition += Vector2f(deltaPos.x, deltaPos.y);
	}

	return valid;
}

void Bomberman::rotatePlayer(float angleDeg)
{
	_playerOrientation += angleDeg;

	_playerEntity->getNode()->clearTransformations();
	_playerEntity->getNode()->rotate(-_playerOrientation, Vector3f(0, 0, 1));
	_playerEntity->getNode()->translate(Vector3f(_playerPosition.x + 0.5f, -_playerPosition.y + 0.5f, 0));
}

void Bomberman::explode(unsigned row, unsigned col)
{
	_activateFlash();

	if (!_gridMap->isClear(row - 1, col))
	{
		if (_gridMap->isDestructible(row - 1, col))
		{
			_gridMap->clear(row - 1, col);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row - 2, col))
		{
			_gridMap->clear(row - 2, col);
		}
	}

	if (!_gridMap->isClear(row + 1, col))
	{
		if (_gridMap->isDestructible(row + 1, col))
		{
			_gridMap->clear(row + 1, col);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row + 2, col))
		{
			_gridMap->clear(row + 2, col);
		}
	}

	if (!_gridMap->isClear(row, col - 1))
	{
		if (_gridMap->isDestructible(row, col - 1))
		{
			_gridMap->clear(row, col - 1);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row, col - 2))
		{
			_gridMap->clear(row, col - 2);
		}
	}

	if (!_gridMap->isClear(row, col + 1))
	{
		if (_gridMap->isDestructible(row, col + 1))
		{
			_gridMap->clear(row, col + 1);
		}
	}
	else
	{
		if (_gridMap->isDestructible(row, col + 2))
		{
			_gridMap->clear(row, col + 2);
		}
	}
}

void Bomberman::animationsUpdate(unsigned elapsedTime)
{
	_playerActive = false;

	unsigned walkTime;
	if (_totalWalkTime + elapsedTime >= WALK_ANIMATION_DURATION)
	{
		walkTime = WALK_ANIMATION_DURATION - _totalWalkTime;
	}
	else
	{
		walkTime = elapsedTime;
	}

	if (walkTime > 0)
	{
		_playerActive = true;

		_totalWalkTime += walkTime;
		float percentageOfAnimation = float(walkTime) / WALK_ANIMATION_DURATION;
		movePlayerForward(percentageOfAnimation);
		wavePlayerMembers(sin(float(_totalWalkTime) / WALK_ANIMATION_DURATION * PI) * _startingFoot);
	}

	unsigned rotationTime;
	if (_totalRotationTime + elapsedTime >= ROTATE_ANIMATION_DURATION)
	{
		rotationTime = ROTATE_ANIMATION_DURATION - _totalRotationTime;
	}
	else
	{
		rotationTime = elapsedTime;
	}

	if (rotationTime > 0)
	{
		_playerActive = true;

		_totalRotationTime += rotationTime;
		float percentageOfAnimation = float(rotationTime) / ROTATE_ANIMATION_DURATION;
		auto frameAngle = percentageOfAnimation * 90 * _rotationDirection;
		rotatePlayer(frameAngle);
	}

	for (auto bomb = _bombs.begin(); bomb < _bombs.end(); ++bomb)
	{
		auto timeUntillExplosion = (*bomb)->getExplosionTime() - getCurrentTime();
		auto timeSincePlaced = BOMB_EXPLOSION_TIME - timeUntillExplosion;
		animateBomb((*bomb)->getEntity()->getNode(), float(timeSincePlaced) / BOMB_EXPLOSION_TIME);
	}
}

void Bomberman::wavePlayerMembers(float harmonicPercentage)
{
	auto rotAxis = Vector3f(1, 0, 0);
	auto rotMultiplier = 20;

	GameManager::getInstance().getRightHand()->clearTransformations();
	GameManager::getInstance().getRightHand()->rotate(harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getLeftHand()->clearTransformations();
	GameManager::getInstance().getLeftHand()->rotate(-harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getRightFoot()->clearTransformations();
	GameManager::getInstance().getRightFoot()->rotate(-harmonicPercentage * rotMultiplier, rotAxis);

	GameManager::getInstance().getLeftFoot()->clearTransformations();
	GameManager::getInstance().getLeftFoot()->rotate(harmonicPercentage * rotMultiplier, rotAxis);
}

void Bomberman::animateBomb(SceneGraphNode* node, float percentage)
{
	auto scale = 1 -sin(percentage * 1.1f * PI) * percentage*percentage;

	node->clearTransformations();
	node->scale(Vector3f(scale, scale, scale));
}

void Bomberman::debug()
{
	explode(_gridMap->getPlayerRow(), _gridMap->getPlayerCol());
}
