#include "GameObject.h"
#include "Matrix4f.h"
#include "Vector4f.h"
#include "MatrixFactory.h"

GameObject::GameObject(BufferObjects* buffer, Scene* scene, Vertex * Vertices, int verticesSize, GLubyte * Indices, int indicesSize)
{
	hasBeenModified = true;
	this->bufferObjects = buffer;
	this->scene = scene;

	//number of vertices
	this->indicesCount = indicesSize;
	this->verticesCount = verticesSize;

	this->Vertices = new Vertex[verticesSize];
	this->Indices = new GLubyte[indicesSize];

	memcpy(this->Indices, Indices, indicesSize * sizeof(GLubyte));

	this->Vertices = new Vertex[verticesCount];
	this->Indices = new GLubyte[indicesCount];

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, Vertices[i].XYZW, 4 * sizeof(GLfloat));
		memcpy(Vertices[i].RGBA, Vertices[i].RGBA, 4 * sizeof(GLfloat));
	}

	memcpy(this->Indices, Indices, 4 * sizeof(GLubyte));

	VboId = bufferObjects->getVboId();
	VaoId = bufferObjects->getVaoId();
}

//used by inheriting object
GameObject::GameObject(BufferObjects* buffer, Scene* scene)
{
	hasBeenModified = true;
	this->bufferObjects = buffer;
	this->scene = scene;
	VboId = bufferObjects->getVboId();
	VaoId = bufferObjects->getVaoId();
}

void GameObject::draw()
{
	if (hasBeenModified)
	{
		bufferObjects->createBufferObjects(VboId, VaoId, Vertices, verticesCount * sizeof(Vertex), Indices, indicesCount * sizeof(GLubyte));
		hasBeenModified = false;
	}

	scene->draw(indicesCount, VaoId);
}

void GameObject::translate(Vector3f translation)
{
	Vector4f vec;
	Matrix4f translateMat = MatrixFactory::Translation4(translation);
	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, (translateMat * Vector4f(Vertices[i].XYZW)).getVector(), 4 * sizeof(GLfloat));
	}
	hasBeenModified = true;
}

void GameObject::rotate(float angle, Vector3f rotation)
{
	Vector4f vec;
	Matrix4f rotateMat = MatrixFactory::Rotation4(angle, rotation);

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, (rotateMat * Vector4f(Vertices[i].XYZW)).getVector(), 4 * sizeof(GLfloat));
	}
	hasBeenModified = true;
}

void GameObject::scale(Vector3f scale)
{
	Vector4f vec;
	Matrix4f scaleMat = MatrixFactory::Scale4(scale);
	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, (scaleMat * Vector4f(Vertices[i].XYZW)).getVector(), 4 * sizeof(GLfloat));
	}
	hasBeenModified = true;
}

void GameObject::shear(float shearX, float shearY)
{
	Vector4f vec;
	Matrix4f shearMat = MatrixFactory::Shear4(shearX, shearY);
	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].XYZW, (shearMat *Vector4f(Vertices[i].XYZW)).getVector(), 4 * sizeof(GLfloat));
	}
	hasBeenModified = true;
}

void GameObject::changeColor(Color color)
{
	GLfloat * colorToUse = new GLfloat[4]{ 1.0,1.0,1.0,0.0 };

	switch (color)
	{
	case RED:
		memcpy(colorToUse, red, sizeof(GLfloat) * 4);
		break;
	case GREEN:
		memcpy(colorToUse, green, sizeof(GLfloat) * 4);
		break;
	case BLUE:
		memcpy(colorToUse, blue, sizeof(GLfloat) * 4);
		break;
	case GREY:
		memcpy(colorToUse, grey, sizeof(GLfloat) * 4);
		break;
	case ORANGE:
		memcpy(colorToUse, orange, sizeof(GLfloat) * 4);
		break;
	case PINK:
		memcpy(colorToUse, pink, sizeof(GLfloat) * 4);
		break;
	case YELLOW:
		memcpy(colorToUse, yellow, sizeof(GLfloat) * 4);
		break;
	case PURPLE:
		memcpy(colorToUse, purple, sizeof(GLfloat) * 4);
		break;
	case BLACK:
		memcpy(colorToUse, black, sizeof(GLfloat) * 4);
		break;
	default: break;
	}

	for (int i = 0; i < verticesCount; i++)
	{
		memcpy(Vertices[i].RGBA, colorToUse, 4 * sizeof(GLfloat));
	}
	hasBeenModified = true;
}

void GameObject::clearObjectFromBuffer()
{
	bufferObjects->destroyBufferObjects(VboId, VaoId);
}

std::ostream& GameObject::operator<<(std::ostream& stream)
{
	for (int i = 0; i < verticesCount; i++)
	{
		stream << std::endl
			<< "[ "
			<< Vertices[i].XYZW[0] << " ,"
			<< Vertices[i].XYZW[1] << " ,"
			<< Vertices[i].XYZW[2] << " ,"
			<< Vertices[i].XYZW[3] << " ] \n";
	}
	stream << std::endl;
	return stream;
}

std::ostream& operator<< (std::ostream& stream, GameObject object)
{
	for (int i = 0; i < object.verticesCount; i++)
	{
		stream << std::endl
			<< "[ "
			<< object.Vertices[i].XYZW[0] << " ,"
			<< object.Vertices[i].XYZW[1] << " ,"
			<< object.Vertices[i].XYZW[2] << " ,"
			<< object.Vertices[i].XYZW[3] << " ] \n";
	}
	stream << std::endl;
	return stream;
}