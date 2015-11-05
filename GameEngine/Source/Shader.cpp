#include "Shader.h"
#include <string>

Shader::Shader()
{
	//cria o programa guardando o id do mesmo
	ProgramId = glCreateProgram();
}

void Shader::addShader(GLenum shaderType, char * shaderLocation)
{
	//cria o shader e guarda um id para o mesmo
	//temos de especificar o shader
	GLuint shaderId = glCreateShader(shaderType);
	this->shadersId[shaderType] = shaderId;

	//carrega o ficheiro de texto do shader
	this->shaders[shaderId] = Utilities::loadFile(shaderLocation);
}

//incremental and sequential number should be used - starts at 0
void Shader::addAttribute(GLuint location, char* atributeName)
{
	this->shaderAttributes[location] = atributeName;
}

void Shader::addUniform(char* uniformName)
{
	//"binda" uma uniform location, do programa criado, a um atributo (matrix) que vamos usar
	this->uniformName.push_back(uniformName);
}

//incremental and sequential number should be used - starts at 0
void Shader::addUniformBlock(GLuint location, char* blockName)
{
	this->uniformBlockName.push_back(blockName);
	this->uniformBlockLocation[blockName] = location;
}

void Shader::createShaderProgram()
{
	for (int i = 0; i < shaderTypes.size(); i++)
	{
		std::map<GLenum, GLuint>::iterator shaderId = shadersId.find(shaderTypes[i]);

		if (shaderId != shadersId.end())
		{
			std::map<GLuint, char *>::iterator shader = shaders.find(shaderId->second);

			if (shader != shaders.end())
			{
				//passa o id do shader, -- , uma referencia para o shader, --
				glShaderSource(shaderId->second, 1, &shader->second, 0);

				//compila o shader criado
				glCompileShader(shaderId->second);
			}
		}
	}

	for (int i = 0; i < shaderTypes.size(); i++)
	{
		std::map<GLenum, GLuint>::iterator shaderId = shadersId.find(shaderTypes[i]);

		if (shaderId != shadersId.end())
		{
			//usa o id dos shaders criados para os associar ao programa
			glAttachShader(ProgramId, shaderId->second);
		}
	}

	for (int atribLocation = 0; atribLocation < shaderAttributes.size(); atribLocation++)
	{
		//"binda" um dado indice, do programa criado, a um atributo que vamos usar
		// segundo campo e o indice e o terceiro o nome do atributo
		glBindAttribLocation(ProgramId, atribLocation, shaderAttributes[atribLocation]);
	}

	glLinkProgram(ProgramId);

	for (auto unifName = uniformName.begin(); unifName != uniformName.end(); ++unifName)
	{
		this->uniformId[(*unifName)] = glGetUniformLocation(ProgramId, (*unifName));
	}

	for (auto blockName = uniformBlockName.begin(); blockName != uniformBlockName.end(); ++blockName)
	{
		GLint id = glGetUniformBlockIndex(ProgramId, (*blockName));
		GLuint location = this->uniformBlockLocation[(*blockName)];
		this->uniformBlockId[(*blockName)] = id;
		glUniformBlockBinding(ProgramId, id, location);
	}

	Utilities::checkOpenGLError("ERROR: Could not create shaders.");
}

void Shader::destroyShaderProgram()
{
	//caso alguém esteja a usar o programa criado, retirá-lo
	glUseProgram(0);

	for (int i = 0; i < shaderTypes.size(); i++)
	{
		std::map<GLenum, GLuint>::iterator shaderId = shadersId.find(shaderTypes[i]);

		if (shaderId != shadersId.end())
		{
			//detachar os shaders do programa
			glDetachShader(ProgramId, shaderId->second);
			//apagar os shaders e finalmente o programa
			glDeleteShader(shaderId->second);
		}
	}
	glDeleteProgram(ProgramId);

	Utilities::checkOpenGLError("ERROR: Could not destroy shaders.");
}

void Shader::useShaderProgram()
{
	//usar o programa criado
	glUseProgram(ProgramId);
}

void Shader::dropShaderProgram()
{
	glUseProgram(0);
}

GLint Shader::getUniformLocation(char* uniformName)
{
	return this->uniformId[uniformName];
}