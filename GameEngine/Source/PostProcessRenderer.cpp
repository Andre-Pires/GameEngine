#include "PostProcessRenderer.h"

#include "ShadowRenderer.h"
#include "Light.h"
#include "Camera.h"
#include "SceneGraphNode.h"

PostProcessRenderer::PostProcessRenderer(Scene * scene, SceneGraphNode * sceneGraph, Shader * shader, Shader * postProcessShader)
{
	this->scene = scene;
	this->shader = shader;
	this->postProcessingShader = postProcessShader;
	this->sceneGraph = sceneGraph;
	this->WinX = 1024;
	this->WinY = 576;
	this->ratio = 1024 / 576;

	postProcessingShader->useShaderProgram();
	this->sceneUniformId = postProcessingShader->getUniformLocation("sceneTexture");
	postProcessingShader->dropShaderProgram();
}

void PostProcessRenderer::generatePostProcessFBO(int viewX, int viewY)
{
	updateViewport(viewX, viewY);

	int shadowMapWidth = WinX * SHADOW_MAP_RATIO;
	int shadowMapHeight = WinY * SHADOW_MAP_RATIO;

	// create a framebuffer object
	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	// Try to use a texture depth component
	glGenTextures(1, &sceneTexture);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, shadowMapWidth, shadowMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); // o GL_FLOAT pode ser GL_UNSIGNED_BYTE

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// The depth buffer

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	//attach the texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sceneTexture, 0);
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// check FBO status
	GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (FBOstatus) {
	case GL_FRAMEBUFFER_UNDEFINED:
		printf("FBO Undefined\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("FBO Incomplete Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("FBO Missing Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("FBO Incomplete Draw Buffer\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("FBO Unsupported\n");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		printf("FBO OK\n");
		break;
	default:
		printf("FBO Problem?\n");
	}

	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
		printf("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO\n");

	//	switch back to window-system-provided framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessRenderer::renderFlash()
{
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glViewport(0, 0, WinX, WinY); // Render on the whole framebuffer, complete from the lower left corner to the upper right

								  // Clear the screen
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->setActiveShader(shader);
	sceneGraph->draw();
	scene->setActiveShader(postProcessingShader);

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	postProcessingShader->useShaderProgram();

	//Note: texture starts at 7 to create sequential space for shadows and textures
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D, sceneTexture);
	glUniform1i(sceneUniformId, 7);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, WinX, WinY); // Render on the whole framebuffer, complete from the lower left corner to the upper right
								  // Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcessRenderer::updateViewport(int x, int y)
{
	this->WinX = x;
	this->WinY = y;
	this->ratio = WinX / WinY;
}