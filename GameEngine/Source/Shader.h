#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#define VERTICES 0
#define COLORS 1

class Shader
{
private:
	//Usados apenas como forma de nomear os diferentes shaders
	GLuint VertexShaderId, FragmentShaderId, ProgramId;
	GLint UniformId;
	GLchar* vertexShader;
	GLchar* fragmentShader;

	static Shader * instance;
	Shader();
public:
	static Shader* getInstance();
	void createShaderProgram();
	void destroyShaderProgram();
	void useShaderProgram();
	void dropShaderProgram();
	GLint getUniformLocation();
};
