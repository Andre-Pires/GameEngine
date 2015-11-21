#include "Texture.h"

Texture::Texture(Shader* shader, char* filename) {
	int width, height;

	this->uniform = shader->getUniformLocation("TextureSampler");

	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	if (image == 0) {
		exit(1);
	}

	glGenTextures(1, &textureID);
	this->textureID = textureID;
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();

	SOIL_free_image_data(image);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureID() {
	return textureID;
}
GLint Texture::getTexUniform() {
	return uniform;
}