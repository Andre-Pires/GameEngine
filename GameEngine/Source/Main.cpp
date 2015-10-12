///////////////////////////////////////////////////////////////////////
//
// Assignment 1 consists in the following:
//
// - Download the libraries: Glew and FreeGlut for your system.
// - Create a project to compile, link and run the code provided in this section in your favourite programming environment (course will use VS2015).
// - Update your graphics drivers to their latest versions.
// - Verify what OpenGL contexts your computer can support, a minimum of OpenGL 3.3 support is required for this course.
//
// Further suggestions to verify your understanding of the concepts explored:
//
// - Change the program so display is called at 60 FPS.
//
// (c)2013-15 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include "Vector3f.h"
#include "Vector2f.h"
#include "Matrix3f.h"
#include "MatrixFactory.h"
#include "Scene.h"
#include "Matrix2f.h"
#include "Shader.h"
#include "Utilities.h"
#include "GameObject.h"
#include "BufferObjects.h"
#include "Triangle.h"
#include "Square.h"

#include "..\Dependencies\glew\glew.h"
#include "..\Dependencies\freeglut\freeglut.h"

#define CAPTION "Game Engine"
#include "Diamond.h"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

BufferObjects * bufferObjects;
Shader  * shader;
Scene * scene;

GameObject * triangle[5];
GameObject * square;
GameObject * diamond;

/////////////////////////////////////////////////////////////////////// SCENE
void createTangram()
{
	for (int i = 0; i < 5; i++)
	{
		triangle[i] = new Triangle(bufferObjects, scene);
	}

	//head
	diamond = new Diamond(bufferObjects, scene);
	diamond->scale(Vector3f(0.5, 0.5, 1.0));
	diamond->translate(Vector3f(0.0, 0.4, 0.0));
	diamond->changeColor(YELLOW);

	//right ear
	triangle[0]->scale(Vector3f(0.5, 0.5, 1.0));
	triangle[0]->rotate(-45, Vector3f(0.0, 0.0, 1.0));
	triangle[0]->translate(Vector3f(0, 0.75, 0.0));
	triangle[0]->changeColor(RED);

	//left ear
	triangle[1]->scale(Vector3f(0.5, 0.5, 1.0));
	triangle[1]->rotate(135, Vector3f(0.0, 0.0, 1.0));
	triangle[1]->translate(Vector3f(0, 0.75, 0.0));
	triangle[1]->changeColor(RED);

	//neck
	triangle[2]->rotate(-45, Vector3f(0.0, 0.0, 1.0));
	triangle[2]->translate(Vector3f(-0.2, 0.6, 0.0));
	triangle[2]->scale(Vector3f(0.5, 0.5, 1.0));
	triangle[2]->changeColor(GREEN);

	//body
	triangle[3]->rotate(135, Vector3f(0.0, 0.0, 1.0));
	triangle[3]->translate(Vector3f(0.458, 0.2, 0.0));
	triangle[3]->scale(Vector3f(0.75, 0.75, 1.0));
	triangle[3]->changeColor(BLUE);

	//legs
	triangle[4]->rotate(90, Vector3f(0.0, 0.0, 1.0));
	triangle[4]->translate(Vector3f(0.46, -0.3, 0.0));
	triangle[4]->scale(Vector3f(0.75, 0.75, 1.0));
	triangle[4]->changeColor(PURPLE);

	//tail
	square = new Square(bufferObjects, scene);
	square->shear(1.25, 0);
	square->scale(Vector3f(0.3, 0.3, 1));
	square->translate(Vector3f(0.345, -0.225, 0));
	square->changeColor(ORANGE);
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

	//creating our figure's objects
	createTangram();
}

void drawScene()
{
	for (int i = 0; i < 5; i++)
	{
		triangle[i]->draw();
	}

	square->draw();

	diamond->draw();
}

/////////////////////////////////////////////////////////////////////// UTILITIES

void clearObjectsFromBuffer()
{
	for (int i = 0; i < 5; i++)
	{
		triangle[i]->clearObjectFromBuffer();
	}

	square->clearObjectFromBuffer();
	diamond->clearObjectFromBuffer();
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

void setupOpenGL() {
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

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
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
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

//Note: Added slack values for floating point comparison
void vectorClassTesting()
{
	{
		Vector3f vec0 = Vector3f(0, 0, 0);
		Vector3f vec1 = Vector3f(1, 1, 1);
		Vector3f vec2 = Vector3f(2, 2, 2);
		Vector3f vec3 = Vector3f(3, 3, 3);

		cout << "\nTesting 3D vectors... \n";
		cout << "Standard operations \n\n";
		cout << "v1 = v1 : " << (vec1 == vec1) << ".\n";
		cout << "v1 + (v2 + v3) = (v1 + v2) + v3 : " << ((vec1 + (vec2 + vec3)) == ((vec1 + vec2) + vec3)) << ".\n";
		cout << "v1 + v0 = v1 : " << ((vec1 + vec0) == vec1) << ".\n";
		cout << "v1 + (-v1) = v0 : " << ((vec1 - vec1) == vec0) << ".\n";
		cout << "a(v1 + v2) = a * v1 + a * v2 : " << ((3 * (vec1 + vec2)) == ((3 * vec1) + (3 * vec2))) << ".\n";
		cout << "(a + b) * v1 = a * v1 + b * v1 : " << (((1 + 2) * (vec1)) == ((1 * vec1) + (2 * vec1))) << ".\n";
		cout << "(a*b) * v1 = a (b * v1) : " << (((1 * 2) * (vec1)) == (1 * (2 * vec1))) << ".\n";
		cout << "1 * v1 = v1 : " << (1 * vec2 == vec2) << ".\n";
		cout << "v3 / 3 = v1 : " << (vec3 / 3 == vec1) << ".\n";
		cout << "length of v3 = 5.19615 :  " << ((abs(vec3.Norm() - 5.19615F)) < 0.00001f) << ".\n";

		cout << "\nDot Product \n\n";
		cout << "v1 . v1 = |v1|^2 : " << (abs(vec2.Dot(vec2) - pow(vec2.Norm(), 2)) < 0.000001f) << ".\n";
		cout << "v1 . v2 = v2 . v1 : " << (vec1.Dot(vec2) == vec2.Dot(vec1)) << ".\n";
		cout << "v1 . (v2 + v3) = v1 . v2 + v1 . v3 : " << (vec1.Dot(vec2 + vec3) == (vec1.Dot(vec2) + vec1.Dot(vec3))) << ".\n";
		cout << "(a * v1) . v2 = a * (v1 . v2) : " << (((3 * vec1).Dot(vec2)) == (3 * vec1.Dot(vec2))) << ".\n";
		cout << "v0 . v1 = 0 : " << (vec1.Dot(vec0) == 0) << ".\n";
		cout << "v1 . v2 = |v1||v2|*cos(teta) : " << (abs(vec1.Dot(vec2) - (vec1.Norm() * vec2.Norm() * cosf(0))) < 0.000001f) << ".\n";

		cout << "\nCross Product \n\n";
		cout << "v2 x v3 = -v2 x v3 : " << (vec2.Cross(vec3) == (vec2 * -1).Cross(vec3)) << ".\n";
		cout << "v2 x v2 = v0 : " << (vec2.Cross(vec2) == vec0) << ".\n";
		cout << "v2 x (v1 + v3) = v2 x v1 + v2 x v3 : " << (vec2.Cross(vec1 + vec3) == vec2.Cross(vec1) + vec2.Cross(vec3)) << ".\n";
		cout << "v2 x (a * v3) = a * (v2 x v3) : " << (vec2.Cross(2 * vec3) == (2 * vec2.Cross(vec3))) << ".\n";
		cout << "(-1, 7, 4) x (-5, 8, 6) = (10, -14, 27) : " << (Vector3f(-1, 7, 4).Cross(Vector3f(-5, 8, 6)) == Vector3f(10, -14, 27)) << ".\n";

		cout << "\nAssignments \n\n";
		(vec3 /= 3);
		cout << "v3 /= 3 <=> v3 = v1 : " << (vec1 == vec3) << ".\n";
		(vec3 *= 3);
		(vec1 += vec2);
		cout << "v1 += v2 <=> v1 = v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		(vec1 = vec3);
		cout << "v1 = v3 => v1 == v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		(vec1 *= 3);
		cout << "v1 *= 3 => v1 == v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		cout << "-v1 == (-1,-1,-1) : " << (-vec1 == Vector3f(-1, -1, -1)) << ".\n";

		cout << "\n3D Vectors done\n";
	}

	{
		Vector2f vec0 = Vector2f(0, 0);
		Vector2f vec1 = Vector2f(1, 1);
		Vector2f vec2 = Vector2f(2, 2);
		Vector2f vec3 = Vector2f(3, 3);

		cout << "\nTesting 2D vectors... \n";
		cout << "Standard operations \n\n";
		cout << "v1 = v1 : " << (vec1 == vec1) << ".\n";
		cout << "v1 + (v2 + v3) = (v1 + v2) + v3 : " << ((vec1 + (vec2 + vec3)) == ((vec1 + vec2) + vec3)) << ".\n";
		cout << "v1 + v0 = v1 : " << ((vec1 + vec0) == vec1) << ".\n";
		cout << "v1 + (-v1) = v0 : " << ((vec1 - vec1) == vec0) << ".\n";
		cout << "a(v1 + v2) = a * v1 + a * v2 : " << ((3 * (vec1 + vec2)) == ((3 * vec1) + (3 * vec2))) << ".\n";
		cout << "(a + b) * v1 = a * v1 + b * v1 : " << (((1 + 2) * (vec1)) == ((1 * vec1) + (2 * vec1))) << ".\n";
		cout << "(a*b) * v1 = a (b * v1) : " << (((1 * 2) * (vec1)) == (1 * (2 * vec1))) << ".\n";
		cout << "1 * v1 = v1 : " << (1 * vec2 == vec2) << ".\n";
		cout << "v3 / 3 = v1 : " << (vec3 / 3 == vec1) << ".\n";
		cout << "length of v3 = 4.24264 :  " << ((abs(vec3.Norm() - 4.24264F)) < 0.00001f) << ".\n";

		cout << "\nDot Product \n\n";
		cout << "v1 . v1 = |v1|^2 : " << (abs(vec2.Dot(vec2) - pow(vec2.Norm(), 2)) < 0.000001f) << ".\n";
		cout << "v1 . v2 = v2 . v1 : " << (vec1.Dot(vec2) == vec2.Dot(vec1)) << ".\n";
		cout << "v1 . (v2 + v3) = v1 . v2 + v1 . v3 : " << (vec1.Dot(vec2 + vec3) == (vec1.Dot(vec2) + vec1.Dot(vec3))) << ".\n";
		cout << "(a * v1) . v2 = a * (v1 . v2) : " << (((3 * vec1).Dot(vec2)) == (3 * vec1.Dot(vec2))) << ".\n";
		cout << "v0 . v1 = 0 : " << (vec1.Dot(vec0) == 0) << ".\n";
		cout << "v1 . v2 = |v1||v2|*cos(teta) : " << (abs((vec1.Dot(vec2) - (vec1.Norm() * vec2.Norm() * cosf(0)))) < 0.000001f) << ".\n";

		cout << "\nAssignments \n\n";
		(vec3 /= 3);
		cout << "v3 /= 3 <=> v3 = v1 : " << (vec1 == vec3) << ".\n";
		(vec3 *= 3);
		(vec1 += vec2);
		cout << "v1 += v2 <=> v1 = v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		(vec1 = vec3);
		cout << "v1 = v3 => v1 == v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		(vec1 *= 3);
		cout << "v1 *= 3 => v1 == v3 : " << (vec1 == vec3) << ".\n";
		(vec1 /= 3);
		cout << "-v1 == (-1,-1) : " << (-vec1 == Vector2f(-1, -1)) << ".\n";

		cout << "\n2D Vectors done\n";
	}
}

void assignment1()
{
	Vector3f a = Vector3f(-1.0f, -2.0f, -3.0f);
	Vector3f b = Vector3f(0.5f, 0.4f, 0.3f);
	Vector3f c = Vector3f(2.0f, 2.0f, 2.0f);
	Vector3f d = Vector3f(0.0f, 0.0f, 0.0f);

	d = (b + c);
	cout << "1. d = b + c: " << d.x << ", " << d.y << ", " << d.z << ".\n";

	d = Vector3f(0, 0, 0);
	d = (-b) - (2.5f * c);
	cout << "2. d = -b - 2.5 * c: " << d.x << ", " << d.y << ", " << d.z << ".\n";

	d = Vector3f(0, 0, 0);
	d = (a.Dot(a)) * c + (b.Cross(c));
	cout << "3. d = (a.a) * c + (b x c) : " << d.x << ", " << d.y << ", " << d.z << ".\n";

	d = Vector3f(0, 0, 0);
	d = (a.Dot(b.Cross(c))) * (a.Cross(b.Cross(c)));
	cout << "4. d = (a . (b x c)) * (a x (b x c)) : " << d.x << ", " << d.y << ", " << d.z << ".\n";
}

void matrixClassTesting()
{
	{
		//matrix 3d

		//these vectors appear transposed due to column major ordering
		float tempArray[9] = {
			1,1,1,
			1,1,1,
			1,1,1 };
		float tempAssimetricArray[9] = {
			1,2,3,
			0,1,4,
			5,6,0 };
		float tempIncrementalArray[9] = {
			1,4,7,
			2,5,8,
			3,6,9 };

		float test[9] = {
			1,2,3,
			0,4,5,
			1,0,6 };
		Matrix3f mat1 = Matrix3f(tempArray);
		Matrix3f mat2 = Matrix3f(tempArray);
		cout << "\nTesting 3D matrices... \n";
		cout << "Standard operations \n\n";
		mat1 += mat1;
		cout << "Sum (1 + 1): " << mat1;
		mat1 -= mat2;
		cout << "Subtraction (2 - 1): " << mat1;

		cout << "Multiplication (1 * 3): " << (mat1 * 3);
		(mat1 *= 3);
		cout << "Multiplication atribution (1 * 3): " << mat1;

		cout << "Division (1 / 3): " << (mat1 / 3);
		(mat1 /= 3);
		cout << "Division atribution (1 / 3): " << mat1;

		cout << "\n Done \n\n";

		cout << "Matrix exclusive operations \n\n";
		mat1 *= 3;
		mat2 *= 4;
		cout << "m1 : " << mat1;
		cout << "m2 : " << mat2;
		cout << "Multiplication (3 * 4): " << (mat1 * mat2);

		mat1 = tempAssimetricArray;
		cout << "Transpose: " << mat1;
		cout << "to: " << mat1.Transpose();

		cout << "Determinant should be 1 and is : " << mat1.Determinant() << endl << endl;

		cout << "Adjoint: " << mat1.Adjoint();

		cout << "Inverse: " << mat1.Inverse();

		mat1 = test;
		cout << "Another Inverse: " << mat1.Inverse();

		Vector3f mult = Vector3f(2, 1, 3);
		mat1 = tempIncrementalArray;
		cout << "Multiplying vector by matrix: " << mat1 * mult;
		cout << "\n Done \n\n";
	}

	//matrix 2d
	{
		Matrix2f mat1 = {
			1,1,
			1,1 };

		Matrix2f mat2 = {
			1,1,
			1,1 };

		Matrix2f matrix2d = {
			1,2,
			3,4 };

		float tempAssimetricArray[4] = {
			1,2,
			3,0, };

		Vector2f vec2d = {
			3,2 };

		Matrix2f matInvert = Matrix2f(matrix2d);
		cout << "\nTesting 2D matrices... \n";
		cout << "Standard operations \n\n";
		mat1 += mat1;
		cout << "Sum (1 + 1): " << mat1;
		mat1 -= mat2;
		cout << "Subtraction (2 - 1): " << mat1;

		cout << "Multiplication (1 * 3): " << (mat1 * 3);
		(mat1 *= 3);
		cout << "Multiplication atribution (1 * 3): " << mat1;

		cout << "Division (1 / 3): " << (mat1 / 3);
		(mat1 /= 3);
		cout << "Division atribution (1 / 3): " << mat1;

		cout << "\n Done \n\n";

		cout << "Matrix exclusive operations \n\n";
		mat1 *= 3;
		mat2 *= 4;
		cout << "m1 : " << mat1;
		cout << "m2 : " << mat2;
		cout << "Multiplication (3 * 4): " << (mat1 * mat2);

		mat1 = tempAssimetricArray;
		cout << "Transpose: " << mat1;
		cout << "to: " << mat1.Transpose();

		cout << "Determinant should be -2 and is : " << matInvert.Determinant() << endl << endl;

		cout << "Adjoint: " << matInvert.Adjoint();

		cout << "Inverse of: " << matInvert;
		cout << "is: " << matInvert.Inverse();

		cout << "Multiplying vector by matrix: " << matInvert * vec2d;
	}
}

void assignment2()
{
	Vector3f p = { 1.0, 1.0, 0.0 };
	float tx = 0.50;
	float ty = 0.25;
	float s = 2.5;
	float theta = 0.785398;

	Matrix3f A = new float[9]{
		1.0f,0.0f,0.0f,
		0.0f,2.0f,0.0f,
		0.0f,0.0f,3.0f
	};

	Matrix3f B = new float[9]{
		1.0f,0.0f,0.0f,
		2.0f,4.0f,0.0f,
		3.0f,5.0f,6.0f
	};

	Matrix3f C = new float[9]{
		1.0f,2.0f,4.0f,
		0.0f,3.0f,5.0f,
		0.0f,0.0f,6.0f
	};

	Matrix3f T = new float[9]{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		tx  , ty ,1.0f
	};

	Matrix3f S = new float[9]{
		 s  ,0.0f,0.0f,
		0.0f, s  ,0.0f,
		0.0f,0.0f,  s
	};

	Matrix3f R = new float[9]{
		cos(theta) ,sin(theta),0.0f,
		-sin(theta),cos(theta),0.0f,
		0.0f       ,   0.0f   ,1.0f
	};

	cout << "1: " << -A + (2.5f * B * C) << endl;

	cout << "2: " << (C.Determinant() * B.Transpose()) + (C.Inverse().Determinant() * B) << endl;

	cout << "3: " << (T * S * R * p) << endl;

	cout << "4: " << (R.Transpose() * R * p) - (R.Inverse() * R * p) << endl;

	//todo - just testing matrix factory

	//check
	cout << "3 to 4 " << MatrixFactory::Mat3to4(B) << endl;

	//check
	cout << "4 to 3 " << MatrixFactory::Mat4to3(Matrix4f(new float[16]{
		0,2,0,0,
		0,-7,0,0,
		8,0,4,0,
		0,0,92,0 })) << endl;
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
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