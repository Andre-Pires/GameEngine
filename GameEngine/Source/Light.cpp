#include "Light.h"

Light::Light(Shader * shader, LightType type)
{
	this->shader = shader;
	this->lightType = type;
	//this->shader->useShaderProgram();

	if (lightType == POINT_LIGHT || lightType == SPOTLIGHT)
	{
		this->shader->addUniform("attenuationFactor");
	}

	if (lightType == SPOTLIGHT)
	{
		this->shader->addUniform("coneAngle");
		this->shader->addUniform("coneFalloffAngle");
		this->shader->addUniform("coneDirection");
	}

	this->shader->addUniform("lightPosition");
	this->shader->addUniform("ambientColor");
	this->shader->addUniform("diffuseColor");
	this->shader->addUniform("specularColor");

	//this->shader->dropShaderProgram();
}

void Light::setLightShaderValues()
{
	shader->useShaderProgram();

	if (this->lightType == POINT_LIGHT || lightType == SPOTLIGHT)
	{
		GLint attenuationId = shader->getUniformLocation("attenuationFactor");
		glUniform1f(attenuationId, attenuation);
	}
	if (lightType == SPOTLIGHT)
	{
		GLint coneAngleId = shader->getUniformLocation("coneAngle");
		GLint coneFalloffAngleId = shader->getUniformLocation("coneFalloffAngle");
		GLint coneDirId = shader->getUniformLocation("coneDirection");
		glUniform1f(coneAngleId, coneAngle);
		glUniform1f(coneFalloffAngleId, coneFalloffAngle);
		glUniform4fv(coneDirId, 1, coneDirection.getVector());
	}
	GLint positionId = shader->getUniformLocation("lightPosition");
	GLint ambientId = shader->getUniformLocation("ambientColor");
	GLint diffuseId = shader->getUniformLocation("diffuseColor");
	GLint specularId = shader->getUniformLocation("specularColor");

	// Set Values
	glUniform4fv(positionId, 1, position.getVector());
	glUniform4fv(ambientId, 1, ambientColor.getVector());
	glUniform4fv(diffuseId, 1, diffuseColor.getVector());
	glUniform4fv(specularId, 1, specularColor.getVector());

	shader->dropShaderProgram();
	shader->checkShaderError("ERROR: Could not create light.");
}