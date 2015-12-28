#pragma once
#include "SceneGraphNode.h"
#define TEXTURE_RATIO 1.0

class PostProcessRenderer
{
private:
	Scene* scene;
	Shader* shader;
	Shader* postProcessingShader;
	SceneGraphNode* sceneGraph;
	int WinX;
	int WinY;
	int ratio;
	GLuint fboId;
	GLuint sceneTexture;
	GLuint depthRenderBuffer;
	GLuint sceneUniformId;;
public:
	PostProcessRenderer(Scene* scene, SceneGraphNode* sceneGraph, Shader* shader, Shader* postProcessShader);
	void generatePostProcessFBO(int viewX, int viewY);
	void renderFlash();
	void updateViewport(int x, int y);
};
