#version 330 core

//In
in vec4 in_Position;
in vec4 in_Color;
in vec4 in_Normal;
in vec2 in_UV;

// Out
out vec4 ex_Color;
out vec4 ex_Normal;
out vec2 ex_UV;

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
    ex_Color = in_Color;
}
