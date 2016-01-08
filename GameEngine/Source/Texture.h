#pragma once

#include <iostream>

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"
#include "..\Simple OpenGL Image Library\src\SOIL.h"

#include "Shader.h"
#include "Noise.h"

class Texture {
public:
	GLuint textureID;
	//Possible texture types:
	// 0 - no texture
	// 1 - image texture
	// 2 - procedural wood texture
	// 3 - procedural marble texture
	int textureType;
public:
	Texture(Shader* shader, char* filename);
	Texture(Shader* shader);
	void bind(GLenum type);
	void unbind(GLenum type);
	GLuint getTextureID();
	GLint getTexUniform(Shader* shader, int type);
	void setTextureType(int type);
private:
	unsigned char *data;
	tex3D noise[32 * 32 * 32];
	void prepareTexture(GLenum type, int size1, int size2);
};