#include "Utilities.h"
#include <sstream>
#include "Shader.h"

float Utilities::lerp(float val1, float val2, float ratio) {
	return ratio * val1 + (1 - ratio) * val2;
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