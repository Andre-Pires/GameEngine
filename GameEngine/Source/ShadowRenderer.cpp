#include "ShadowRenderer.h"
#include "Light.h"
#include "Camera.h"
#include "SceneGraphNode.h"

ShadowRenderer::ShadowRenderer(Light * light, int lightIndex, Camera * camera, Scene * scene, SceneGraphNode * sceneGraph, Shader * shader, Shader * shadowShader, GLuint lightViewMatrixId, GLuint shadowMapId)
{
	this->scene = scene;
	this->shader = shader;
	this->shadowShader = shadowShader;
	this->sceneGraph = sceneGraph;
	this->light = light;
	this->lightIndex = lightIndex;
	this->camera = camera;
	this->WinX = 1024;
	this->WinY = 576;
	this->ratio = 1024 / 576;
	this->center = Vector3f(0.0f, 0.0f, 0.0f);
	this->lightViewMatrixId = lightViewMatrixId;
	this->shadowMapId = shadowMapId;
}

void ShadowRenderer::generateShadowFBO()
{
	int shadowMapWidth = WinX * SHADOW_MAP_RATIO;
	int shadowMapHeight = WinY * SHADOW_MAP_RATIO;

	GLenum FBOstatus;

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Remove artifact on the edges of the shadowmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	GLfloat * ones = new GLfloat[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, ones);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);

	// Instruct openGL that we won't bind a color texture with the currently bound FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// check FBO status
	FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");

	//	switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowRenderer::renderShadows()
{
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glViewport(0, 0, WinX * SHADOW_MAP_RATIO, WinY * SHADOW_MAP_RATIO); // Render on the whole framebuffer, complete from the lower left corner to the upper right

																		// Clear the screen
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Vector3f lightPos = Vector3f(light->position.x, light->position.y, light->position.z);

	if (light->lightType == DIRECTIONAL_LIGHT)
	{
		// activate offset for polygons
		glEnable(GL_POLYGON_OFFSET_FILL);
		// offset by two units equal to smallest value of change in the shadow map
		// and offset by two units depending on the slope of the polygon
		glPolygonOffset(2.0f, 2.0f);
		camera->ortho(-20.0f + lightPos.x, 20.0f + lightPos.x, -20.0f + lightPos.y, 20.0f + lightPos.y, -100.0f + lightPos.z, 100.0f + lightPos.z);
		camera->lookAt(lightPos, center, Vector3f(0, 1, 0));
	}
	else
	{
		// for spot and point light :
		camera->perspective(120.0f, ratio, 0.1f, 50.0f);
		camera->lookAt(lightPos, Vector3f(lightPos.x, lightPos.y, center.z), Vector3f(0, 1, 0));
	}

	camera->updateCamera();
	Matrix4f depthProjectionMatrix = camera->getProjectionMatrix();
	Matrix4f depthViewMatrix = camera->getViewMatrix();
	Matrix4f depthMVP = depthProjectionMatrix * depthViewMatrix;

	Matrix4f biasMatrix = Matrix4f(new float[16]
	{
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	});

	scene->setActiveShader(shadowShader);
	sceneGraph->draw();
	scene->setActiveShader(shader);

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	shader->useShaderProgram();

	glUniformMatrix4fv(lightViewMatrixId, 1, GL_FALSE, MatrixFactory::Mat4toGLfloat(biasMatrix * depthMVP));
	//Note: texture starts at 1 since index 0 is already in use
	glActiveTexture(GL_TEXTURE2 + lightIndex);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glUniform1i(shadowMapId, 2 + lightIndex);

	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (light->lightType == DIRECTIONAL_LIGHT)
		glDisable(GL_POLYGON_OFFSET_FILL);

	glViewport(0, 0, WinX, WinY); // Render on the whole framebuffer, complete from the lower left corner to the upper right
								  // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->dropShaderProgram();
}

void ShadowRenderer::updateViewport(int x, int y)
{
	this->WinX = x;
	this->WinY = y;
	this->ratio = WinX / WinY;
}

void ShadowRenderer::updateCameraCenter(Vector3f center)
{
	this->center = center;
}