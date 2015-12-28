#version 330 core

// Out
out vec4 out_Color;
in vec2 ex_UV;

uniform float flashRatio;
uniform sampler2D sceneTexture;

void main(void)
{
	out_Color = mix(vec4(1.0), texture( sceneTexture, ex_UV ), flashRatio);
}
