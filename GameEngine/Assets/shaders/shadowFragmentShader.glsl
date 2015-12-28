#version 330 core

// Out
out vec4 out_Color;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	out_Color = vec4( 0.0,0.0, 0.0, 0.0 );
}
