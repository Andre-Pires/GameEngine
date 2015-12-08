#version 330 core

//In
in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_UV;
in vec3 in_Tangent;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix  * in_Position;
}
