#include "Utilities.h"
#include <sstream>

bool Utilities::isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;

	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void Utilities::checkOpenGLError(std::string error) {
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

GLchar* Utilities::loadFile(char* fileName) {
	std::string fileLine, result;
	std::ifstream shaderFile(fileName);
	char* shaderContents;
	int size;

	if (shaderFile.is_open()) {
		while (getline(shaderFile, fileLine)) {
			result += fileLine + "\n";
		}
		shaderFile.close();
	}
	size = result.size();
	shaderContents = new char[size];
	memcpy(shaderContents, result.c_str(), size);
	shaderContents[size - 1] = '\0';

	return shaderContents;
}