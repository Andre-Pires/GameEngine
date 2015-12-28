#version 330 core

//In
in vec4 in_Position;
in vec2 in_UV;

//out
out vec2 ex_UV;

void main(void)
{
	gl_Position = in_Position;
	ex_UV = in_UV;
}
