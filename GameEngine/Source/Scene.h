#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "BufferObjects.h"
#include "Shader.h"
#include "Texture.h"
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "Camera.h"

class Scene
{
	Shader* shader;
	char * modelName;
	char * normalName;
	Camera* camera;
public:
	Scene(Shader* shader, Camera * camera, char * modelName, char * normalName);
	void draw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors, float materialShininess, Texture* texture, Texture* normalMap, Texture* channel);
	void standardDraw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors, float materialShininess, Texture* texture, Texture* normalMap, Texture* channel);
	void drawShadowMap(int vertices, GLuint vao, Matrix4f modelMatrix);
	void drawPostProcess(int vertices, GLuint vao);
	void setActiveShader(Shader* shader);
};
