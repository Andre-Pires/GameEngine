#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "BufferObjects.h"
#include "Shader.h"
#include "Matrix4f.h"

class Scene
{
	Shader* shader;
public:
	Scene(Shader* shader);
	void draw(int vertices, GLuint vao, Matrix4f transform);
	void setActiveShader(Shader* shader);
};
