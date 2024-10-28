#include <iostream>
using namespace std;
#include "Engine/GameObject.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <imgui.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Engine/Camera.h"
#include "Engine/Mesh.h"
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <exception>
#include "Engine/MyWindow.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_events.h>

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera camera;
static GameObject go;

static bool middleMousePressed = false;
static ivec2 lastMousePosition;

static void drawFloorGrid(int size, double step) {
	glBegin(GL_LINES);
	for (double i = -size; i <= size; i += step) {
		glVertex3d(i, 0, -size);
		glVertex3d(i, 0, size);
		glVertex3d(-size, 0, i);
		glVertex3d(size, 0, i);
	}
	glEnd();
}

void configureCamera() {
	glm::dmat4 projectionMatrix = camera.projection();
	glm::dmat4 viewMatrix = camera.view();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(glm::value_ptr(viewMatrix));
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	configureCamera();
	drawFloorGrid(16, 0.25);
	go.draw();
}

static void init_opengl() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not available.");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
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

static void mouseButton_func(int button, int state, int x, int y) {
	if (button == SDL_BUTTON_MIDDLE) {
		middleMousePressed = (state == SDL_PRESSED);
		if (middleMousePressed) {
			lastMousePosition = ivec2(x, y); // Guardar la posición inicial del ratón
		}
	}
}

static void mouseMotion_func(int x, int y) {
	if (middleMousePressed) {
		// Calcular el movimiento basado en la diferencia de la posición del ratón
		ivec2 delta = ivec2(x, y) - lastMousePosition;
		lastMousePosition = ivec2(x, y);

		// Mover la cámara basado en el movimiento del ratón
		camera.transform().translate(vec3(delta.x * 0.01, 0, -delta.y * 0.01));
	}
}

const char* checkFile(char* file) {
	string filePath = file;
	string extension;
	extension = filePath[filePath.size() - 3] + filePath[filePath.size() - 2] + filePath[filePath.size() - 1];
	const char* ext = extension.data();
	return ext;
}

int main(int argc, char* argv[]) {
	ilInit();
	iluInit();
	ilutInit();
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	init_opengl();

	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	SDL_Event event;
	char* dropped_filePath;
	auto mesh = make_shared<Mesh>();
	auto imageTexture = make_shared<Image>();

	while (window.processEvents() && window.isOpen()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_DROPFILE:
				dropped_filePath = event.drop.file;
				mesh->LoadFile(dropped_filePath);
				imageTexture->LoadTexture("Baker_house.png");
				go.setMesh(mesh);
				go.setTextureImage(imageTexture);
				SDL_free(dropped_filePath);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				mouseButton_func(event.button.button, event.button.state, event.button.x, event.button.y);
				break;
			case SDL_MOUSEMOTION:
				mouseMotion_func(event.motion.x, event.motion.y);
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}
