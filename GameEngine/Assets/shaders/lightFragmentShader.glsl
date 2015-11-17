#version 330 core
// In
in vec4 ex_Position;
in vec4 ex_Color;
in vec4 ex_Normal;

// Out
out vec4 out_Color;


// array of lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
   int lightType; // 0 = point light, 1 = spotlight, 2 = directional light
   vec4 position;
   vec4 ambientColor;
   vec4 diffuseColor;
   vec4 specularColor;
   float lightRange;
   float coneAngle;
   float coneFalloffAngle;
   vec4 coneDirection;
} sceneLights[MAX_LIGHTS];

uniform vec4 eyeDirection;


// Matrix
uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float shininess = 10.0;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space


vec4 calculateLight(Light light){

	vec4 lightDir;
	float spot = 1.0;
	float attenuation = 1.0f;

	if(light.lightType == 2) // directional light
	{
		lightDir = normalize(vec4(light.position.xyz, 1.0));
	}
	else // point or spot light
	{
		lightDir = normalize(light.position - ex_Position);

		float distanceToLight = length(light.position - ex_Position);

		attenuation = 1.0 / (1.0 - pow(distanceToLight / light.lightRange, 2));

		if(light.lightType == 1){ //spotlight

			float lightToSurfaceAngle = degrees(acos(dot(-lightDir, normalize(light.coneDirection))));

			float innerConeAngle = light.coneAngle;
			float outerConeAngle = light.coneAngle + light.coneFalloffAngle;

			float falloffAngle = innerConeAngle - outerConeAngle;

			spot = clamp((lightToSurfaceAngle - outerConeAngle) / falloffAngle, 0.0, 1.0);


			//NOTE: allows to see the spotlight soft edges really clearly
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

	return light.ambientColor + spot *  attenuation * (lambertian * light.diffuseColor + specular * light.specularColor);
}


void main(void)
{
	vec4 lightColorResult = vec4(0.0);

	//adds the different lights' contribution for resulting color
	for(int i = 0; i < numLights; i++)
	{
		lightColorResult += calculateLight(sceneLights[i]);
	}

	vec4 lightColorGammaCorrected = pow(lightColorResult, vec4(1.0/screenGamma));

	out_Color = ex_Color * lightColorGammaCorrected;
	//out_Color = abs(ex_Normal);
}
