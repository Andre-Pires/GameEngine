#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <list>

const GLuint UBO_BP = 0;

class Shader
{
private:
	//Usados apenas como forma de nomear os diferentes shaders
	GLuint ProgramId;
	GLint UniformId;
	GLint UboId;
	std::map<GLenum, GLuint> shadersId;
	std::map<GLuint, GLchar*> shaders;
	std::array<GLenum, 3> shaderTypes = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	std::map<int, char *> shaderAttributes;
public:
	Shader();
	void addShader(GLenum shaderType, char* shaderLocation);
	void addAttribute(int location, char* atributeName);
	void addUniform(char* uniformName);
	void addUniformBlock(GLuint location, char* blockName);
	void createShaderProgram();
	void destroyShaderProgram();
	void useShaderProgram();
	void dropShaderProgram();
	GLint getUniformLocation();
};
