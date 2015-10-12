#pragma once
#include "GameObject.h"

class Triangle : public GameObject
{
private:
	static const int VERTEX_SIZE = 3;
	static const int INDEX_SIZE = 3;
public:
	Triangle(BufferObjects* buffer, Scene* scene);
};
