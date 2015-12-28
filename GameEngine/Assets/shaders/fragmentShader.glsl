#version 330 core
// In
in vec4 ex_Position;
in vec4 ex_Color;
in vec3 ex_Normal;
in vec4 ex_shadowCoord[2];
in vec2 ex_UV;
in vec3 ex_Tangent;
in vec4 mcPosition;

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
uniform float materialShininess;

// Texture Samplers
uniform sampler2D TextureSampler;
uniform sampler3D WoodSampler;
uniform int textureActive;
uniform int woodTextureActive;

// Normal Map Samplers
uniform sampler2D NormalMapSampler;
vec3 mappedNormal;

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
uniform sampler2DShadow shadowMap[2];


const float screenGamma = 2.2; // Assume the monitor is calibrated to the sRGB color space

//For wood texture
const float NoiseScale = 0.1;
const float Noisiness = 0.1;
const vec3 LightWood = vec3(0.9, 0.54, 0.22);
const vec3 DarkWood = vec3(0.43, 0.25, 0.09);
const float RingFrequency = 12.0;
const float RingSharpness = 21;
const float RingScale = 1.5;
const float GrainScale = 0.5;
const float GrainThreshold = 0.05;
const float LightGrains = 0.8;
const float DarkGrains = 0.2;

layout(location = 0) out vec4 color;

float chebyshevUpperBound(vec4 shadowCoordW, int lightIndex)
{

    float distance = shadowCoordW.z;

    float moments = textureProj(shadowMap[lightIndex],shadowCoordW);

	// Surface is fully lit. as the current fragment is before the light occluder
	if (distance <= moments)
		return 1.0 ;

	float variance = (moments * moments);
	variance = max(variance,0.00002); //NOTE: this variance looks good in my case

	float d = distance - moments;
	float p_max = variance / (variance + d*d);

	return p_max;
}

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float calculateShadow(int lightIndex){

    float visibility=1.0;
    float bias = 0.0003;

    for (int i=0;i<6;i++){
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

        //NOTE: acho que serve assim, tenho de estudar pq é que isto funciona e descrever depois na dicsussao
        vec4 shadowCoordW = ex_shadowCoord[lightIndex] / ex_shadowCoord[lightIndex].w;

        shadowCoordW.xy += poissonDisk[index]/700.0;
        shadowCoordW.z -= bias;
        visibility -= 0.15*(1.0- chebyshevUpperBound(shadowCoordW, lightIndex));
    }

    if(visibility < 1.0){
        return visibility;
    }else{
        return 1.0;
    }
}

vec4 calculateLight(int index){

    Light light = sceneLights[index];
	vec3 lightDir;
	float spot = 1.0;
	float attenuation = 1.0f;

	if(light.lightType == 2) // directional light
	{
		lightDir = normalize(light.position.xyz);
	}
	else // point or spot light
	{
		lightDir = normalize(vec3(ViewMatrix *light.position) - ex_Position.xyz);

		float distanceToLight = length(vec3(ViewMatrix *light.position) - ex_Position.xyz);

        if(distanceToLight < light.lightRange)
            attenuation = 1.0 - pow(distanceToLight / light.lightRange, 2);
        else
            attenuation = 0.0;

		if(light.lightType == 1){ //spotlight

            vec3 lightDirWorld = normalize(light.position.xyz - vec3(inverse(ViewMatrix) *ex_Position));
			float lightToSurfaceAngle = degrees(acos(dot(- lightDirWorld, normalize(light.coneDirection.xyz))));

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

	float diffuseCoefficient =	clamp(dot(lightDir, mappedNormal.xyz), 0.0, 1.0);
	float specularCoefficient = 0.0;

	if(diffuseCoefficient > 0.0 )
	{
		vec3 viewDir = normalize( -ex_Position.xyz);
		vec3 halfDir = normalize(lightDir  + viewDir);

        //Blinn-Phong
        //materialShininess = 32.0;
		//float specAngle = max(dot(halfDir, mappedNormal), 0.0);
		//specularCoefficient = pow(specAngle, materialShininess);

        //Gaussian
        //materialShininess = 0.3;
        float NdotH = clamp(dot(mappedNormal, halfDir), 0.0, 1.0);
        float angle = acos(NdotH);
        specularCoefficient = exp(- pow (angle/ materialShininess, 2));
	}

    //return mix(vec4(0.9,0.0,0.0,1.0),vec4(0.0,0.9,0.0,1.0), specularCoefficient);

    //NOTE: removed the specular contribution from the material (made the highlight hard to see)
	vec4 lightIntensity = light.ambientColor * materialAmbient + spot  *  attenuation * (diffuseCoefficient * light.diffuseColor * materialDiffuse + specularCoefficient * light.specularColor);

    if(spot == 0 || attenuation == 0){
        return lightIntensity;
    }else{
        return lightIntensity * calculateShadow(index);
    }

}

//calculate new normal
vec3 CalculateBumpedNormal()
{
    vec3 Normal = normalize(ex_Normal.xyz);
    vec3 Tangent = normalize(ex_Tangent);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(NormalMapSampler, ex_UV).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main(void)
{
    if(textureActive == 1){
        mappedNormal = CalculateBumpedNormal();
    } else {
        mappedNormal = ex_Normal;
    }

	vec4 lightColorResult = vec4(0.0);
    vec4 colorResult = vec4(0.0);
	//adds the different lights' contribution for the resulting color
	for(int i = 0; i < numLights; i++)
	{
		lightColorResult += calculateLight(i);
	}

	lightColorResult = pow(lightColorResult, vec4(1.0/screenGamma));

    //if we have textures add them
    if(textureActive == 1){
        //if the texture is wood
        if(woodTextureActive == 1){
          vec4 temp = texture(WoodSampler, mcPosition.xyz * NoiseScale) * Noisiness;
          vec3 noisevec = temp.rgb;
          vec3 location = mcPosition.xyz + noisevec;

          float dist = sqrt(location.x * location.x + location.z * location.z);
          dist *= RingFrequency;

          float r = fract(dist + noisevec[0] + noisevec[1] + noisevec[2]) * 2.0;

          if (r > 1.0){
            r = 2.0 - r;
          }
          vec3 colorTemp = mix(LightWood, DarkWood, r);

          r = fract((mcPosition.x + mcPosition.z) * GrainScale + 0.5);
          noisevec[2] *= r;
          if (r < GrainThreshold){
            colorTemp += LightWood * LightGrains * noisevec[2];
          }else{
            colorTemp -= LightWood * DarkGrains * noisevec[2];
          }
          colorResult = vec4(colorTemp, 1.0) * lightColorResult;

        }else{
          colorResult = texture( TextureSampler, ex_UV ) * lightColorResult;
        }
    }else{
        colorResult = lightColorResult;
    }

    color = colorResult;
}
