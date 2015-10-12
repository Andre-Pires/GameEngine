#include "Shader.h"
#include <string>

Shader * Shader::instance = 0;

Shader::Shader()
{
}

Shader * Shader::getInstance() {
	if (instance == 0)
	{
		instance = new Shader();
	}

	return instance;
}

void Shader::createShaderProgram()
{
	vertexShader = Utilities::loadFile("Source/vertexShader.glsl");
	//cria o shader e guarda um id para o mesmo
	//temos de especificar o shader
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	//passa o id do shader, //TODO + qlq coisa, uma referencia para o shader, //TODO + qlq coisa
	glShaderSource(VertexShaderId, 1, &vertexShader, 0);
	//compila o shader criado
	glCompileShader(VertexShaderId);

	fragmentShader = Utilities::loadFile("Source/fragmentShader.glsl");

	//o mesmo que o de cima, mas este e o fragment
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &fragmentShader, 0);
	glCompileShader(FragmentShaderId);

	//cria o programa guardando o id do mesmo
	ProgramId = glCreateProgram();
	//usa o id dos shaders criados para os associar ao programa
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	//"binda" um dado indice, do programa criado, a um atributo que vamos usar
	// segundo campo e o indice e o terceiro o nome do atributo
	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");
	glLinkProgram(ProgramId);

	//"binda" uma uniform location, do programa criado, a um atributo (matrix) que vamos usar
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	Utilities::checkOpenGLError("ERROR: Could not create shaders.");
}

void Shader::destroyShaderProgram()
{
	//caso alguém esteja a usar o programa criado, retirá lo
	glUseProgram(0);
	//detachar os shaders do programa
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	//apagar os shaders e finalmente o programa
	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
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

GLint Shader::getUniformLocation()
{
	return UniformId;
}