#include "Light.h"

Light::Light(Vector4f position, Vector4f ambient, Vector4f diffuse, Vector4f specular, Shader * shader)
{
	this->shader = shader;
	this->shader->addUniform("lightPosition");
	this->shader->addUniform("ambientColor");
	this->shader->addUniform("diffuseColor");
	this->shader->addUniform("specularColor");
	this->shader->addUniform("attenuationFactor");
	this->position = position;
	this->ambientColor = ambient;
	this->diffuseColor = diffuse;
	this->specularColor = specular;
}

Light::Light(Shader * shader)
{
	this->shader = shader;

	//this->shader->useShaderProgram();

	this->shader->addUniform("lightPosition");
	this->shader->addUniform("ambientColor");
	this->shader->addUniform("diffuseColor");
	this->shader->addUniform("specularColor");
	this->shader->addUniform("attenuationFactor");

	//this->shader->dropShaderProgram();
}

void Light::setLightShaderValues()
{
	GLint positionId = shader->getUniformLocation("lightPosition");
	GLint ambientId = shader->getUniformLocation("ambientColor");
	GLint diffuseId = shader->getUniformLocation("diffuseColor");
	GLint specularId = shader->getUniformLocation("specularColor");
	GLint attenuationId = shader->getUniformLocation("attenuationFactor");

	shader->useShaderProgram();

	//	std::cout << "pos " << position << std::endl;
	//	std::cout << "amb " << ambientColor << std::endl;
	//	std::cout << "dif " << diffuseColor << std::endl;
	//	std::cout << "spec " << specularColor << std::endl;

	// Set Values
	glUniform4fv(positionId, 1, position.getVector());
	glUniform4fv(ambientId, 1, ambientColor.getVector());
	glUniform4fv(diffuseId, 1, diffuseColor.getVector());
	glUniform4fv(specularId, 1, specularColor.getVector());
	//glUniform1f(attenuationId, attenuation);

	shader->dropShaderProgram();
	shader->checkShaderError("ERROR: Could not create light.");
}