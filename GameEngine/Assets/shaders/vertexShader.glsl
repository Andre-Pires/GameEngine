#version 330 core

in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;

out vec4 ex_Position;
out vec4 ex_Color;
out vec4 ex_Normal;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	ex_Normal = normalize(NormalMatrix * in_Normal);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;
	ex_Color = in_Color;
	ex_Position = ModelMatrix * in_Position;

}
