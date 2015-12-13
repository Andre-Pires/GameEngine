#version 330 core

//In
in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_UV;
in vec3 in_Tangent;

//out
out vec2 ex_UV;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

void main(void)
{
	gl_Position = in_Position;
	ex_UV = in_UV;
}
