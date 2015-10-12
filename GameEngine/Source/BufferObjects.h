#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"

#define VERTICES 0
#define COLORS 1

class BufferObjects
{
private:
	BufferObjects();
	static BufferObjects * instance;


public:
	static BufferObjects* getInstance();
	void createBufferObjects(GLuint * VboId, GLuint VaoId, Vertex * Vertices, int verticesSize, GLubyte *Indices, int indicesSize);
	void destroyBufferObjects(GLuint * VboId, GLuint VaoId);
	void bindVao(GLuint VaoId);
	void unbindVao();
	GLuint getVaoId();
	GLuint* getVboId();
};
