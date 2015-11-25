#version 330 core
// In
in vec4 ex_Position;
in vec4 ex_Color;
in vec4 ex_Normal;
in vec4 ex_shadowCoord;
in vec2 ex_UV;

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

// Material Attributes
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;

// Texture Samplers
uniform sampler2D TextureSampler;

uniform vec4 cameraPosition;

// Matrix
uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


//shadow uniform
uniform sampler2DShadow shadowMap;

float shininess;
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

		float distanceToLight = length((light.position) - ex_Position);

		attenuation = 1.0 / (1.0 - pow(distanceToLight / light.lightRange, 2));

		if(light.lightType == 1){ //spotlight

			float lightToSurfaceAngle = degrees(acos(dot(-lightDir, normalize(light.coneDirection))));

			float innerConeAngle = light.coneAngle;
			float outerConeAngle = light.coneAngle + light.coneFalloffAngle;

			float falloffAngle = innerConeAngle - outerConeAngle;
			spot = clamp((lightToSurfaceAngle - outerConeAngle) / falloffAngle, 0.0, 1.0);


			//NOTE: allows to see the spotlight soft edges really clearly
			/*if(spot > 0.7){
				return vec4(1.0,0.0,0.0,1.0);
			}else if(spot > 0.4){
				return vec4(0,1.0,0.0,1.0);
			}else{
				return vec4(0,0.0,1.0,1.0);
			}*/
            //return mix(vec4(0.9,0.0,0.0,0.0),vec4(0.0,0.9,0.0,0.0), spot);
		}
	}

	float diffuseCoefficient =	max(dot(lightDir, ex_Normal), 0.0);
	float specularCoefficient = 0.0;

	if(diffuseCoefficient > 0.0 )
	{
		vec4 viewDir = normalize(cameraPosition - ex_Position);
		vec4 halfDir = normalize(lightDir + viewDir);

        //Blinn-Phong
        //shininess = 16.0;
		//float specAngle = max(dot(halfDir, ex_Normal), 0.0);
		//specularCoefficient = pow(specAngle, shininess);

        //Gaussian
        shininess = 0.8;
        float dotNH = clamp(dot(halfDir, ex_Normal), 0.0, 1.0);
        float angle = acos(dotNH);
        specularCoefficient = exp(- pow (angle/ shininess, 2));
	}

	return light.ambientColor * materialAmbient + spot *  attenuation * (diffuseCoefficient * light.diffuseColor * materialDiffuse + specularCoefficient * light.specularColor * materialSpecular);
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

    //TODO: testing shadows

	float shadow = texture(shadowMap,vec3(ex_shadowCoord.xy, (ex_shadowCoord.z)/ex_shadowCoord.w));

    //out_Color = vec4(lightColorGammaCorrected * shadow);
    //TODO: testing shadows

	out_Color = texture( TextureSampler, ex_UV ) * lightColorGammaCorrected;
    //out_Color = materialAmbient;
	//out_Color = abs(ex_Normal);
}
