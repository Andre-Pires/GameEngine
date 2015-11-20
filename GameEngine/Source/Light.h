#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "Shader.h"
#include "Matrix4f.h"

class Light
{
private:

public:
	Vector4f ambientColor;
	Vector4f diffuseColor;
	Vector4f specularColor;
	Vector4f position;
	float lightRange;
	float coneAngle;
	float coneFalloffAngle;
	Vector4f coneDirection;
	float ambientCoefficient;
	Shader * shader;
	LightType lightType;
	int lightIndex;
	Light(Shader * shader, int lightIndex, LightType type);
	void setLightShaderValues();
};
