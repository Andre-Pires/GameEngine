#include "Scene.h"

Scene::Scene(Shader * shader, Camera * camera, char * modelName, char * normalName)
{
	this->shader = shader;
	this->modelName = modelName;
	this->normalName = normalName;
	this->camera = camera;
}

/////////////////////////////////////////////////////////////////////// SCENE

void Scene::draw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors, float materialShininess, Texture* texture, Texture* normalMap)
{
	if (shader != nullptr)
	{
		switch (shader->getShaderType())
		{
		case MAIN_SHADER:
			standardDraw(vertices, vao, modelMatrix, materialColors, materialShininess, texture, normalMap);
			break;
		case SHADOW_SHADER:
			drawShadowMap(vertices, vao, modelMatrix);
			break;
		case POSTPROCESS_SHADER:
			drawPostProcess(vertices, vao);
			break;
		default:
			cout << "ERROR: Invalid shader type." << endl;
			break;
		}
	}
}

void Scene::standardDraw(int vertices, GLuint vao, Matrix4f modelMatrix, Material materialColors, float materialShininess, Texture* texture, Texture* normalMap)
{
	if (shader != nullptr)
	{
		BufferObjects * obj = BufferObjects::getInstance();
		GLint modelUniform = shader->getUniformLocation(modelName);
		GLint normalUniform = shader->getUniformLocation(normalName);
		GLint emissiveUniform = shader->getUniformLocation("materialEmissive");
		GLint ambientUniform = shader->getUniformLocation("materialAmbient");
		GLint diffuseUniform = shader->getUniformLocation("materialDiffuse");
		GLint specularUniform = shader->getUniformLocation("materialSpecular");
		GLint shininessUniform = shader->getUniformLocation("materialShininess");
		GLint textureTypeUnif = shader->getUniformLocation("textureType");

		//usar o vertex array object criado
		obj->bindVao(vao);
		//usar o programa criado
		shader->useShaderProgram();

		glUniform4fv(emissiveUniform, 1, materialColors.EMISSIVE);
		glUniform4fv(ambientUniform, 1, materialColors.AMBIENT);
		glUniform4fv(diffuseUniform, 1, materialColors.DIFFUSE);
		glUniform4fv(specularUniform, 1, materialColors.SPECULAR);
		glUniform1f(shininessUniform, materialShininess);

		Matrix3f tempMat = MatrixFactory::Mat4to3(camera->getViewMatrix() * modelMatrix);
		Matrix3f normalMat = tempMat.Inverse().Transpose();

		//usar o id do uniform "matrix" criado para passar a matrix model
		//passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
		glUniformMatrix3fv(normalUniform, 1, GL_FALSE, normalMat.getMatrix());
		////passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelMatrix.getMatrix());

		//texturas
		if (texture != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			if (texture->textureType == 1) {
				glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
				glUniform1i(texture->getTexUniform(shader, TEXCOORDS), 0);
				glUniform1i(textureTypeUnif, 1);
			}
			else if (texture->textureType == 2 || texture->textureType == 3) {
				glBindTexture(GL_TEXTURE_3D, texture->getTextureID());
				glUniform1i(texture->getTexUniform(shader, PROCEDURAL), 0);
				glUniform1i(textureTypeUnif, texture->textureType);
			}
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMap->getTextureID());
			glUniform1i(normalMap->getTexUniform(shader, TANGENTS), 1);
		}
		else
		{
			glUniform1i(textureTypeUnif, 0);
		}

		//definimos a primitiva a renderizar, numero de elementos a renderizar (vertices), o tipo do valor, um ponteiro para a posição onde está stored
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, (GLvoid*)0);

		//fazemos unbind do programa e Vao
		//usar o vertex array object criado
		obj->unbindVao();

		//usar o programa criado
		shader->dropShaderProgram();

		shader->checkShaderError("ERROR: Could not draw scene (standard).");
	}
}

void Scene::drawShadowMap(int vertices, GLuint vao, Matrix4f modelMatrix)
{
	if (shader != nullptr)
	{
		BufferObjects * obj = BufferObjects::getInstance();
		GLint modelUniform = shader->getUniformLocation(modelName);

		//usar o vertex array object criado
		obj->bindVao(vao);
		//usar o programa criado
		shader->useShaderProgram();

		////passamos o id do atributo, o numero de matrizes, se deve ser transposta e a matriz
		glUniformMatrix4fv(modelUniform, 1, GL_FALSE, modelMatrix.getMatrix());

		//definimos a primitiva a renderizar, numero de elementos a renderizar (vertices), o tipo do valor, um ponteiro para a posição onde está stored
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, (GLvoid*)0);

		//fazemos unbind do programa e Vao
		//usar o vertex array object criado
		obj->unbindVao();

		//usar o programa criado
		shader->dropShaderProgram();

		shader->checkShaderError("ERROR: Could not draw scene (shadow map).");
	}
}

void Scene::drawPostProcess(int vertices, GLuint vao)
{
	if (shader != nullptr)
	{
		BufferObjects * obj = BufferObjects::getInstance();
		GLint modelUniform = shader->getUniformLocation(modelName);

		//usar o vertex array object criado
		obj->bindVao(vao);
		//usar o programa criado
		shader->useShaderProgram();

		//definimos a primitiva a renderizar, numero de elementos a renderizar (vertices), o tipo do valor, um ponteiro para a posição onde está stored
		glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, (GLvoid*)0);

		//fazemos unbind do programa e Vao
		//usar o vertex array object criado
		obj->unbindVao();

		//usar o programa criado
		shader->dropShaderProgram();

		shader->checkShaderError("ERROR: Could not draw scene (post-process).");
	}
}

void Scene::setActiveShader(Shader * shader)
{
	this->shader = shader;
}