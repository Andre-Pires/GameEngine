#include "BufferObjects.h"

BufferObjects * BufferObjects::instance = 0;

BufferObjects::BufferObjects()
{
}

BufferObjects * BufferObjects::getInstance() {
	if (instance == 0)
	{
		instance = new BufferObjects();
	}

	return instance;
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void BufferObjects::createBufferObjects(GLuint * VboId, GLuint VaoId, Vertex * Vertices, int verticesSize, GLubyte *Indices, int indicesSize)
{
	//binda o vertexArray "VaoID" para ser utilizado
	glBindVertexArray(VaoId);
	{
		//usa o 1 buffer object para guardar os vertices e as cores
		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ARRAY_BUFFER, verticesSize, Vertices, GL_STATIC_DRAW);
		//enabla um dos atributos adicionados ao vertex shader na sua criacao (neste caso o VERTICES)
		glEnableVertexAttribArray(VERTICES);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertices na estrutura, o inicio da mesma
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		//enabla um dos atributos adicionados ao vertex shader na sua criacao
		glEnableVertexAttribArray(COLORS);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertice na estrutura, o inicio da mesma (tem de ser depois da posicoes)
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(sizeof(Vertices[0].XYZW)));

		//usa o 2 buffer object para guardar os indices que permitem selecionar quais os vertices (dos presentes no buffer) a serem desenhados
		//assim nao temos de estar sempre a adicionar vertices podemos faze lo tudo de uma vez, limitando nos a escolher quais usar de seguida
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);

		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, Indices, GL_STATIC_DRAW);
	}

	//remover todos os buffers em uso
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Utilities::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void BufferObjects::destroyBufferObjects(GLuint * VboId, GLuint VaoId)
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Utilities::checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void BufferObjects::bindVao(GLuint VaoId)
{
	//usar o vertex array object criado
	glBindVertexArray(VaoId);
}

void BufferObjects::unbindVao()
{
	glBindVertexArray(0);
}

GLuint BufferObjects::getVaoId()
{
	GLuint VaoId;
	//gera nome para os vertex array object
	// 1 arg é o numero de nomes a criar, o 2 o array onde guardar esse nome
	glGenVertexArrays(1, &VaoId);

	return VaoId;
}

GLuint * BufferObjects::getVboId()
{
	GLuint * VboId = new GLuint[2];

	//gera dois buffer names e guarda os
	glGenBuffers(2, VboId);

	return VboId;
}