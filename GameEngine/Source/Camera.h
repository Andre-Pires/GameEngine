#pragma once
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "Vector3f.h"
#include "BufferObjects.h"

class Scene;

class Camera
{
private:
	Matrix4f View;
	Matrix4f Projection;
	GLuint *cameraVboId;
	BufferObjects * bufferObjects;
	Scene * scene;
public:
	Camera(BufferObjects* buffer, Scene* scene);
	void updateCamera();
	void lookAt(Vector3f eye, Vector3f center, Vector3f up);
	void ortho(float left, float right, float bottom, float top, float nearp, float farp);
	void perspective(float fov, float ratio, float nearp, float farp);
};
