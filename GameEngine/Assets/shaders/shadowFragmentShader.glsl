#version 330 core


// Out
out vec4 out_Color;

// Material Attributes
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;

// Texture Samplers
uniform sampler2D TextureSampler;
uniform sampler3D WoodSampler;
uniform int textureActive;
uniform int woodTextureActive;

uniform vec4 cameraPosition;

uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	out_Color = vec4( 0.0,0.0, 0.0, 0.0 );
}
