///////////////////////////////////////////////////////////////////////
//
// (c)2015 by André Pires
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "Vector3f.h"
#include "MatrixFactory.h"
#include "Scene.h"
#include "Shader.h"
#include "Utilities.h"
#include "GeometricObject.h"
#include "BufferObjects.h"
#include "Triangle.h"
#include "Square.h"
#include "Diamond.h"
#include "Camera.h"
#include "Plane.h"
#include "Mesh.h"
#include "SceneGraphNode.h"
#include "Light.h"
#include "Texture.h"
#include "Bomberman.h"
#include "ShadowRenderer.h"
#include "PostProcessRenderer.h"

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"

#define CAPTION "Game Engine"
#define ANIMATION_RATE 1000 / 60

int WinX = 1024, WinY = 576;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float viewportRatio = 1.33;

BufferObjects* bufferObjects;
Shader* shader;
Shader* shadowShader;
Shader* postProcessingShader;
Scene* scene;
Camera* camera;
Bomberman* bomberman;

//Textures
Texture* texture;

// Camera Position
float eyeX = 0;
float eyeY = 0;
float eyeZ = 5;
float centerX, centerY, centerZ = 0;
CameraType cameraType = CONTROLLED_PERSP;
GimbalLockState gimbalState = GIMBAL_LOCK_OFF;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 5.25f;
float rotateX, rotateY = 0.0f;
float zoom = 3.0f;

SceneGraphNode* gameNode;
SceneGraphNode* sceneGraph;
SceneGraphNode* scenePlane;

// Lights
int controllableLight = 0;
std::vector<Light *> sceneLights;

// Shadows
bool shadowRenderingActive = true;
std::vector<ShadowRenderer *> shadowRenderers;

//Post Process Flash
PostProcessRenderer * postProcessRenderer;
bool flashActive;
std::clock_t flashStart;
typedef void(*CallbackType)();

unsigned _lastUpdateTime = 0;

void activateFlash();

/////////////////////////////////////////////////////////////////////// SCENE
// correct order -> scale * rotation * translation
void createGameScene()
{
	gameNode = new SceneGraphNode(sceneGraph, scene);
	bomberman = new Bomberman(std::string("Assets/layouts/1.txt"), scene, gameNode, bufferObjects, shader, &activateFlash);

	sceneGraph->add(gameNode);

	Mesh mesh = Mesh(std::string("Assets/mesh/quad.obj"));
	GeometricObject * object = new GeometricObject(bufferObjects, scene, mesh);
	object->rotate(90, Vector3f(0.0, 0.0, 1.0));
	scenePlane = new SceneGraphNode(sceneGraph, object, scene);
}

void activateFlash()
{
	flashActive = true;
	flashStart = std::clock();
}

void flashTimer(int value)
{
	float totalDuration = 1500;
	float timeElapsed = (std::clock() - flashStart) / (double)(CLOCKS_PER_SEC / 1000);
	if (flashActive)
	{
		if (timeElapsed < totalDuration)
		{
			float ratio = (1 * timeElapsed) / totalDuration;

			postProcessingShader->useShaderProgram();
			glUniform1f(postProcessingShader->getUniformLocation(std::string("flashRatio")), ratio);
		}
		else
		{
			flashActive = false;
		}
	}

	glutTimerFunc(ANIMATION_RATE, flashTimer, 0);
}

void updateCamera()
{
	if (cameraType == ORTHOGRAPHIC)
	{
		camera->ortho(-6.0f + centerX, 6.0f + centerX, -6.0f + centerY, 6.0f + centerY, -3.0f, 3.0f);
	}
	else if (cameraType == PERSPECTIVE)
	{
		camera->perspective(45.0f, viewportRatio, 0.1f, 100.0f);

		if (gimbalState == GIMBAL_LOCK_ON)
		{
			// set the camera using a function similar to gluLookAt
			camera->lookAt(Vector3f(4.0, -6.0, 11), Vector3f(4.0, -4.0, 0), Vector3f(0, 1, 0));
		}
		else if (gimbalState == GIMBAL_LOCK_OFF)
		{
			camera->quaternionLookAt(0, 35, 8, Vector3f(0, 0, 5), Vector3f(-5.5, 3.0, 0), Vector3f(0, 1, 0));
		}
	}
	else if (cameraType == CONTROLLED_PERSP)
	{
		camera->perspective(45.0f, viewportRatio, 0.1f, 100.0f);
		Vector2f playerPosition = bomberman->getPlayerPosition();
		// set the camera using a function similar to gluLookAt
		if (gimbalState == GIMBAL_LOCK_ON)
		{
			// set the camera using a function similar to gluLookAt
			camera->lookAt(Vector3f(playerPosition.x, -playerPosition.y, eyeZ), Vector3f(playerPosition.x, -playerPosition.y, centerZ), Vector3f(0, 1, 0));
		}
		else if (gimbalState == GIMBAL_LOCK_OFF)
		{
			camera->quaternionLookAt(rotateX, rotateY, zoom, Vector3f(0, 0, 5), Vector3f(-playerPosition.x, playerPosition.y, centerZ), Vector3f(0, 1, 0));
		}
	}

	camera->updateCamera();
}

void drawScene()
{
	unsigned currentUpdateTime = glutGet(GLUT_ELAPSED_TIME);
	unsigned elapsed = currentUpdateTime - _lastUpdateTime;
	_lastUpdateTime = currentUpdateTime;

	bomberman->update(elapsed);

	//NOTE: calculates shadows for up to 2 lights in scene
	if (shadowRenderingActive)
	{
		for (int i = 0; i < min((int)sceneLights.size(), 2); i++)
		{
			shadowRenderers[i]->updateCameraCenter(Vector3f(centerX, centerY, centerZ));
			shadowRenderers[i]->renderShadows();
		}
	}
	scene->setActiveShader(shader);

	//make the spotlight follow the player
	sceneLights[0]->position.x = bomberman->getPlayerPosition().x;
	sceneLights[0]->position.y = -bomberman->getPlayerPosition().y;

	//NOTE: shadow rendering borrows the camera for calculations so we have to put it back
	if (cameraType == CONTROLLED_PERSP || shadowRenderingActive)
	{
		updateCamera();

		for (int i = 0; i < sceneLights.size(); i++)
		{
			sceneLights[i]->setLightShaderValues();
		}
	}

	if (flashActive)
	{
		postProcessRenderer->renderFlash();

		scene->setActiveShader(postProcessingShader);
		scenePlane->draw();
		scene->setActiveShader(shader);
	}
	else
	{
		sceneGraph->draw();
	}
}

void createProgram()
{
	//creating and linking shader program with respective shaders
	shader = new Shader();
	shader->addShader(GL_VERTEX_SHADER, "Assets/shaders/vertexShader.glsl");
	shader->addShader(GL_FRAGMENT_SHADER, "Assets/shaders/fragmentShader.glsl");
	shader->addAttribute(VERTICES, "in_Position");
	shader->addAttribute(NORMALS, "in_Normal");
	shader->addAttribute(TEXCOORDS, "in_UV");
	shader->addAttribute(TANGENTS, "in_Tangent");

	//used while drawing the scene
	shader->addUniform("materialAmbient");
	shader->addUniform("materialDiffuse");
	shader->addUniform("materialSpecular");
	shader->addUniform("materialShininess");
	shader->addUniform("ModelMatrix");
	shader->addUniform("NormalMatrix");

	//textures
	shader->addUniform("TextureSampler");
	shader->addUniform("WoodSampler");
	shader->addUniform("NormalMapSampler");
	shader->addUniform("textureActive");
	shader->addUniform("woodTextureActive");
	//used for setting up the lights
	shader->addUniform("cameraPosition");
	shader->addUniform("numLights");

	//used for the camera's matrixes
	shader->addUniformBlock(UBO_BP, "SharedMatrices");

	//NOTE: code for the spotlight
	Light * spotLight = new Light(shader, sceneLights.size(), SPOTLIGHT);
	sceneLights.push_back(spotLight);

	//NOTE: code for the point light
	Light * pointLight = new Light(shader, sceneLights.size(), POINT_LIGHT);
	sceneLights.push_back(pointLight);

	//NOTE: code for the direccional light
//	Light * directionalLight = new Light(shader, sceneLights.size(), DIRECTIONAL_LIGHT);
//	sceneLights.push_back(directionalLight);

	for (int i = 0; i < 2; i++)
	{
		//shadow uniforms
		shader->addUniform("lightViewMatrix[" + std::to_string(i) + "]");
		shader->addUniform("shadowMap[" + std::to_string(i) + "]");
	}

	//NOTE: has to happen after the lights set the uniforms
	shader->createShaderProgram();

	//generating vao
	bufferObjects = BufferObjects::getInstance();

	//should be initialized after light camera since this camera should occupy the shared UBOs
	camera = new Camera(bufferObjects, scene, shader);

	//creating new scene object for further drawing
	scene = new Scene(shader, camera, "ModelMatrix", "NormalMatrix");

	sceneGraph = new SceneGraphNode(scene);

	//creating our game's objects
	createGameScene();

	//NOTE: code for the point light
	pointLight->position = Vector4f(6, 0.5, 7.0, 1.0);
	pointLight->ambientColor = Vector4f(0.1, 0.1, 0.1, 1.0);
	pointLight->diffuseColor = Vector4f(0.8, 0.8, 0.8, 1.0);
	pointLight->specularColor = Vector4f(1.0, 1.0, 1.0, 1.0);
	pointLight->lightRange = 20.0f;

	//NOTE: code for the spotlight
	spotLight->position = Vector4f(bomberman->getPlayerPosition().x, bomberman->getPlayerPosition().y, 4.0, 1.0);
	spotLight->ambientColor = Vector4f(0.05, 0.05, 0.05, 1.0);
	spotLight->diffuseColor = Vector4f(0.8, 0.8, 0.8, 1.0);
	spotLight->specularColor = Vector4f(1.0, 1.0, 1.0, 1.0);
	spotLight->lightRange = 25.0f;
	spotLight->coneAngle = 30.00f;
	spotLight->coneFalloffAngle = 3.0f;
	spotLight->coneDirection = Vector4f(0.3, 0.0, -1.0, 1.0);

	//NOTE: code for the directional light
//	directionalLight->position = Vector4f(-9.6, 0, 10.0, 1.0);
//	directionalLight->ambientColor = Vector4f(0.1, 0.1, 0.1, 1.0);
//	directionalLight->diffuseColor = Vector4f(0.85, 0.85, 0.85, 1.0);
//	directionalLight->specularColor = Vector4f(0.9, 0.9, 0.9, 1.0);

	//passes amount of scene lights to shader
	shader->useShaderProgram();
	GLint lighNumberId = shader->getUniformLocation("numLights");
	glUniform1i(lighNumberId, sceneLights.size());
	shader->dropShaderProgram();

	///SHADOW SHADER -- LIGHTER THAN MAIN SHADER
	shadowShader = new Shader();
	shadowShader->addShader(GL_VERTEX_SHADER, "Assets/shaders/shadowVertexShader.glsl");
	shadowShader->addShader(GL_FRAGMENT_SHADER, "Assets/shaders/shadowFragmentShader.glsl");
	shadowShader->addAttribute(VERTICES, "in_Position");
	shadowShader->addAttribute(NORMALS, "in_Normal");
	shadowShader->addAttribute(TEXCOORDS, "in_UV");
	shadowShader->addAttribute(TANGENTS, "in_Tangents");

	//used while drawing the scene
	shadowShader->addUniform("materialAmbient");
	shadowShader->addUniform("materialDiffuse");
	shadowShader->addUniform("materialSpecular");
	shadowShader->addUniform("materialShininess");
	shadowShader->addUniform("ModelMatrix");
	shadowShader->addUniform("NormalMatrix");

	//textures
	shadowShader->addUniform("TextureSampler");
	shadowShader->addUniform("WoodSampler");
	shadowShader->addUniform("NormalMapSampler");
	shadowShader->addUniform("textureActive");
	shadowShader->addUniform("woodTextureActive");
	//used for setting up the lights
	shadowShader->addUniform("cameraPosition");
	shadowShader->addUniform("numLights");

	//used for the camera's matrixes
	shadowShader->addUniformBlock(UBO_BP, "SharedMatrices");
	shadowShader->createShaderProgram();

	//for performance reasons shader only supports two lights' shadows
	for (int i = 0; i < min((int)sceneLights.size(), 2); i++)
	{
		shadowRenderers.push_back(new ShadowRenderer(sceneLights[i], i, camera, scene, sceneGraph, shader, shadowShader));
		shadowRenderers[i]->generateShadowFBO(WinX, WinY);
	}

	///POST PROCESSING SHADER
	postProcessingShader = new Shader();
	postProcessingShader->addShader(GL_VERTEX_SHADER, "Assets/shaders/postProcessingVertShader.glsl");
	postProcessingShader->addShader(GL_FRAGMENT_SHADER, "Assets/shaders/postProcessingFragShader.glsl");
	postProcessingShader->addAttribute(VERTICES, "in_Position");
	postProcessingShader->addAttribute(NORMALS, "in_Normal");
	postProcessingShader->addAttribute(TEXCOORDS, "in_UV");
	postProcessingShader->addAttribute(TANGENTS, "in_Tangents");

	//used while drawing the scene
	postProcessingShader->addUniform("materialAmbient");
	postProcessingShader->addUniform("materialDiffuse");
	postProcessingShader->addUniform("materialSpecular");
	postProcessingShader->addUniform("materialShininess");
	postProcessingShader->addUniform("ModelMatrix");
	postProcessingShader->addUniform("NormalMatrix");

	//textures
	postProcessingShader->addUniform("TextureSampler");
	postProcessingShader->addUniform("WoodSampler");
	postProcessingShader->addUniform("NormalMapSampler");
	postProcessingShader->addUniform("textureActive");
	postProcessingShader->addUniform("woodTextureActive");

	//used for setting up the lights
	postProcessingShader->addUniform("cameraPosition");
	postProcessingShader->addUniform("numLights");

	//texture for the scene post processing
	postProcessingShader->addUniform("sceneTexture");
	postProcessingShader->addUniform("flashRatio");

	postProcessingShader->createShaderProgram();

	postProcessRenderer = new PostProcessRenderer(scene, sceneGraph, shader, postProcessingShader);
	postProcessRenderer->generatePostProcessFBO(WinX, WinY);
}

/////////////////////////////////////////////////////////////////////// UTILITIES

void clearObjectsFromBuffer()
{
	sceneGraph->clearBuffer();
}

/*********************************************************/
/*                                                       */
/*                        KEYBOARD                       */
/*                                                       */

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key)
	{
	case '1':
		cameraType = ORTHOGRAPHIC;
		updateCamera();
		cout << "Camera status: " << endl << " - Ortographic camera active." << endl;
		break;

	case '2':
		cameraType = PERSPECTIVE;
		updateCamera();
		cout << "Camera status: " << endl << " - Perspective camera active." << endl;
		break;

	case '3':
		cameraType = CONTROLLED_PERSP;
		updateCamera();
		cout << "Camera status: " << endl << " - Controllable perspective camera active." << endl;
		break;
	case 'p':
	case 'P':
		if (cameraType == ORTHOGRAPHIC || cameraType == PERSPECTIVE)
		{
			cameraType = CONTROLLED_PERSP;
			cout << "Camera status: " << endl << " - Controllable perspective camera active." << endl;
		}
		else if (cameraType == CONTROLLED_PERSP)
		{
			cameraType = ORTHOGRAPHIC;
			cout << "Camera status: " << endl << " - Ortographic camera active." << endl;
		}
		updateCamera();
		break;
	case 'c':
	case 'C':
		if (gimbalState == GIMBAL_LOCK_ON)
		{
			gimbalState = GIMBAL_LOCK_OFF;
			cout << "Gimbal status: " << endl << " - Camera isn't gimbal locked." << endl;
		}
		else if (gimbalState == GIMBAL_LOCK_OFF)
		{
			gimbalState = GIMBAL_LOCK_ON;
			cout << "Gimbal status: " << endl << " - Camera is gimbal locked." << endl;
		}
		updateCamera();
		break;
	case 'w':
	case 'W':
		centerY += 0.25;
		break;

	case 'a':
	case 'A':
		centerX -= 0.25;
		break;

	case 's':
	case 'S':
		centerY -= 0.25;
		break;
	case 'd':
	case 'D':
		centerX += 0.25;
		break;
	case '+':
		sceneLights[controllableLight]->position.y += 0.1f;
		break;
	case '-':
		sceneLights[controllableLight]->position.y -= 0.1f;
		break;
	case 'h':
	case 'H':
		sceneLights[controllableLight]->position.x -= 0.1f;
		cout << "Light " << controllableLight << ", pos: " << sceneLights[controllableLight]->position << endl;
		break;
	case 'j':
	case 'J':
		sceneLights[controllableLight]->position.x += 0.1f;
		cout << "Light " << controllableLight << ", pos: " << sceneLights[controllableLight]->position << endl;
		break;
	case 'k':
	case 'K':
		sceneLights[controllableLight]->position.z += 0.1f;
		cout << "Light " << controllableLight << ", pos: " << sceneLights[controllableLight]->position << endl;
		break;
	case 'l':
	case 'L':
		sceneLights[controllableLight]->position.z -= 0.1f;
		cout << "Light " << controllableLight << ", pos: " << sceneLights[controllableLight]->position << endl;
		break;
	case '\\':
	case '|':
		controllableLight = (controllableLight + 1) % sceneLights.size();
		cout << "Controlling light number: " << controllableLight << endl;
		break;
	case ' ':
		bomberman->placeBomb();
		break;
	case 'z':
		bomberman->debug();
		break;
	}
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		bomberman->playerWalk();
		break;
	case GLUT_KEY_DOWN:
		bomberman->playerWalkBackwards();
		break;
	case GLUT_KEY_LEFT:
		bomberman->rotatePlayerLeft();
		break;
	case GLUT_KEY_RIGHT:
		bomberman->rotatePlayerRight();
		break;
	case GLUT_KEY_F1:
		shadowRenderingActive = shadowRenderingActive ? false : true;
		break;
	}
}

/*********************************************************/
/*                                                       */
/*                         MOUSE                         */
/*                                                       */

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)
	{
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP)
	{
		if (tracking == 1)
		{
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2)
		{
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}

	updateCamera();
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{
	if (cameraType == CONTROLLED_PERSP && tracking != 0)
	{
		int deltaX, deltaY;
		float alphaAux, betaAux;
		float rAux;

		deltaX = -xx + startX;
		deltaY = yy - startY;

		// left mouse button: move camera
		if (tracking == 1)
		{
			alphaAux = alpha + deltaX;
			betaAux = beta + deltaY;

			// being used by quaternions to rotate the camera
			rotateX = alphaAux;
			rotateY = betaAux;

			if (betaAux > 85.0f)
				betaAux = 85.0f;
			else if (betaAux < -85.0f)
				betaAux = -85.0f;
			rAux = r;
		}
		// right mouse button: zoom
		else if (tracking == 2)
		{
			alphaAux = alpha;
			betaAux = beta;

			rAux = r + (deltaY * 0.01f);

			if (rAux < 0.1f)
				rAux = 0.1f;

			// being used by quaternions to zoom the camera
			zoom = rAux;
		}

		eyeX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
		eyeZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
		eyeY = rAux * sin(betaAux * 3.14f / 180.0f);

		//  not using an idle func
		//glutPostRedisplay();
	}
}

/////////////////////////////////////////////////////////////////////// CALLBACKs

void cleanup()
{
	shader->destroyShaderProgram();
	clearObjectsFromBuffer();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
}

void timedRedisplay(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timedRedisplay, 0);
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;

	if (WinY == 0)
		WinY = 1;
	viewportRatio = WinX * 1.0 / WinY;

	updateCamera();

	glViewport(0, 0, WinX, WinY);

	//always generate 2 framebuffers at the start of application to allow easier
	//switching between number of shadow calculations
	for (int i = 0; i < min((int)sceneLights.size(), 2); i++)
	{
		shadowRenderers[i]->generateShadowFBO(WinX, WinY);
	}

	postProcessRenderer->generatePostProcessFBO(WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, timedRedisplay, 0);
	glutTimerFunc(0, flashTimer, 0);
}

void setupOpenGL()
{
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1)
	{
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	createProgram();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	std::cout << std::boolalpha;
	srand(static_cast <unsigned> (time(0)));
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////