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

// Spotlight Attributes
uniform float coneAngle;
uniform float coneFalloffAngle;
uniform vec4 coneDirection;
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
	float spot = 1.0;
	vec4 lightDir;
	float attenuation = 1.0f;

	if(lightPosition.w == 0.0) // directional light
	{
		lightDir = normalize(vec4(lightPosition.xyz, 1.0));
		attenuation = 1.0f; // directional lights don't fade
	}
	else // point or spot light
	{
		lightDir = normalize(lightPosition - ex_Position);
		float distanceToLight = length(lightPosition - ex_Position);

		attenuation = 1.0 / (1.0 - pow(distanceToLight / attenuationFactor, 2));

		//cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-lightDir, normalize(coneDirection))));


        if(lightToSurfaceAngle > coneAngle){
            attenuation = 0.0;
        }

			float innerConeAngle = coneAngle;
			float outerConeAngle = coneAngle + coneFalloffAngle;

			float falloffAngle = innerConeAngle - outerConeAngle;

			spot = clamp((lightToSurfaceAngle - outerConeAngle) / falloffAngle, 0.0, 1.0);


			//NOTE: allows to see the spotlight softedges really clearly
			/*if(spot > 0.7){
				out_Color = vec4(1.0,0.0,0.0,1.0);
				return;
			}else if(spot > 0.4){
				out_Color = vec4(0,1.0,0.0,1.0);
				return;
			}else{
				out_Color = vec4(0,0.0,1.0,1.0);
				return;
			}*/

	}

	float lambertian =	max(dot(lightDir, ex_Normal), 0.0);
	float specular = 0.0;

	if(lambertian > 0.0)
	{

		vec4 viewDir = normalize(-ex_Position);

		//Blinn-Phong
		vec4 halfDir = normalize(lightDir + viewDir);

		float specAngle = max(dot(halfDir, ex_Normal), 0.0);

		specular = pow(specAngle, shininess);
	}

	vec4 colorLinear = ambientColor + spot * /* attenuation * */ (lambertian * diffuseColor + specular * specularColor);

	vec4 lightColorGammaCorrected = pow(colorLinear, vec4(1.0/screenGamma));

	out_Color = ex_Color * lightColorGammaCorrected;
	//out_Color = abs(ex_Normal);
}
