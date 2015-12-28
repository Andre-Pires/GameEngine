#pragma once
#include <codecvt>
#include "../Dependencies/glew/glew.h"
#include <corecrt_memcpy_s.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

//shader locations
const GLuint UBO_BP = 0;
#define VERTICES 0
#define NORMALS 1
#define TEXCOORDS 2
#define TANGENTS 3
#define WOOD 4

// angle unit conversion
#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

// floating point tolerance
const float slackThreshold = (float)1.0e-5;

//estrutura usada para guardar a informacao dos vertices
// por enquanto as posicoes e cores dos vertices
typedef struct {
	GLfloat XYZW[4];
} Vertex;

typedef struct {
	GLfloat AMBIENT[4];
	GLfloat DIFFUSE[4];
	GLfloat SPECULAR[4];
} Material;

typedef struct {
	GLfloat UV[2];
} Texcoord;

typedef struct {
	GLfloat NXNYNZ[3];
} Normal;

typedef struct {
	GLfloat TXTYTZ[3];
} Tangent;

typedef struct {
	float RGB[3];
} tex3D;

//colors
const GLfloat greyAmbient[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
const GLfloat greyDiffuse[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
const GLfloat greySpecular[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

const GLfloat orangeAmbient[4] = { 0.15f, 0.25f, 0.0f, 1.0f };
const GLfloat orangeDiffuse[4] = { 0.7f, 0.4f, 0.0f, 1.0f };
const GLfloat orangeSpecular[4] = { 1.0f, 0.6f, 0.0f, 1.0f };

const GLfloat greenAmbient[4] = { 0.0f, 0.2f, 0.0f, 1.0f };
const GLfloat greenDiffuse[4] = { 0.0f, 0.7f, 0.0f, 1.0f };
const GLfloat greenSpecular[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

const GLfloat blueAmbient[4] = { 0.0f, 0.0f, 0.2f, 1.0f };
const GLfloat blueDiffuse[4] = { 0.0f, 0.0f, 7.0f, 1.0f };
const GLfloat blueSpecular[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

const GLfloat pinkAmbient[4] = { 0.15f, 0.0f, 0.15f, 1.0f };
const GLfloat pinkDiffuse[4] = { 0.7f, 0.0f, 0.4f, 1.0f };
const GLfloat pinkSpecular[4] = { 1.0f, 0.0f, 0.6f, 1.0f };

const GLfloat redAmbient[4] = { 0.15f, 0.0f, 0.0f, 1.0f };
const GLfloat redDiffuse[4] = { 0.7f, 0.0f, 0.0f, 1.0f };
const GLfloat redSpecular[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

const GLfloat purpleAmbient[4] = { 0.15f, 0.0f, 0.25f, 1.0f };
const GLfloat purpleDiffuse[4] = { 0.3f, 0.0f, 0.5f, 1.0f };
const GLfloat purpleSpecular[4] = { 0.4f, 0.0f, 0.6f, 1.0f };

const GLfloat yellowAmbient[4] = { 0.15f, 0.15f, 0.0f, 1.0f };
const GLfloat yellowDiffuse[4] = { 0.7f, 0.7f, 0.0f, 1.0f };
const GLfloat yellowSpecular[4] = { 1.0f, 1.0f, 0.0f, 1.0f };

const GLfloat brownAmbient[4] = { 0.1254902f, 0.0643138f, 0.0352942f, 1.0f };
const GLfloat brownDiffuse[4] = { 0.513725f, 0.10784f, 0.088235f, 1.0f };
const GLfloat brownSpecular[4] = { 0.627451f, 0.321569f, 0.176471f, 1.0f };

const GLfloat blackAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat blackDiffuse[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat blackSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

const GLfloat whiteAmbient[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
const GLfloat whiteDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat whiteSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

enum Color { GREY, ORANGE, RED, GREEN, BLUE, PINK, YELLOW, PURPLE, BLACK, BROWN, WHITE };
enum CameraType { ORTHOGRAPHIC, PERSPECTIVE, CONTROLLED_PERSP };
enum GimbalLockState { GIMBAL_LOCK_ON, GIMBAL_LOCK_OFF };
enum AnimationState { ANIMATION_STANDARD, ANIMATION_REVERSE, ANIMATION_OFF, ANIMATION_ON };
enum LightType { POINT_LIGHT, SPOTLIGHT, DIRECTIONAL_LIGHT };
enum ShaderType { MAIN_SHADER, SHADOW_SHADER, POSTPROCESS_SHADER };

class Utilities
{
public:

	static float lerp(float val1, float val2, float ratio);
	static GLchar* loadFile(char* path);
};
