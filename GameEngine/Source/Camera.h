#pragma once
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "Vector3f.h"
#include "BufferObjects.h"
#include "Quaternions.h"
#include <stdlib.h>

class Scene;

class Camera
{
private:
	Matrix4f View;
	Matrix4f Projection;
	GLuint *cameraVboId;
	BufferObjects * bufferObjects;
	Scene * scene;
	Shader* shader;
	Vector3f initialPitch;
	Vector3f initialYaw;
	bool CameraChanged;
	void updateShaderCameraPosition(Matrix4f rotation, Vector3f eye);
public:
	Camera(BufferObjects* buffer, Scene* scene, Shader * shader);
	void updateCamera();
	void lookAt(Vector3f eye, Vector3f center, Vector3f up);
	void rodriguesLookAt(float rotationX, float rotationY, Vector3f eye, Vector3f center, Vector3f up);
	void quaternionLookAt(float rotationX, float rotationY, float zoom, Vector3f eye, Vector3f center, Vector3f up);
	void ortho(float left, float right, float bottom, float top, float nearp, float farp);
	void perspective(float fov, float ratio, float nearp, float farp);
	Matrix4f getViewMatrix();
	Matrix4f getProjectionMatrix();
};
