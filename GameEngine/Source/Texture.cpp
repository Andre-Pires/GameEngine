#include "Texture.h"

//Constructor for 2D Textures - generated using images
Texture::Texture(Shader* shader, char* filename) {
	int width, height;

	data = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	if (data == 0) {
		exit(1);
	}

	prepareTexture(GL_TEXTURE_2D, width, height);

	//image texture
	textureType = 1;

	SOIL_free_image_data(data);
}

//Contructor for 3D Textures - generated using procedural calculations
Texture::Texture(Shader* shader) {
	int size = 32;
	float persistence = 0.5;
	int octaves = 4;
	PerlinNoise tempNoise = PerlinNoise(persistence, octaves);
	tempNoise.calculateNoise(size);
	for (int x = 0; x < size*size*size; x++) {
		noise[x] = tempNoise.noise[x];
	}
	//3D texture only need the size, thefore the use of 0
	prepareTexture(GL_TEXTURE_3D, size, 0);

	//default - wood texture
	textureType = 2;
}

void Texture::prepareTexture(GLenum type, int size1, int size2) {
	glGenTextures(1, &textureID);
	this->textureID = textureID;

	bind(type);

	if (type == GL_TEXTURE_2D)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size1, size2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	if (type == GL_TEXTURE_3D)
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, size1, size1, size1, 0, GL_RGB, GL_FLOAT, noise);

	glGenerateMipmap(type);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	unbind(type);
}

void Texture::bind(GLenum type) {
	glBindTexture(type, textureID);
}

void Texture::unbind(GLenum type) {
	glBindTexture(type, 0);
}

GLuint Texture::getTextureID() {
	return textureID;
}
GLint Texture::getTexUniform(Shader * shader, int type) {
	if (type == TEXCOORDS) {
		return shader->getUniformLocation("TextureSampler");
	}
	else if (type == TANGENTS) {
		return shader->getUniformLocation("NormalMapSampler");
	}
	else if (type == PROCEDURAL) {
		return shader->getUniformLocation("NoiseSampler");
	}
	else
	{
		std::cerr << "ERROR: No texture type corresponds to this value!" << std::endl;
	}
}

void Texture::setTextureType(int type) {
	textureType = type;
}