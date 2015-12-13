#pragma once
#include "../Dependencies/glew/glew.h"
#include "Vector3f.h"
#include "Utilities.h"
#include <ratio>
#include "Matrix4f.h"
#include "Camera.h"
#include "Light.h"
#include "SceneGraphNode.h"
#define SHADOW_MAP_RATIO 1.0

class ShadowRenderer
{
private:
	Vector3f center;
	int WinY;
	int WinX;
	Camera* camera;
	Light* light;
	GLuint fboId;
	GLuint depthTextureId;
	int ratio;
	SceneGraphNode* sceneGraph;
	Shader* shadowShader;
	Shader* shader;
	Scene* scene;
	int lightIndex;
	GLuint lightViewMatrixId;
	GLuint shadowMapId;
public:
	ShadowRenderer(Light* light, int lightIndex, Camera* camera, Scene* scene, SceneGraphNode* sceneGraph, Shader* shader, Shader* shadowShader);
	void generateShadowFBO(int viewX, int viewY);
	void renderShadows();
	void updateViewport(int x, int y);
	void updateCameraCenter(Vector3f center);
};
