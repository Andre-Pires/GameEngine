#include "Bomberman.h"
#include <string>
#include <fstream>
#include <cassert>
#include "Square.h"
#include "Diamond.h"
#include "Triangle.h"


Bomberman::Bomberman(std::string& filename)
{
	parseFile(filename);
}


Bomberman::~Bomberman()
{
}

void Bomberman::createSceneGraph(Scene* scene, SceneGraphNode* gameNode, BufferObjects* bufferObjects, Shader *shader)
{
	auto wallTexture = new Texture(shader, "Assets/textures/stone_wall_texture.jpg");
	auto destructibleTexture = new Texture(shader, "Assets/textures/text.jpg");
	auto playerTexture = new Texture(shader, "Assets/textures/text.jpg");

	auto row = 0;

	for (auto rowVec = _gameCells.begin(); rowVec < _gameCells.end(); ++rowVec)
	{
		auto col = 0;

		for (auto colVec = (*rowVec).begin(); colVec < (*rowVec).end(); ++colVec)
		{
			if ((*colVec) != CellStatus::clear)
			{
				GeometricObject *aBox;
				Texture *texture;

				if ((*colVec) == CellStatus::player)
				{
					_playerX = col;
					_playerY = row;
					_playerObject = aBox = new Square(bufferObjects, scene);
					aBox->changeColor(BLUE);
					aBox->translate(Vector3f(0.5f, 0.5f, 0));
					aBox->scale(Vector3f(0.5f, 0.5f, 1));
					texture = playerTexture;
					(*colVec) = CellStatus::clear;
				}
				else
				{
					aBox = new Square(bufferObjects, scene);
					aBox->changeColor(((*colVec) == CellStatus::wall) ? GREY : BROWN);
					texture = ((*colVec) == CellStatus::wall) ? wallTexture : destructibleTexture;
				}

				aBox->translate(Vector3f(col - 2, -(row - 2), 0));
				aBox->repeatTexture(3.0);
				gameNode->add(new SceneGraphNode(gameNode, aBox, scene, texture));

			}

			col++;
		}

		row++;
	}
}

void Bomberman::parseFile(std::string filename)
{
	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		_gameCells.push_back(parseLine(line));
	}
}


std::vector<CellStatus> Bomberman::parseLine(std::string line)
{
	std::vector<CellStatus> gameRow;

	for (auto& c : line)
	{
		gameRow.push_back(parseCharacter(c));
	}

	return gameRow;
}


CellStatus Bomberman::parseCharacter(char c)
{
	switch (c)
	{
	case ' ':
		return CellStatus::clear;
	case '#':
		return CellStatus::wall;
	case '+':
		return CellStatus::destructible;
	case '1':
		return CellStatus::player;
	default:
		assert(false);
		return CellStatus::clear;
	}
}

bool Bomberman::isCellClear(unsigned row, unsigned col) const
{
	return _gameCells[row][col] == CellStatus::clear;
}

bool Bomberman::setPlayerPos(float x, float y)
{
	assert(x >= 0);
	assert(y >= 0);

	unsigned row = int(x);
	unsigned col = int(y);

	bool valid = isCellClear(row, col);

	if (valid)
	{
		
	}

	return valid;
}

std::string Bomberman::dump() const
{
	std::string raw = "";

	for (auto rowVec = _gameCells.begin(); rowVec < _gameCells.end(); ++rowVec)
	{
		if (rowVec != _gameCells.begin())
			raw += "\n";

		for (auto colVec = (*rowVec).begin(); colVec < (*rowVec).end(); ++colVec)
			raw += fromCSToChar(*colVec);
	}
	return raw;
}


CellStatus Bomberman::fromCharToCS(char c)
{
	CellStatus status;

	switch (c)
	{
	case ' ':
		status = CellStatus::clear;
	case '#':
		status = CellStatus::wall;
	case '+':
		status = CellStatus::destructible;
	case '1':
		status = CellStatus::player;
	default:
		assert(false);
		status = CellStatus::clear;
	}

	return status;
}


char Bomberman::fromCSToChar(CellStatus cs)
{
	char c;

	switch (cs)
	{
	case CellStatus::clear:
		c = ' ';
		break;
	case CellStatus::wall:
		c = '#';
		break;
	case CellStatus::destructible:
		c = '+';
		break;
	case CellStatus::player:
		c = '1';
		break;
	default:
		assert(false);
		c = ' ';
		break;
	}

	return c;
}

bool Bomberman::movePlayer(float dx, float dy)
{
	unsigned row = round(_playerY - dy);
	unsigned col = round(_playerX + dx);

	bool valid = isCellClear(row, col);

	if (valid)
	{
		_playerObject->translate(Vector3f(dx, dy, 0));
		_playerX += dx;
		_playerY -= dy;
	}

	return valid;
}
