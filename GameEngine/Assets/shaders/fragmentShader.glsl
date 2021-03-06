#version 330 core
// In
in vec4 ex_Position;
in vec3 ex_Normal;
in vec4 ex_shadowCoord[2];
in vec2 ex_UV;
in vec3 ex_Tangent;
in vec4 ex_mcPosition;

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
uniform vec4 materialEmissive;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;

// Texture Samplers
uniform sampler2D TextureSampler;
uniform sampler3D NoiseSampler;
uniform int textureType;

// Normal Map Samplers
uniform sampler2D NormalMapSampler;
vec3 mappedNormal;

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

//For marble texture
const float NoiseScaleMarble = 0.3;
const int noiseLevel = 4;
const vec3 MarbleColor = vec3(0.98,0.95,0.83);
const vec3 VeinColor = vec3(0.52,0.51,0.46);
const float NoiseFactor = 0.8;
const float PositionFactor = 10;
const float IntensityFactor = 14;

//For moss effect
const vec3 mossColor = vec3(0.18, 0.3, 0.06);

layout(location = 0) out vec4 color;

//NOTE: Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float calculateShadow(int lightIndex){

    float visibility=1.0;
    float bias = 0.0001;
    int samplesInLight = 0;
    float sampleResult;
    for (int i=0;i<6;i++){

		//  - Always the same samples.
		//    Gives a fixed pattern in the shadow, but no noise
        int index = i;
		//  - A random sample, based on the pixel's screen location.
		//    No banding, but the shadow moves with the camera, which looks weird.
		// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
		//  - A random sample, based on the pixel's position in world space.
		//    The position is rounded to the millimeter to avoid too much aliasing
        // int index = int(6.0*random(floor(ex_Position.xyz*1000.0), i))%6;

        vec4 shadowCoordW = ex_shadowCoord[lightIndex] / ex_shadowCoord[lightIndex].w;

        shadowCoordW.xy += poissonDisk[index]/500.0;
        shadowCoordW.z -= bias;
        sampleResult = textureProj(shadowMap[lightIndex],shadowCoordW);
        visibility -= 0.167 * (1.0 - sampleResult);

        // Early bailing - if there's no shadow on 3 samples then
        // we assume the vertex is lit
        if(sampleResult == 1.0){
            samplesInLight++;
        }
        if(samplesInLight >= 3)
        {
            break;
        }

    }

    if(visibility > 0.0){
        return visibility;
    }else{
        return 0.0;
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

            vec3 lightDirWorld = normalize(light.position.xyz - vec3(inverse(ViewMatrix) * ex_Position));
			float lightToSurfaceAngle = degrees(acos(dot(- lightDirWorld, normalize(light.coneDirection.xyz))));

			float innerConeAngle = light.coneAngle;
			float outerConeAngle = light.coneAngle + light.coneFalloffAngle;

			float falloffAngle = innerConeAngle - outerConeAngle;
			spot = clamp((lightToSurfaceAngle - outerConeAngle) / falloffAngle, 0.0, 1.0);


			//NOTE: Useful for debug, allows to see the spotlight soft edges really clearly
			/*if(spot > 0.7){
				return vec4(1.0,0.0,0.0,1.0);
			}else if(spot > 0.4){
				return vec4(0,1.0,0.0,1.0);
			}else{
				return vec4(0,0.0,1.0,1.0);
			}*/
		}
	}

	float diffuseCoefficient =	clamp(dot(lightDir, mappedNormal.xyz), 0.0, 1.0);
	float specularCoefficient = 0.0;

	if(diffuseCoefficient > 0.0 )
	{
		vec3 viewDir = normalize( -ex_Position.xyz);
		vec3 halfDir = normalize(lightDir  + viewDir);

        //Blinn-Phong
        //good Shininess value: 32.0
		//float specAngle = max(dot(halfDir, mappedNormal), 0.0);
		//specularCoefficient = pow(specAngle, materialShininess);

        //Gaussian
        //good Shininess value: 0.3
        float NdotH = clamp(dot(mappedNormal, halfDir), 0.0, 1.0);
        float angle = acos(NdotH);
        specularCoefficient = exp(- pow (angle/ materialShininess, 2));
	}

	vec4 lightIntensity = light.ambientColor * materialAmbient + spot  *  attenuation * (diffuseCoefficient * light.diffuseColor * materialDiffuse + specularCoefficient * light.specularColor * materialSpecular);

    if(spot == 0 || attenuation == 0){
        return lightIntensity;
    }else{
        return lightIntensity * calculateShadow(index);
    }

}

vec4 generateWoodTexture(){
    vec4 temp = texture(NoiseSampler, ex_mcPosition.xyz * NoiseScale) * Noisiness;
    vec3 noisevec = temp.rgb;
    vec3 location = ex_mcPosition.xyz + noisevec;

    float dist = sqrt(location.x * location.x + location.z * location.z);
    dist *= RingFrequency;

    float r = fract(dist + noisevec[0] + noisevec[1] + noisevec[2]) * 2.0;

    if (r > 1.0){
      r = 2.0 - r;
    }
    vec3 colorTemp = mix(LightWood, DarkWood, r);

    r = fract((ex_mcPosition.x + ex_mcPosition.z) * GrainScale + 0.5);
    noisevec[2] *= r;
    if (r < GrainThreshold){
      colorTemp += LightWood * LightGrains * noisevec[2];
    }else{
      colorTemp -= LightWood * DarkGrains * noisevec[2];
    }

    return vec4(colorTemp, 1.0);
}

vec4 noise(float f){
  return texture(NoiseSampler, ex_mcPosition.xyz * NoiseScaleMarble * f);
}

vec4 generateMarbleTexture(){
    float marble=0.0;
    float f = 0.5;
    for(int i=0; i < noiseLevel; i++) {
      marble += noise(f).x/f;
      f *= 2.17;
    }
    vec3 colortemp = vec3(marble);

    vec3 wcPosition = vec3(ModelMatrix * ex_mcPosition);

    vec3 p = wcPosition * 0.5 + 0.5;
    float noise = texture(NoiseSampler, p).r * 0.5 + 0.5;

    float intensity = clamp(noise * NoiseFactor, 0.0, 1.0);
    intensity = cos(ex_mcPosition.x * PositionFactor + intensity * IntensityFactor) * 0.5 + 0.5;
    vec3 a = mix(MarbleColor, VeinColor, intensity);
    vec3 b = mix(colortemp, VeinColor, 0.7);
    vec3 color = mix(a, colortemp, 0.3);

    return vec4(color, 1.0);
}

float generateMoss(){
    vec3 wcPosition = vec3(ModelMatrix * ex_mcPosition);

    vec3 p = wcPosition * 0.5 + 0.5;
    float moss = (texture(NoiseSampler, p).r * 0.5 + 0.5);
    if(moss < 0.8){
      moss-= 0.4;
    } if(moss == 0.9){
      moss-= 0.35;
    } if(moss == 1){
      moss-= 0.3;
    } 
    return max(moss, 0.0);

}

//calculate new normal
vec3 calculateBumpedNormal()
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
    if(textureType != 0){
        mappedNormal = calculateBumpedNormal();
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
    if(textureType != 0){
        //if the texture is wood
        if(textureType == 2){
          vec4 textureResult = generateWoodTexture();
          colorResult = textureResult * lightColorResult;
        //if the texture if marble
        }else if(textureType == 3){
          vec4 textureResult = generateMarbleTexture();
          colorResult = textureResult * lightColorResult;
        //else it is an image texture
        }else{
          float moss = generateMoss();
          vec3 texture = texture(TextureSampler, ex_UV).rgb;
          vec4 colorTemp = vec4(mix(mossColor, texture, (1-moss)), 1.0);
          colorResult = colorTemp * lightColorResult;
        }
    }else{
        colorResult = lightColorResult;
    }

    //add the material's emissive color to make an object shine
    // even if it isn't lit
    color = colorResult + materialEmissive;
}
