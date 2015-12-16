#pragma once

#include <iostream>

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"
#include "..\Simple OpenGL Image Library\src\SOIL.h"

#include "Shader.h"

class Texture {
public:
	GLuint textureID;
	int textureDim;
	tex3D noise[32 * 32 * 32];
public:
	Texture(Shader* shader, char* filename);
	Texture(Shader* shader);
	void bind();
	void unbind();
	GLuint getTextureID();
	GLint getTexUniform(Shader* shader, int type);
};