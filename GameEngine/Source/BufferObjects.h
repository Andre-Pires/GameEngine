#pragma once
#include "../Dependencies/glew/glew.h"
#include "Utilities.h"
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "Shader.h"
#include <vector>

//Note: Buffer Objects uses the bind locations from Shader.h

class BufferObjects
{
private:
	BufferObjects();
	static BufferObjects * instance;

public:
	static BufferObjects* getInstance();
	void createBufferObjects(GLuint* VboId, GLuint VaoId, std::vector<Vertex> Vertices, std::vector<GLubyte> Indices, std::vector<Normal> Normals, std::vector<Texcoord> Textures);
	void createCameraBufferObjects(GLuint* camVboId);
	void updateCamera(GLuint * VboId, Matrix4f view, Matrix4f projection);
	void destroyBufferObjects(GLuint * VboId, GLuint VaoId);
	void bindVao(GLuint VaoId);
	void unbindVao();
	GLuint getVaoId();
	GLuint* getVboId();
	GLuint* getCameraVboId();
};
