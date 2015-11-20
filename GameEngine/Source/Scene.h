#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "BufferObjects.h"
#include "Shader.h"
#include "Matrix4f.h"

class Scene
{
	Shader* shader;
	char * modelName;
	char * normalName;
public:
	Scene(Shader* shader, char * modelName, char * normalName);
	void draw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors);
	void setActiveShader(Shader* shader);
};
