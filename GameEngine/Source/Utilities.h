#pragma once
#include <codecvt>
#include "../Dependencies/glew/glew.h"
#include <corecrt_memcpy_s.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include "Shader.h"

//shader locations
const GLuint UBO_BP = 0;
#define VERTICES 0
#define COLORS 1
#define TEXCOORDS 2
#define NORMALS 3

// angle unit conversion
#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

// floating point tolerance
const float slackThreshold = (float)1.0e-5;

//estrutura usada para guardar a informacao dos vertices
// por enquanto as posicoes e cores dos vertices
typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

typedef struct {
	GLfloat UV[2];
} Texcoord;

typedef struct {
	GLfloat NXNYNZ[3];
} Normal;

//colors
const GLfloat grey[4] = { 0.6f, 0.6f, 0.6f, 1.0f };
const GLfloat orange[4] = { 1.0f, 0.6f, 0.0f, 1.0f };
const GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat pink[4] = { 1.0f, 0.0f, 0.6f, 1.0f };
const GLfloat red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat purple[4] = { 0.4f, 0.0f, 0.6f, 1.0f };
const GLfloat yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat brown[4] = { 0.627451f, 0.321569f, 0.176471f, 1.0f };
const GLfloat black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

enum Color { GREY, ORANGE, RED, GREEN, BLUE, PINK, YELLOW, PURPLE, BLACK, BROWN };
enum CameraType { ORTHOGRAPHIC, PERSPECTIVE, CONTROLLED_PERSP };
enum GimbalLockState { GIMBAL_LOCK_ON, GIMBAL_LOCK_OFF };
enum AnimationState { ANIMATION_STANDARD, ANIMATION_REVERSE, ANIMATION_OFF, ANIMATION_ON };

class Utilities
{
public:

	static float lerp(float val1, float val2, float ratio);
	static GLchar* loadFile(char* path);
};
