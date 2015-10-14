#include "Scene.h"
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "BufferObjects.h"

/////////////////////////////////////////////////////////////////////// SCENE

void Scene::draw(int vertices, GLuint vao, Matrix4f transform)
{
	BufferObjects * obj = BufferObjects::getInstance();
	Shader  * shader = Shader::getInstance();
	GLint UniformId = shader->getUniformLocation();

	//usar o vertex array object criado
	obj->bindVao(vao);

	//usar o programa criado
	shader->useShaderProgram();

	//usar o id do uniform "matrix" criado para passar a matrix
	//passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
	glUniformMatrix4fv(UniformId, 1, GL_FALSE, MatrixFactory::Mat4toGLfloat(transform));
	//definimos a primitiva a renderizar, numero de elementos a renderizar (vertices), o tipo do valor, um ponteiro para a posição onde está stored
	glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_BYTE, (GLvoid*)0);

	//fazemos unbind do programa e Vao
	//usar o vertex array object criado
	obj->unbindVao();

	//usar o programa criado
	shader->dropShaderProgram();

	Utilities::checkOpenGLError("ERROR: Could not draw scene.");
}