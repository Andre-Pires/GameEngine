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
	//default - wood texture
	textureType = 2;
}

void Texture::prepareTexture(GLenum type, int size1, int size2) {
	glGenTextures(1, &textureID);
	this->textureID = textureID;

	glBindTexture(type, textureID);

	if (type == GL_TEXTURE_2D)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size1, size2, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	if (type == GL_TEXTURE_3D)
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, size1, size1, size1, 0, GL_RED, GL_FLOAT, noise);

	glGenerateMipmap(type);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(type, 0);
}

void Texture::bind(Shader* shader, GLenum activeIndex, GLint uniform, int type, GLint index) {
	glActiveTexture(activeIndex);
	if (textureType == 1 && type != CHANNEL) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(getTexUniform(shader, type), index);
	} 
	else if (textureType == 2 || textureType == 3) {
		glBindTexture(GL_TEXTURE_3D, textureID);
		glUniform1i(getTexUniform(shader, PROCEDURAL), index);
	}
	else if (type == CHANNEL) {
		glBindTexture(GL_TEXTURE_3D, textureID);
		glUniform1i(getTexUniform(shader, type), index);
	}
	if (type != TANGENTS) {
		glUniform1i(uniform, textureType);
	}
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
	else if (type == CHANNEL) {
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

void Texture::setTextureNoise(float noise, int index) {
	this->noise[index] = noise;
}