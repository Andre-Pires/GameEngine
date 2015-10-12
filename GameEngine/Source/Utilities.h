#pragma once
#include <codecvt>
#include "../Dependencies/glew/glew.h"
#include <corecrt_memcpy_s.h>
#include <string>
#include <fstream>
#include <iostream>

//estrutura usada para guardar a informacao dos vertices
// por enquanto as posicoes e cores dos vertices
typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

//colors
const GLfloat grey[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat orange[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
const GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat pink[4] = { 1.0f, 0.0f, 0.6f, 1.0f };
const GLfloat red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat purple[4] = { 0.4f, 0.0f, 0.6f, 1.0f };
const GLfloat yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

enum Color { GREY, ORANGE, RED, GREEN, BLUE, PINK, YELLOW, PURPLE, BLACK };

class Utilities
{
public:

	static bool isOpenGLError();
	static void checkOpenGLError(std::string error);
	static GLchar* loadFile(char* path);
};
