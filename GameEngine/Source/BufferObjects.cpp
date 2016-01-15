#include "BufferObjects.h"
#include "Mesh.h"

//Note: Buffer Objects uses the bind locations from Shader.h
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

void BufferObjects::createBufferObjects(GLuint * VboId, GLuint VaoId, std::vector<Vertex> Vertices, std::vector<GLuint> Indices, std::vector<Normal> Normals, std::vector<Texcoord> Textures, std::vector<Tangent> Tangents)
{
	//binda o vertexArray "VaoID" para ser utilizado
	glBindVertexArray(VaoId);
	{
		//usa o 1 buffer object para guardar os vertices e as cores
		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);
		//enabla um dos atributos adicionados ao vertex shader na sua criacao (neste caso o VERTICES)
		glEnableVertexAttribArray(VERTICES);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertices na estrutura, o inicio da mesma
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), Normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(NORMALS);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertice na estrutura, o inicio da mesma (tem de ser depois da posicoes)
		glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ARRAY_BUFFER, Textures.size() * sizeof(Texcoord), Textures.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(TEXCOORDS);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertice na estrutura, o inicio da mesma (tem de ser depois da posicoes)
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[3]);
		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ARRAY_BUFFER, Tangents.size() * sizeof(Tangent), Tangents.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(TANGENTS);
		//define o tamanho do atributo, o tipo, se esta normalizado, o tamanho de cada vertice na estrutura, o inicio da mesma (tem de ser depois da posicoes)
		glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Tangent), 0);

		//usa o 2 buffer object para guardar os indices que permitem selecionar quais os vertices (dos presentes no buffer) a serem desenhados
		//assim nao temos de estar sempre a adicionar vertices podemos faze lo tudo de uma vez, limitando nos a escolher quais usar de seguida
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[4]);

		//descreve o espaco a alocar como o tamanho da estrutura criada e diz que os dados sao estaticos
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);
	}

	//remover todos os buffers em uso
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader::checkGenericOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void BufferObjects::createCameraBufferObjects(GLuint * camVboId)
{
	glBindBuffer(GL_UNIFORM_BUFFER, *camVboId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 32, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, *camVboId);

	Shader::checkGenericOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void BufferObjects::updateCamera(GLuint * camVboId, Matrix4f view, Matrix4f projection)
{
	glBindBuffer(GL_UNIFORM_BUFFER, *camVboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat) * 16, view.getMatrix());
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(GLfloat) * 16, sizeof(GLfloat) * 16, projection.getMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	Shader::checkGenericOpenGLError("ERROR: Problem updating camera.");
}

void BufferObjects::destroyBufferObjects(GLuint * VboId, GLuint VaoId)
{
	if (*VboId == -1 || VaoId == -1)
	{
		//object's buffers were already destroyed (several nodes might share an object)
		return;
	}

	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(TANGENTS);
	glDeleteBuffers(5, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//mark buffers has having been destroyed
	*VboId = -1;
	VaoId = -1;

	Shader::checkGenericOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
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
	//alterado para criar espaco para as normais, texturas e tangentes
	GLuint * VboId = new GLuint[5];

	//gera dois buffer names e guarda os
	glGenBuffers(5, VboId);

	return VboId;
}

GLuint* BufferObjects::getCameraVboId()
{
	GLuint* cameraVboId = new GLuint;
	//gera dois buffer names e guarda os
	glGenBuffers(1, cameraVboId);

	return cameraVboId;
}