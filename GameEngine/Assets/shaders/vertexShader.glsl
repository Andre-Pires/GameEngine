#version 330 core

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_UV;

out vec4 ex_Position;
out vec4 ex_Color;
out vec3 ex_Normal;
out vec2 ex_UV;
// Used for shadow lookup
out vec4 ex_shadowCoord;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

// Values that stay constant for the whole mesh.
uniform mat4 lightViewMatrix;

void main(void)
{
	//ex_Normal = normalize(NormalMatrix * in_Normal);
	//NOTE:this should be moved to the application
	mat3 nMat = transpose(inverse(mat3(ViewMatrix * ModelMatrix)));
	ex_Normal = normalize(nMat * in_Normal.xyz);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_Color = in_Color;
	ex_Position =   ViewMatrix * ModelMatrix * in_Position;
	ex_UV = in_UV;
	ex_shadowCoord = lightViewMatrix * ModelMatrix * in_Position;
}
