#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "Shader.h"

//Note: Buffer Objects uses the bind locations from Shader.h

class BufferObjects
{
private:
	BufferObjects();
	static BufferObjects * instance;
	

public:
	static BufferObjects* getInstance();
	void createBufferObjects(GLuint * VboId, GLuint VaoId, Vertex * Vertices, int verticesSize, GLubyte *Indices, int indicesSize);
	void createCameraBufferObjects(GLuint* camVboId);
	void updateCamera(GLuint * VboId, Matrix4f view, Matrix4f projection);
	void destroyBufferObjects(GLuint * VboId, GLuint VaoId);
	void bindVao(GLuint VaoId);
	void unbindVao();
	GLuint getVaoId();
	GLuint* getVboId();
	GLuint* getCameraVboId();
};
