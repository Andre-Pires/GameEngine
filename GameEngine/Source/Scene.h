#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "BufferObjects.h"
#include "Shader.h"

class Scene
{
public:
	void draw(int vertices, GLuint vao);
};
