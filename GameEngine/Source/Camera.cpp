#include "Camera.h"

Camera::Camera(BufferObjects* buffer, Scene* scene, Shader * shader)
{
	this->bufferObjects = buffer;
	this->scene = scene;
	this->shader = shader;
	this->cameraVboId = bufferObjects->getCameraVboId();
	bufferObjects->createCameraBufferObjects(cameraVboId);

	this->View = MatrixFactory::Identity4();
	this->Projection = MatrixFactory::Identity4();

	CameraChanged = true;
}

void Camera::updateCamera()
{
	if (CameraChanged)
	{
		bufferObjects->updateCamera(cameraVboId, View, Projection);
	}
}

void Camera::lookAt(Vector3f eye, Vector3f center, Vector3f up)
{
	Vector3f view = center - eye;

	view = view.Normalize();

	Vector3f side = view.Cross(up);

	side = side.Normalize();

	Vector3f u = side.Cross(view);

	//the minuses are due to opengl's norm of an inverted z
	Matrix4f rotation = Matrix4f(
		new float[16]{
		side.x, u.x, -view.x, 0,
		side.y, u.y, -view.y, 0,
		side.z, u.z, -view.z, 0,
		0, 0, 0, 1
	});

	Matrix4f translation = MatrixFactory::Translation4(-eye);

	this->View = rotation * translation;

	CameraChanged = true;
}

void Camera::rodriguesLookAt(float rotationX, float rotationY, Vector3f eye, Vector3f center, Vector3f up)
{
	Vector3f view = center - eye;

	view = view.Normalize();

	Vector3f side = view.Cross(up);

	side = side.Normalize();

	Vector3f u = side.Cross(view);

	Matrix4f rotationMatX = MatrixFactory::Rotation4(rotationY, side);

	Matrix4f rotationMatY = MatrixFactory::Rotation4(-rotationX, u);

	Matrix4f rotation = rotationMatX * rotationMatY;

	Matrix4f translation = MatrixFactory::Translation4(-eye);

	this->View = translation * rotation;

	CameraChanged = true;
}

void Camera::quaternionLookAt(float rotationX, float rotationY, float zoom, Vector3f eye, Vector3f center, Vector3f up)
{
	Vector3f view = center - eye;

	view = view.Normalize();

	Vector3f side = view.Cross(up);

	side = side.Normalize();

	Vector3f u = side.Cross(view);

	Quaternion verticalRotation = Quaternion::FromAngleAxis(rotationY, side);

	Quaternion horizontalRotation = Quaternion::FromAngleAxis(-rotationX, u);

	Quaternion rotation = verticalRotation * horizontalRotation;

	Matrix4f translation = MatrixFactory::Translation4(-Vector3f(0, 0, zoom + eye.z));

	this->View = translation * rotation.ToMatrix4();

	CameraChanged = true;
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

	CameraChanged = true;
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

	CameraChanged = true;
}

Matrix4f Camera::getViewMatrix()
{
	return this->View;
}

void Camera::updateEyeDirection(Vector3f center, Vector3f eye)
{
	GLint id = this->shader->getUniformLocation("eyeDirection");

	shader->useShaderProgram();

	Vector4f eyeDirection = (center - eye).Normalize().toVector4();

	glUniform4fv(id, 1, eyeDirection.getVector());

	shader->dropShaderProgram();
}