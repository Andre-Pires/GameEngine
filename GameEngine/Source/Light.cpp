#include "Light.h"

Light::Light(Shader * shader, int lightIndex, LightType type)
{
	this->shader = shader;
	this->lightType = type;
	this->lightIndex = lightIndex;

	if (lightType == POINT_LIGHT || lightType == SPOTLIGHT)
	{
		this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].lightRange");
	}

	if (lightType == SPOTLIGHT)
	{
		this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].coneAngle");
		this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].coneFalloffAngle");
		this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].coneDirection");
	}

	this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].lightType");

	this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].position");

	this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].ambientColor");

	this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].diffuseColor");

	this->shader->addUniform("sceneLights[" + std::to_string(lightIndex) + "].specularColor");
}

void Light::setLightShaderValues()
{
	shader->useShaderProgram();

	if (this->lightType == POINT_LIGHT || lightType == SPOTLIGHT)
	{
		GLint lightRangeId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].lightRange");
		glUniform1f(lightRangeId, lightRange);
	}
	if (lightType == SPOTLIGHT)
	{
		GLint coneAngleId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].coneAngle");
		GLint coneFalloffAngleId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].coneFalloffAngle");
		GLint coneDirId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].coneDirection");
		glUniform1f(coneAngleId, coneAngle);
		glUniform1f(coneFalloffAngleId, coneFalloffAngle);
		glUniform4fv(coneDirId, 1, coneDirection.getVector());
	}

	GLint lightTypeId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].lightType");
	GLint positionId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].position");
	GLint ambientId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].ambientColor");
	GLint diffuseId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].diffuseColor");
	GLint specularId = shader->getUniformLocation("sceneLights[" + std::to_string(lightIndex) + "].specularColor");

	// Set Values
	glUniform1i(lightTypeId, lightType);
	glUniform4fv(positionId, 1, position.getVector());
	glUniform4fv(ambientId, 1, ambientColor.getVector());
	glUniform4fv(diffuseId, 1, diffuseColor.getVector());
	glUniform4fv(specularId, 1, specularColor.getVector());

	shader->dropShaderProgram();
	shader->checkShaderError("ERROR: Could not create light.");
}