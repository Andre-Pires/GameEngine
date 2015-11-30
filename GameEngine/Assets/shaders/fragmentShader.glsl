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
uniform int textureActive;

uniform vec4 cameraPosition;

// Matrix
uniform mat4 ModelMatrix;
uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

vec2 poissonDisk[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

//shadow uniform
uniform sampler2DShadow shadowMap;

float shininess;
const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

vec4 calculateLight(Light light){

	vec4 lightDir;
	float spot = 1.0;
	float attenuation = 1.0f;

	if(light.lightType == 2) // directional light
	{
		lightDir = normalize(light.position);
	}
	else // point or spot light
	{
		lightDir = normalize(ViewMatrix *light.position - ex_Position);

		float distanceToLight = length(light.position - ex_Position);

        if(distanceToLight < light.lightRange)
            attenuation = 1.0 - pow(distanceToLight / light.lightRange, 2);
        else
            attenuation = 0.0;

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

	float diffuseCoefficient =	clamp(dot(lightDir, ex_Normal), 0.0, 1.0);
	float specularCoefficient = 0.0;

	if(diffuseCoefficient > 0.0 )
	{
		vec4 viewDir = normalize(-ex_Position);
		vec4 halfDir = normalize(lightDir  + viewDir);

        //Blinn-Phong
        //shininess = 4.0;
		//float specAngle = max(dot(halfDir, ex_Normal), 0.0);
		//specularCoefficient = pow(specAngle, shininess);

        //Gaussian
        shininess = 0.9;
        float NdotH = clamp(dot(ex_Normal, halfDir), 0.0, 1.0);
        float angle = acos(NdotH);
        specularCoefficient = exp(- pow (angle/ shininess, 2));
	}

    //return mix(vec4(0.9,0.0,0.0,0.0),vec4(0.0,0.9,0.0,0.0), specularCoefficient);

	return light.ambientColor * materialAmbient + spot  *  attenuation * (diffuseCoefficient * light.diffuseColor * materialDiffuse + specularCoefficient * light.specularColor * materialSpecular);
}


void main(void)
{
	vec4 lightColorResult = vec4(0.0);
    vec4 colorResult = vec4(0.0);
	//adds the different lights' contribution for resulting color
	for(int i = 0; i < numLights; i++)
	{
		lightColorResult += calculateLight(sceneLights[i]);
	}

	lightColorResult = pow(lightColorResult, vec4(1.0/screenGamma));

    //if we have textures add them
    if(textureActive == 1){
        colorResult = texture( TextureSampler, ex_UV ) * lightColorResult;
    }
    else{
        colorResult = lightColorResult;
    }

    //TODO: testing shadows

    float visibility=1.0;
	float bias = 0.0025;

    // Sample the shadow map 4 times
	for (int i=0;i<4;i++){
		// use either :
		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
		int index = i;
		//  - A random sample, based on the pixel's screen location.
		//    No banding, but the shadow moves with the camera, which looks weird.
		// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		//  - A random sample, based on the pixel's position in world space.
		//    The position is rounded to the millimeter to avoid too much aliasing
		// int index = int(16.0*random(floor(ex_Position.xyz*1000.0), i))%16;

		// being fully in the shadow will eat up 4*0.2 = 0.8
		// 0.2 potentially remain, which is quite dark.
		visibility -= 0.2*(1.0-textureProj( shadowMap, vec4(ex_shadowCoord.xy + poissonDisk[index]/700.0,  ex_shadowCoord.z-bias, ex_shadowCoord.w) ));
	}

    if(visibility < 1.0){
        colorResult *= visibility;
        //out_Color = mix(vec4(1.0,0.0,0.0,1.0), vec4(0.0,0.0,1.0,1.0), shadow);
    }

    out_Color = colorResult;


    //TODO: testing shadows
	//out_Color = abs(ex_Normal);
}
