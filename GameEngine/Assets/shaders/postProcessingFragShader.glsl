#version 330 core

// Out
out vec4 out_Color;
in vec2 ex_UV;

// Material Attributes
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;

// Texture Samplers
uniform sampler2D TextureSampler;
uniform sampler2D NormalMapSampler;
uniform int textureActive;

uniform vec4 cameraPosition;
uniform float flashRatio;
uniform sampler2D sceneTexture;

void main(void)
{
	out_Color = mix(vec4(1.0), texture( sceneTexture, ex_UV ), flashRatio);
}
