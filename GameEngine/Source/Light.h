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
	float coneFalloffAngle;
	Vector4f coneDirection;
	float ambientCoefficient;
	Shader * shader;
	LightType lightType;
	Light(Shader * shader, LightType type);
	void setLightShaderValues();
};
