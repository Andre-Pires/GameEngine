#include "Bomberman.h"
#include "GameManager.h"
#include "Bomb.h"

Bomberman::Bomberman(std::string& filename, Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader* shader): _playerPosition(1, 1), _playerOrientation(0)
{
	GameManager::getInstance().setScene(scene);
	GameManager::getInstance().setGameNode(gameNode);
	GameManager::getInstance().setBufferObjects(bufferObjects);
	GameManager::getInstance().setShader(shader);

	_gridMap = new GridMap(filename);
	_playerEntity = _gridMap->getPlayerEntity();
	_playerPosition = Vector2f(_gridMap->getPlayerCol(), _gridMap->getPlayerRow());
}

Bomberman::~Bomberman()
{
}

//void Bomberman::createSceneGraph(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader *shader)
//{
//	auto wallTexture = new Texture(shader, "Assets/textures/stone_wall_texture.jpg");
//	auto destructibleTexture = new Texture(shader, "Assets/textures/text.jpg");
//	auto playerTexture = new Texture(shader, "Assets/textures/text.jpg");
//	auto normalMap = new Texture(shader, "Assets/textures/stone_wall_texture_normal_map.jpg");
//
//	auto row = 0;
//
//	for (auto rowVec = _gameCells.begin(); rowVec < _gameCells.end(); ++rowVec)
//	{
//		auto col = 0;
//
//		for (auto colVec = (*rowVec).begin(); colVec < (*rowVec).end(); ++colVec)
//		{
//			if ((*colVec) != CellStatus::clear)
//			{
//				GeometricObject *aBox;
//				Texture *texture;
//
//				if ((*colVec) == CellStatus::player)
//				{
//					_playerX = col;
//					_playerY = row;
//					_playerObject = aBox = new Square(bufferObjects, scene);
//					aBox->changeColor(BLUE);
//					aBox->translate(Vector3f(0.5f, 0.5f, 0));
//					aBox->scale(Vector3f(0.5f, 0.5f, 1));
//					texture = playerTexture;
//					(*colVec) = CellStatus::clear;
//				}
//				else
//				{
//					aBox = new Square(bufferObjects, scene);
//					aBox->changeColor(((*colVec) == CellStatus::wall) ? GREY : BROWN);
//					texture = ((*colVec) == CellStatus::wall) ? wallTexture : destructibleTexture;
//				}
//
//				aBox->translate(Vector3f(col - 2, -(row - 2), 0));
//				aBox->repeatTexture(3.0);
//				gameNode->add(new SceneGraphNode(gameNode, aBox, scene, texture, normalMap));
//			}
//
//			auto ground = new Square(bufferObjects, scene);
//			ground->changeColor(GREEN);
//			ground->translate(Vector3f(col - 2, -(row - 2), -1));
//			gameNode->add(new SceneGraphNode(gameNode, ground, scene));
//
//			col++;
//		}
//
//		row++;
//	}
//}

bool Bomberman::update()
{
	for (auto bomb = _bombs.begin(); bomb < _bombs.end(); )
	{
		if ((*bomb)->getExplosionTime() <= std::chrono::system_clock::now())
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

	return true;
}

Vector2f Bomberman::angleTo2D(float angleDeg)
{
	float rad = (angleDeg * PI) / 180;

	return Vector2f(cos(rad), sin(rad));
}

bool Bomberman::placeBomb()
{
	auto playerOrientation2D = angleTo2D(-_playerOrientation);
	unsigned bombRow = _gridMap->getPlayerRow() - round(playerOrientation2D.y);
	unsigned bombCol = _gridMap->getPlayerCol() + round(playerOrientation2D.x);

	if (_gridMap->isClear(bombRow, bombCol))
	{
		auto bombEntity = GameManager::getInstance().createBomb(bombCol, -float(bombRow));
		_gridMap->setEntity(bombRow, bombCol, bombEntity);

		_bombs.push_back(new Bomb(bombEntity, bombRow, bombCol));

		return true;
	}
	else
	{
		return false;
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

void Bomberman::debug()
{
	explode(_gridMap->getPlayerRow(), _gridMap->getPlayerCol());
}