#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <list>

class Shader
{
private:
	GLuint ProgramId;
	GLint UniformId;
	GLint UboId;
	//storage for shader text, id and type
	std::map<GLenum, GLuint> shadersId;
	std::map<GLuint, GLchar*> shaders;
	std::array<GLenum, 3> shaderTypes = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	//storage for shader atributes
	std::map<int, char *> shaderAttributes;
	//storage for uniform names and ids
	std::list<char *> uniformName;
	std::map<char *, GLint> uniformId;
	//storage for uniform block names and ids
	std::list<char *> uniformBlockName;
	std::map<char *, GLuint> uniformBlockLocation;
	std::map<char *, GLint> uniformBlockId;
public:
	Shader();
	void addShader(GLenum shaderType, char* shaderLocation);
	void addAttribute(GLuint location, char* atributeName);
	void addUniform(char* uniformName);
	void addUniformBlock(GLuint location, char* blockName);
	void createShaderProgram();
	void destroyShaderProgram();
	void useShaderProgram();
	void dropShaderProgram();
	GLint getUniformLocation(char* uniformName);
	GLint getShaderId(GLenum shader);
	GLuint getProgramId();
	void checkShaderError(std::string error);
	static bool isOpenGLError();
	static void checkGenericOpenGLError(std::string error);
};
