#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "Shader.h"

class Light
{
private:

public:
	Vector4f ambientColor;
	Vector4f diffuseColor;
	Vector4f specularColor;
	Vector4f position;
	float attenuation;
	float coneAngle;
	Vector3f coneDirection;
	float ambientCoefficient;
	Shader * shader;
	Light(Vector4f position, Vector4f ambient, Vector4f diffuse, Vector4f specular, Shader* shader);
	Light(Shader * shader);
	void setLightShaderValues();
};
