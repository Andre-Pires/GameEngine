﻿#include "Scene.h"
#include "Matrix4f.h"
#include "MatrixFactory.h"
#include "BufferObjects.h"

Scene::Scene(Shader * shader, char * modelName, char * normalName)
{
	this->shader = shader;
	this->modelName = modelName;
	this->normalName = normalName;
}

/////////////////////////////////////////////////////////////////////// SCENE

void Scene::draw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors, Texture* texture, Texture* normalMap)
{
	if (shader != nullptr)
	{
		BufferObjects * obj = BufferObjects::getInstance();
		GLint modelUniform = shader->getUniformLocation(modelName);
		GLint normalUniform = shader->getUniformLocation(normalName);
		GLint ambientUniform = shader->getUniformLocation("materialAmbient");
		GLint diffuseUniform = shader->getUniformLocation("materialDiffuse");
		GLint specularUniform = shader->getUniformLocation("materialSpecular");
		GLint textureActiveUnif = shader->getUniformLocation("textureActive");
		GLint woodTextureActiveUnif = shader->getUniformLocation("woodTextureActive");
		//usar o vertex array object criado
		obj->bindVao(vao);
		shader->checkShaderError("ERROR: Could not draw scene.");
		//usar o programa criado
		shader->useShaderProgram();

		glUniform4fv(ambientUniform, 1, materialColors.AMBIENT);
		glUniform4fv(diffuseUniform, 1, materialColors.DIFFUSE);
		glUniform4fv(specularUniform, 1, materialColors.SPECULAR);
		//glUniformMatrix4fv(normalUniform, 1, GL_FALSE, MatrixFactory::Mat4toGLfloat(normalMat));

		Matrix3f tempMat = MatrixFactory::Mat4to3(modelMatrix);
		Matrix4f normalMat = MatrixFactory::Mat3to4(tempMat.Inverse().Transpose());

		//usar o id do uniform "matrix" criado para passar a matrix model
		//passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
		glUniformMatrix4fv(normalUniform, 1, GL_FALSE, normalMat.getMatrix());
		////passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelMatrix.getMatrix());

		//texturas
		if (texture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			if (texture->textureDim == 0) {
				glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
				glUniform1i(texture->getTexUniform(shader, TEXCOORDS), 0);
				glUniform1i(woodTextureActiveUnif, 0);
			}
			else if (texture->textureDim == 1) {
				glBindTexture(GL_TEXTURE_3D, texture->getTextureID());
				glUniform1i(texture->getTexUniform(shader, WOOD), 0);
				glUniform1i(woodTextureActiveUnif, 1);
				shader->checkShaderError("ERROR: Could not draw scene.");
			}
			glUniform1i(textureActiveUnif, 1);
			shader->checkShaderError("ERROR: Could not draw scene.");
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap->getTextureID());
			glUniform1i(normalMap->getTexUniform(shader, TANGENTS), 1);
			shader->checkShaderError("ERROR: Could not draw scene.");
		}
		else
		{
			glUniform1i(textureActiveUnif, 0);
			glUniform1i(woodTextureActiveUnif, 0);
			shader->checkShaderError("ERROR: Could not draw scene.");
		}

		//definimos a primitiva a renderizar, numero de elementos a renderizar (vertices), o tipo do valor, um ponteiro para a posição onde está stored
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, (GLvoid*)0);

		//fazemos unbind do programa e Vao
		//usar o vertex array object criado
		obj->unbindVao();

		//usar o programa criado
		shader->dropShaderProgram();

		shader->checkShaderError("ERROR: Could not draw scene.");
	}
}

void Scene::setActiveShader(Shader * shader)
{
	this->shader = shader;
}