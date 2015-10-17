#include "Camera.h"

Camera::Camera(BufferObjects* buffer, Scene* scene)
{
	this->bufferObjects = buffer;
	this->scene = scene;
	this->cameraVboId = bufferObjects->getCameraVboId();
	bufferObjects->createCameraBufferObjects(cameraVboId);

	this->View = MatrixFactory::Identity4();
	this->Projection = MatrixFactory::Identity4();
}

void Camera::updateCamera()
{
	bufferObjects->updateCamera(cameraVboId, View, Projection);
}

void Camera::lookAt(Vector3f eye, Vector3f center, Vector3f up)
{
	Vector3f view = center - eye;

	view = view.Normalize();

	Vector3f side = view.Cross(up);

	side = side.Normalize();

	Vector3f u = side.Cross(view);

	// the minuses are due to opengl's norm of an inverted z
	Matrix4f rotation = Matrix4f(
		new float[16]{
		side.x, u.x, -view.x, 0,
		side.y, u.y, -view.y, 0,
		side.z, u.z, -view.z, 0,
		0, 0, 0, 1
	});

	Matrix4f translation = MatrixFactory::Translation4(-eye);

	this->View = rotation * translation;
}

//clears the view matrix when running
void Camera::ortho(float left, float right, float bottom, float top, float nearp, float farp)
{
	this->View = MatrixFactory::Identity4();

	Vector3f translateToOrigin = Vector3f(-(left + right) / 2, -(top + bottom) / 2, -(nearp + farp) / 2);

	Vector3f scaleToNormalized = Vector3f(2 / (right - left), 2 / (top - bottom), 2 / (farp - nearp));

	Matrix4f translation = MatrixFactory::Translation4(translateToOrigin);

	Matrix4f scale = MatrixFactory::Scale4(scaleToNormalized);

	// the 3,3 field minus is due to opengl's norm
	Matrix4f result = scale * translation;
	result.setValue(3, 3, -result(3, 3));

	this->Projection = result;
}

// maximum angle possible for the fov is 180
void Camera::perspective(float fov, float ratio, float nearp, float farp)
{
	float radians = (fov * PI) / 180;

	auto d = 1 / tan(radians / 2);

	this->Projection = Matrix4f(

		new float[16]{
		d / ratio, 0, 0, 0,
		0, d, 0, 0,
		0, 0, (nearp + farp) / (nearp - farp), -1,
		0, 0, (2 * farp * nearp) / (nearp - farp), 0
	});
}