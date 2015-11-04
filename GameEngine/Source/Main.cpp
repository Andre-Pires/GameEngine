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
#include "SceneGraphNode.h"

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"

#define CAPTION "Game Engine"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;
float ratio = 1.33;

BufferObjects* bufferObjects;
Shader* shader;
Scene* scene;
Camera* camera;

// Camera Position
float eyeX = 0;
float eyeY = 0;
float eyeZ = 5;
float centerX, centerY, centerZ = 0;
CameraType cameraType = ORTHOGRAPHIC;
GimbalLockState gimbalState = GIMBAL_LOCK_OFF;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 5.25f;
float rotateX, rotateY, zoom = 0.0f;

SceneGraphNode* sceneGraph;
SceneGraphNode* tangramNode;

/////////////////////////////////////////////////////////////////////// SCENE
// correct order -> scale * rotation * translation
void createTangram()
{
	tangramNode = new SceneGraphNode(sceneGraph);

	///NOTE: linhas com as posições finais
	//	//head
	//	GeometricObject * diamond = new Diamond(bufferObjects, scene);
	//	diamond->scale(Vector3f(0.5, 0.5, 0.5));
	//	diamond->translate(Vector3f(0.0, 0.5, 0.0));
	//	diamond->changeColor(YELLOW);
	//	diamond->shadeColor();
	//	tangramNode->add(new SceneGraphNode(sceneGraph, diamond));
	//
	//	//right ear
	//	{
	//		GeometricObject * triangle = new Triangle(bufferObjects, scene);
	//		triangle->scale(Vector3f(0.5, 0.5, 0.4));
	//		triangle->rotate(-135, Vector3f(0.0, 0.0, 1.0));
	//		triangle->translate(Vector3f(0.35, 1.55, 0.0));
	//		triangle->changeColor(RED);
	//		triangle->shadeColor();
	//		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	//	}
	//
	//	//left ear
	//	{
	//		GeometricObject * triangle = new Triangle(bufferObjects, scene);
	//		triangle->scale(Vector3f(0.5, 0.5, 0.4));
	//		triangle->rotate(45, Vector3f(0.0, 0.0, 1.0));
	//		triangle->translate(Vector3f(-0.35, 0.85, 0.0));
	//		triangle->changeColor(RED);
	//		triangle->shadeColor();
	//		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	//	}
	//
	//	//neck
	//	{
	//		GeometricObject * triangle = new Triangle(bufferObjects, scene);
	//		triangle->scale(Vector3f(0.7143, 0.7143, 0.4));
	//		triangle->rotate(-135, Vector3f(0.0, 0.0, 1.0));
	//		triangle->translate(Vector3f(0.25, 0.75, 0.0));
	//		triangle->changeColor(GREEN);
	//		triangle->shadeColor();
	//		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	//	}
	//
	//	//body
	//	{
	//		GeometricObject * triangle = new Triangle(bufferObjects, scene);
	//		triangle->scale(Vector3f(1, 1, 0.6));
	//		triangle->rotate(45, Vector3f(0.0, 0.0, 1.0));
	//		triangle->translate(Vector3f(0.25, -0.65, 0.0));
	//		triangle->changeColor(BLUE);
	//		triangle->shadeColor();
	//		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	//	}
	//
	//	//legs
	//	{
	//		GeometricObject * triangle = new Triangle(bufferObjects, scene);
	//		triangle->scale(Vector3f(1, 1, 0.4));
	//		triangle->rotate(0, Vector3f(0.0, 0.0, 1.0));
	//		triangle->translate(Vector3f(-0.05, -0.95, 0.0));
	//		triangle->changeColor(PURPLE);
	//		triangle->shadeColor();
	//		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	//	}
	//
	//	//tail
	//	GeometricObject * square = new Square(bufferObjects, scene);
	//	square->scale(Vector3f(0.7143, 0.5, 0.6));
	//	square->shear(1.15, 0);
	//	square->translate(Vector3f(0.95, -0.95, 0));
	//	square->changeColor(ORANGE);
	//	square->shadeColor();
	//	tangramNode->add(new SceneGraphNode(sceneGraph, square));
	//
	//	sceneGraph->add(tangramNode);
	///NOTE: linhas com as posições finais

	//head
	GeometricObject* diamond = new Diamond(bufferObjects, scene);
	diamond->scale(Vector3f(0.5, 0.5, 0.5));
	diamond->translate(Vector3f(0.36, -1.15, 0.0));
	diamond->changeColor(YELLOW);
	diamond->shadeColor();
	tangramNode->add(new SceneGraphNode(sceneGraph, diamond));

	//right ear
	{
		GeometricObject* triangle = new Triangle(bufferObjects, scene);
		triangle->scale(Vector3f(0.5, 0.5, 0.4));
		triangle->rotate(-135, Vector3f(0.0, 0.0, 1.0));
		triangle->translate(Vector3f(0.7143, -0.09, 0.0));
		triangle->changeColor(RED);
		triangle->shadeColor();
		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	}

	//left ear
	{
		GeometricObject* triangle = new Triangle(bufferObjects, scene);
		triangle->scale(Vector3f(0.5, 0.5, 0.4));
		triangle->rotate(135, Vector3f(0.0, 0.0, 1.0));
		triangle->translate(Vector3f(0.36, -1.15, 0.0));
		triangle->changeColor(RED);
		triangle->shadeColor();
		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	}

	//neck
	{
		GeometricObject* triangle = new Triangle(bufferObjects, scene);
		triangle->scale(Vector3f(0.7143, 0.7143, 0.4));
		triangle->rotate(0, Vector3f(0.0, 0.0, 1.0));
		triangle->translate(Vector3f(0.0, -1.5, 0.0));
		triangle->changeColor(GREEN);
		triangle->shadeColor();
		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	}

	//body
	{
		GeometricObject* triangle = new Triangle(bufferObjects, scene);
		triangle->scale(Vector3f(1, 1, 0.6));
		triangle->rotate(45, Vector3f(0.0, 0.0, 1.0));
		triangle->translate(Vector3f(-0.7, -1.5, 0.0));
		triangle->changeColor(BLUE);
		triangle->shadeColor();
		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	}

	//legs
	{
		GeometricObject* triangle = new Triangle(bufferObjects, scene);
		triangle->scale(Vector3f(1, 1, 0.4));
		triangle->rotate(-45, Vector3f(0.0, 0.0, 1.0));
		triangle->translate(Vector3f(-0.7, -0.09, 0.0));
		triangle->changeColor(PURPLE);
		triangle->shadeColor();
		tangramNode->add(new SceneGraphNode(sceneGraph, triangle));
	}

	//tail
	GeometricObject* square = new Square(bufferObjects, scene);
	square->scale(Vector3f(0.71, 0.35, 0.6));
	square->shear(1.0, 0);
	square->translate(Vector3f(-0.7, -1.5, 0));
	square->changeColor(ORANGE);
	square->shadeColor();
	tangramNode->add(new SceneGraphNode(sceneGraph, square));

	sceneGraph->add(tangramNode);

	//ground
	GeometricObject* ground = new Plane(bufferObjects, scene);
	ground->scale(Vector3f(5.0, 5.0, 1.0));
	ground->translate(Vector3f(-2.5, -2.5, -1.01));
	sceneGraph->add(new SceneGraphNode(sceneGraph, ground));
}

//interpolation : alpha * x + (1 - alpha) * y
void interpolateTangram(float ratio)
{
	//TODO: figure out way to interpolate
	sceneGraph->rotate(ratio * 0 + (1 - ratio) * -135, Vector3f(0.0, 0.0, 1.0));
}

void createProgram()
{
	//creating and linking shader program with respective shaders
	shader = Shader::getInstance();
	shader->createShaderProgram();

	//generating vao
	bufferObjects = BufferObjects::getInstance();

	//creating new scene object for further drawing
	scene = new Scene();

	camera = new Camera(bufferObjects, scene);

	sceneGraph = new SceneGraphNode();

	//creating our figure's objects
	createTangram();
}

void updateCamera()
{
	if (cameraType == ORTHOGRAPHIC)
	{
		camera->ortho(-2.0f + centerX, 2.0f + centerX, -2.0f + centerY, 2.0f + centerY, -2.0f, 2.0f);
	}
	else if (cameraType == PERSPECTIVE)
	{
		camera->perspective(45.0f, ratio, 0.1f, 100.0f);

		if (gimbalState == GIMBAL_LOCK_ON)
		{
			// set the camera using a function similar to gluLookAt
			camera->lookAt(Vector3f(0, 0, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		}
		else if (gimbalState == GIMBAL_LOCK_OFF)
		{
			camera->quaternionLookAt(0, 0, 0, Vector3f(0, 0, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
		}
	}

	else if (cameraType == CONTROLLED_PERSP)
	{
		camera->perspective(45.0f, ratio, 0.1f, 100.0f);
		// set the camera using a function similar to gluLookAt

		if (gimbalState == GIMBAL_LOCK_ON)
		{
			// set the camera using a function similar to gluLookAt
			camera->lookAt(Vector3f(eyeX, eyeY, eyeZ), Vector3f(centerX, centerY, centerZ), Vector3f(0, 1, 0));
		}
		else if (gimbalState == GIMBAL_LOCK_OFF)
		{
			camera->quaternionLookAt(rotateX, rotateY, zoom, Vector3f(0, 0, 5), Vector3f(centerX, centerY, centerZ), Vector3f(0, 1, 0));
		}
	}

	camera->updateCamera();
}

void drawScene()
{
	if (cameraType == CONTROLLED_PERSP)
	{
		updateCamera();
	}

	sceneGraph->draw();
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
		tangramNode->scale(Vector3f(1.1f, 1.1f, 1.0f));
		break;
	case '-':
		tangramNode->scale(Vector3f(0.9f, 0.9f, 1.0f));
		break;
	case 'i':
	case 'I':
		interpolateTangram(0.25);
	}
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		sceneGraph->translate(Vector3f(0.0f, 0.1f, 0.0f));
		break;
	case GLUT_KEY_DOWN:
		sceneGraph->translate(Vector3f(0.0f, -0.1f, 0.0f));
		break;
	case GLUT_KEY_LEFT:
		sceneGraph->translate(Vector3f(-0.1f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_RIGHT:
		sceneGraph->translate(Vector3f(0.1f, 0.0f, 0.0f));
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
	ratio = WinX * 1.0 / WinY;

	updateCamera();

	glViewport(0, 0, WinX, WinY);
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
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////