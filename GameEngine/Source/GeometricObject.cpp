#include "GeometricObject.h"
#include "Matrix4f.h"
#include "Vector4f.h"
#include "MatrixFactory.h"
#include "Mesh.h"

//used by inheriting object
GeometricObject::GeometricObject(BufferObjects* buffer, Scene* scene)
{
	this->bufferObjects = buffer;
	this->scene = scene;
	VboId = bufferObjects->getVboId();
	VaoId = bufferObjects->getVaoId();

	calculateTangents();

	//standard color and lighting values
	GLfloat * emissive = new GLfloat[4]{ 0.0f,0.0f,0.0f,0.0f };
	memcpy(MaterialColors.EMISSIVE, emissive, 4 * sizeof(GLfloat));
	this->materialShininess = 0.3f;

	transformations = MatrixFactory::Identity4();
}

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

	//standard color and lighting values
	GLfloat * emissive = new GLfloat[4]{ 0.0f,0.0f,0.0f,0.0f };
	memcpy(MaterialColors.EMISSIVE, emissive, 4 * sizeof(GLfloat));
	this->materialShininess = 0.3f;

	calculateTangents();
	updateBuffer();
}

void GeometricObject::draw(Matrix4f parentNodeTransformations, Texture* texture, Texture* normalMap)
{
	scene->draw(indicesCount, VaoId, parentNodeTransformations * transformations, MaterialColors, materialShininess, texture, normalMap);
}

void GeometricObject::updateBuffer()
{
	bufferObjects->createBufferObjects(VboId, VaoId, Vertices, Indices, Normals, Texcoords, Tangents);
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
		memcpy(ambientToUse, pinkAmbient, sizeof(GLfloat) * 4);
		memcpy(diffuseToUse, pinkDiffuse, sizeof(GLfloat) * 4);
		memcpy(specularToUse, pinkSpecular, sizeof(GLfloat) * 4);
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

	delete(ambientToUse);
	delete(diffuseToUse);
	delete(specularToUse);
	updateBuffer();
}

void GeometricObject::changeEmmissiveColor(Color color)
{
	GLfloat * emissiveToUse = new GLfloat[4]{ 1.0,1.0,1.0,0.0 };

	switch (color)
	{
	case RED:
		memcpy(emissiveToUse, redSpecular, sizeof(GLfloat) * 4);
		break;
	case GREEN:
		memcpy(emissiveToUse, greenSpecular, sizeof(GLfloat) * 4);
		break;
	case BLUE:
		memcpy(emissiveToUse, blueSpecular, sizeof(GLfloat) * 4);
		break;
	case GREY:
		memcpy(emissiveToUse, greySpecular, sizeof(GLfloat) * 4);
		break;
	case ORANGE:
		memcpy(emissiveToUse, orangeSpecular, sizeof(GLfloat) * 4);
		break;
	case PINK:
		memcpy(emissiveToUse, pinkSpecular, sizeof(GLfloat) * 4);
		break;
	case YELLOW:
		memcpy(emissiveToUse, yellowSpecular, sizeof(GLfloat) * 4);
		break;
	case PURPLE:
		memcpy(emissiveToUse, purpleSpecular, sizeof(GLfloat) * 4);
		break;
	case BROWN:
		memcpy(emissiveToUse, brownSpecular, sizeof(GLfloat) * 4);
		break;
	case BLACK:
		memcpy(emissiveToUse, blackSpecular, sizeof(GLfloat) * 4);
	case WHITE:
		memcpy(emissiveToUse, whiteSpecular, sizeof(GLfloat) * 4);
		break;
	default: break;
	}

	memcpy(MaterialColors.EMISSIVE, emissiveToUse, 4 * sizeof(GLfloat));

	delete(emissiveToUse);
	updateBuffer();
}

void GeometricObject::changeShininess(float shininess)
{
	this->materialShininess = shininess;
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

void GeometricObject::calculateTangents() {
	Tangent tempTan;
	tempTan.TXTYTZ[0] = 0.0;
	tempTan.TXTYTZ[1] = 0.0;
	tempTan.TXTYTZ[2] = 0.0;
	for (int i = 0; i < indicesCount; i++) {
		this->Tangents.push_back(tempTan);
	}

	//create tangents for normal mapping
	if (Texcoords.size() != 0) {
		for (int i = 0; i < indicesCount; i += 3) {
			Vertex v0 = Vertices[Indices[i]];
			Vertex v1 = Vertices[Indices[i + 1]];
			Vertex v2 = Vertices[Indices[i + 2]];

			Texcoord t0 = Texcoords[Indices[i]];
			Texcoord t1 = Texcoords[Indices[i + 1]];
			Texcoord t2 = Texcoords[Indices[i + 2]];

			GLfloat Edge1[3];
			GLfloat Edge2[3];

			for (int j = 0; j < 3; j++) {
				Edge1[j] = v1.XYZW[j] - v0.XYZW[j];
				Edge2[j] = v2.XYZW[j] - v0.XYZW[j];
			}

			float DeltaU1 = t1.UV[0] - t0.UV[0];
			float DeltaV1 = t1.UV[1] - t0.UV[1];
			float DeltaU2 = t2.UV[0] - t0.UV[0];
			float DeltaV2 = t2.UV[1] - t0.UV[1];

			float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

			GLfloat Tangent[3];

			Tangent[0] = f * (DeltaV2 * Edge1[0] - DeltaV1 * Edge2[0]);
			Tangent[1] = f * (DeltaV2 * Edge1[1] - DeltaV1 * Edge2[1]);
			Tangent[2] = f * (DeltaV2 * Edge1[2] - DeltaV1 * Edge2[2]);

			for (int j = 0; j < 3; j++) {
				this->Tangents[Indices[i]].TXTYTZ[j] += Tangent[j];
				this->Tangents[Indices[i + 1]].TXTYTZ[j] += Tangent[j];
				this->Tangents[Indices[i + 2]].TXTYTZ[j] += Tangent[j];
			}
		}

		for (int i = 0; i < Vertices.size(); i++) {
			Vector3f tangent((float)Tangents[i].TXTYTZ[0], (float)Tangents[i].TXTYTZ[1], (float)Tangents[i].TXTYTZ[2]);
			tangent.Normalize();
			Tangents[i].TXTYTZ[0] = (GLfloat)tangent.x;
			Tangents[i].TXTYTZ[1] = (GLfloat)tangent.y;
			Tangents[i].TXTYTZ[2] = (GLfloat)tangent.z;
		}
	}
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