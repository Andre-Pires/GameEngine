#version 330 core

in vec4 in_Position;
in vec3 in_Normal;
in vec2 in_UV;
in vec3 in_Tangent;

out vec4 ex_Position;
out vec3 ex_Normal;
out vec2 ex_UV;
out vec4 ex_mcPosition;
// Used for shadow lookup
out vec4 ex_shadowCoord[2];
out vec3 ex_Tangent;

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

//needed to check how many lights need shadows (max. 2)
uniform int numLights;

// Values that stay constant for the whole mesh.
uniform mat4 lightViewMatrix[2];

void main(void)
{
	ex_Normal = normalize(NormalMatrix * in_Normal);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_Position =   ViewMatrix * ModelMatrix * in_Position;
	ex_UV = in_UV;
	ex_mcPosition = in_Position;

	ex_shadowCoord[0] = lightViewMatrix[0] * ModelMatrix * in_Position;
	if(numLights > 1){
		ex_shadowCoord[1] = lightViewMatrix[1] * ModelMatrix * in_Position;
	}

	ex_Tangent = (ModelMatrix * vec4(in_Tangent, 0.0)).xyz;
}
