#include <iostream>
using namespace std;
#include "Engine/GameObject.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Camera.h"
#include "Engine/Mesh.h"
#include <vector>
#include <array>
using namespace std;

static Camera camera;
static Mesh mesh;


static array< array<glm::u8vec3, 256>, 256> texture;

static void initializeTexture()
{
	for (int s = 0; s < texture.front().size(); ++s)
	{
		for (int t = 0; t < texture.size(); ++t)
		{
			const glm::u8vec3 whiteColor(255, 255, 255);
			const glm::u8vec3 blackColor(0, 0, 0);
			texture[s][t] = (s / 8 + t / 8) % 2 == 0 ? whiteColor : blackColor;
		}
	}
}

static void drawFloorGrid(int size, double step) {
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);
	for (double i = -size; i <= size; i += step) {
		glVertex3d(i, 0, -size);
		glVertex3d(i, 0, size);
		glVertex3d(-size, 0, i);
		glVertex3d(size, 0, i);
	}
	glEnd();
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&camera.view()[0][0]);

	drawFloorGrid(16, 0.25);
	mesh.Draw();

	glutSwapBuffers();
}

static void init_opengl() {
	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void reshape_func(int width, int height) {
	glViewport(0, 0, width, height);
	camera.aspect = static_cast<double>(width) / height;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(&camera.projection()[0][0]);
}

static void mouseWheel_func(int wheel, int direction, int x, int y) {
	camera.transform().translate(vec3(0, 0, direction * 0.1));
}

static void idle_func() {
	//animate triangles
	
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	// Iniit window and context
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Glut Simple Example");

	// Init OpenGL
	init_opengl();

	// Init camera
	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	// Debugger for the Model Loader
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// Init My Mesh
	mesh.LoadFile("BakerHouse.fbx");
	 
	
	

	// Init triangles
	/*red_triangle.transform.pos() = vec3(0, 1, 0);
	red_triangle.color = glm::u8vec3(255, 0, 0);
	green_triangle.transform.pos() = vec3(1, 1, 0);
	green_triangle.color = glm::u8vec3(0, 255, 0);
	blue_triangle.transform.pos() = vec3(0, 1, 1);
	blue_triangle.color = glm::u8vec3(255, 255, 255);*/

	// Init Texture

	ilInit();
	iluInit();
	auto il_img_id = ilGenImage();
	ilBindImage(il_img_id);
	//ilLoadImage("Lenna.png");
	auto img_width = ilGetInteger(IL_IMAGE_WIDTH);
	auto img_height = ilGetInteger(IL_IMAGE_HEIGHT);
	auto img_bpp = ilGetInteger(IL_IMAGE_BPP);
	auto img_format = ilGetInteger(IL_IMAGE_FORMAT);
	auto img_data = ilGetData();


	initializeTexture();
	unsigned int texture_id = 0;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.front().size(), texture.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data());
	glTexImage2D(GL_TEXTURE_2D, 0, img_bpp, img_width, img_height, 0, img_format, GL_UNSIGNED_BYTE, img_data);
	ilDeleteImage(il_img_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
	

	// Init cube
	//cube.initBuffers();



	// Set Glut callbacks
	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutMouseWheelFunc(mouseWheel_func);

	// Enter glut main loop
	glutMainLoop();

	return EXIT_SUCCESS;
}