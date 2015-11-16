#version 330 core
// In
in vec4 ex_Position;
in vec4 ex_Color;
in vec4 ex_Normal;

// Out
out vec4 out_Color;

// Light Attributes
uniform vec4 lightPosition;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float attenuationFactor;

// Camera View Vector
uniform vec4 eyeDirection;


// Matrix
uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float shininess = 16.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

void main(void)
{
	vec4 lightDir = normalize(lightPosition - ex_Position);

	float lambertian =	max(dot(lightDir, ex_Normal), 0.0);
	float specular = 0.0;

	if(lambertian > 0.0){

		vec4 viewDir = normalize(-ex_Position);

		//Blinn Phong
		vec4 halfDir = normalize(lightDir + viewDir);

		float specAngle = max(dot(halfDir, ex_Normal), 0.0);

		specular = pow(specAngle, shininess);
	}

	vec4 colorLinear = ambientColor + lambertian * diffuseColor + specular * specularColor;

	vec4 lightColorGammaCorrected = pow(colorLinear, vec4(1.0/screenGamma));

	out_Color = ex_Color * lightColorGammaCorrected;
	//out_Color = abs(ex_Normal);
}
