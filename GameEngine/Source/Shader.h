#pragma once
#include "../Dependencies/glew/glew.h"
#include <iostream>
#include <fstream>
#include <map>
#include <array>
#include <list>
#include <string>
#include "Utilities.h"

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
	std::list<std::string> uniformName;
	std::map<std::string, GLint> uniformId;
	//storage for uniform block names and ids
	std::list<char *> uniformBlockName;
	std::map<char *, GLuint> uniformBlockLocation;
	std::map<char *, GLint> uniformBlockId;

public:
	ShaderType shaderType;
	Shader(ShaderType type);
	void addShader(GLenum shaderType, char* shaderLocation);
	void addAttribute(GLuint location, char* atributeName);
	void addUniform(std::string uniformName);
	void addUniformBlock(GLuint location, char* blockName);
	void createShaderProgram();
	void destroyShaderProgram();
	void useShaderProgram();
	void dropShaderProgram();
	GLint getUniformLocation(std::string uniformName);
	GLint getShaderId(GLenum shader);
	GLuint getProgramId();
	void checkShaderError(std::string error);
	static bool isOpenGLError();
	static void checkGenericOpenGLError(std::string error);
	ShaderType getShaderType();
};
