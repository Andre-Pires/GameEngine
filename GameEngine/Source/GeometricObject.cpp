#include "GeometricObject.h"
#include "Matrix4f.h"
#include "Vector4f.h"
#include "MatrixFactory.h"
#include "Mesh.h"

//TODO: never been used, might have bugs
GeometricObject::GeometricObject(BufferObjects* buffer, Scene* scene, Vertex * Vertices, int verticesSize, GLuint * Indices, int indicesSize)
{
	this->bufferObjects = buffer;
	this->scene = scene;

	//number of vertices
	this->indicesCount = indicesSize;
	this->verticesCount = verticesSize;

	memcpy(this->Indices.data(), Indices, indicesSize * sizeof(GLuint));

	for (int i = 0; i < verticesCount; i++)
	{
		this->Vertices.push_back(Vertices[i]);
	}

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

//TODO: falta testar a questão das texturas e normais
GeometricObject::GeometricObject(BufferObjects* buffer, Scene* scene, Mesh mesh)
{
	this->bufferObjects = buffer;
	this->scene = scene;
	VboId = bufferObjects->getVboId();
	VaoId = bufferObjects->getVaoId();

	transformations = MatrixFactory::Identity4();

	this->Vertices = mesh.Vertices;
	this->verticesCount = mesh.Vertices.size();
	this->Texcoords = mesh.Texcoords;
	this->Normals = mesh.Normals;
	this->indicesCount = mesh.Vertices.size();

	for (int i = 0; i < mesh.Vertices.size(); i++)
	{
		this->Indices.push_back(i);
	}

	changeColor(WHITE);
}

void GeometricObject::draw(Matrix4f parentNodeTransformations, Texture* texture)
{
	scene->draw(indicesCount, VaoId, parentNodeTransformations * transformations, MaterialColors, texture);
}

void GeometricObject::updateBuffer()
{
	bufferObjects->createBufferObjects(VboId, VaoId, Vertices, Indices, Normals, Texcoords);
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
	GLfloat * ambientToUse = new GLfloat[4]{ 1.0,1.0,1.0,0.0 };
	GLfloat * diffuseToUse = new GLfloat[4]{ 1.0,1.0,1.0,0.0 };
	GLfloat * specularToUse = new GLfloat[4]{ 1.0,1.0,1.0,0.0 };

	switch (color)
	{
	case RED:
		memcpy(ambientToUse, redAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, redDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, redSpecular, sizeof(GLfloat) * 4);
		break;
	case GREEN:
		memcpy(ambientToUse, greenAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, greenDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, greenSpecular, sizeof(GLfloat) * 4);
		break;
	case BLUE:
		memcpy(ambientToUse, blueAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, blueDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, blueSpecular, sizeof(GLfloat) * 4);
		break;
	case GREY:
		memcpy(ambientToUse, greyAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, greyDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, greySpecular, sizeof(GLfloat) * 4);
		break;
	case ORANGE:
		memcpy(ambientToUse, orangeAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, orangeDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, orangeSpecular, sizeof(GLfloat) * 4);
		break;
	case PINK:
		memcpy(ambientToUse, yellowAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, yellowDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, yellowSpecular, sizeof(GLfloat) * 4);
		break;
	case YELLOW:
		memcpy(ambientToUse, yellowAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, yellowDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, yellowSpecular, sizeof(GLfloat) * 4);
		break;
	case PURPLE:
		memcpy(ambientToUse, purpleAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, purpleDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, purpleSpecular, sizeof(GLfloat) * 4);
		break;
	case BROWN:
		memcpy(ambientToUse, brownAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, brownDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, brownSpecular, sizeof(GLfloat) * 4);
		break;
	case BLACK:
		memcpy(ambientToUse, blackAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, blackDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, blackSpecular, sizeof(GLfloat) * 4);
	case WHITE:
		memcpy(ambientToUse, whiteAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, whiteDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, whiteSpecular, sizeof(GLfloat) * 4);
		break;
	default: break;
	}

	memcpy(MaterialColors.AMBIENT, ambientToUse, 4 * sizeof(GLfloat));
	memcpy(MaterialColors.DIFFUSE, diffuseToUse, 4 * sizeof(GLfloat));
	memcpy(MaterialColors.SPECULAR, specularToUse, 4 * sizeof(GLfloat));

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

void GeometricObject::repeatTexture(float repeat) {
	for (int i = 0; i < verticesCount; i++)
	{
		this->Texcoords[i].UV[0] *= repeat;
		this->Texcoords[i].UV[1] *= repeat;
	}

	updateBuffer();
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