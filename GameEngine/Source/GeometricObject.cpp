#include "GeometricObject.h"
#include "Matrix4f.h"
#include "Vector4f.h"
#include "MatrixFactory.h"

GeometricObject::GeometricObject(BufferObjects* buffer, Scene* scene, Vertex * Vertices, int verticesSize, GLubyte * Indices, int indicesSize)
{
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

	updateBuffer();
}

//used by inheriting object
GeometricObject::GeometricObject(BufferObjects* buffer, Scene* scene)
{
	this->bufferObjects = buffer;
	this->scene = scene;
	VboId = bufferObjects->getVboId();
	VaoId = bufferObjects->getVaoId();

	transformations = MatrixFactory::Identity4();
}

void GeometricObject::draw(Matrix4f parentNodeTransformations)
{
	scene->draw(indicesCount, VaoId, parentNodeTransformations * transformations);
}

void GeometricObject::updateBuffer()
{
	bufferObjects->createBufferObjects(VboId, VaoId, Vertices, verticesCount * sizeof(Vertex), Indices, indicesCount * sizeof(GLubyte));
}

void GeometricObject::translate(Vector3f translation)
{
	Vector4f vec;
	Matrix4f translateMat = MatrixFactory::Translation4(translation);

	transformations = translateMat * transformations;
}

void GeometricObject::rotate(float angle, Vector3f rotation)
{
	Vector4f vec;
	Matrix4f rotateMat = MatrixFactory::Rotation4(angle, rotation);

	transformations = rotateMat * transformations;
}

void GeometricObject::scale(Vector3f scale)
{
	Vector4f vec;
	Matrix4f scaleMat = MatrixFactory::Scale4(scale);
	transformations = scaleMat * transformations;
}

void GeometricObject::shear(float shearX, float shearY)
{
	Vector4f vec;
	Matrix4f shearMat = MatrixFactory::Shear4(shearX, shearY);
	transformations = shearMat * transformations;
}

void GeometricObject::changeColor(Color color)
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
	case BROWN:
		memcpy(colorToUse, brown, sizeof(GLfloat) * 4);
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

	updateBuffer();
}

void GeometricObject::clearObjectFromBuffer()
{
	bufferObjects->destroyBufferObjects(VboId, VaoId);
}

void GeometricObject::shadeColor()
{
	//not implemented
	return;
}

std::ostream& GeometricObject::operator<<(std::ostream& stream)
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

std::ostream& operator<< (std::ostream& stream, GeometricObject object)
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