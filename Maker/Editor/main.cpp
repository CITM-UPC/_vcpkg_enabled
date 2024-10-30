#include <iostream>
#include <string>
using namespace std;
#include "Engine/GameObject.h"
#include "Engine/Scene.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
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
#include "MyWindow.h"
#include "MyGUI.h"
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_events.h>
#include <glm/gtc/matrix_transform.hpp>



using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(1280, 720);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static Camera camera;
glm::dmat4 projectionMatrix;
glm::dmat4 viewMatrix;


static bool middleMousePressed = false;
static ivec2 lastMousePosition;
static bool rightMousePressed = false;
static bool leftMousePressed = false;
static vector <GameObject> gameObjects;
GameObject* selectedObject = nullptr;
static double moveSpeed = 0.1;
float yaw = 0.0f;
float pitch = 0.0f;
const float MAX_PITCH = 89.0f;
bool altKeyPressed = false;
vec3 target;
ivec2 delta;
int lastMouseX; 
int lastMouseY;


// Function to convert screen coordinates to world coordinates
glm::vec3 screenToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
	// Normalize mouse coordinates to [-1, 1]
	float x = (2.0f * mouseX) / screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / screenHeight;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	// Convert to homogeneous clip coordinates
	glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);

	// Convert to eye coordinates
	glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	// Convert to world coordinates
	glm::vec3 ray_wor = glm::vec3(glm::inverse(viewMatrix) * ray_eye);
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}

bool rayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingBox& bbox) {
	float tmin = (bbox.min.x - rayOrigin.x) / rayDir.x;
	float tmax = (bbox.max.x - rayOrigin.x) / rayDir.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (bbox.min.y - rayOrigin.y) / rayDir.y;
	float tymax = (bbox.max.y - rayOrigin.y) / rayDir.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (bbox.min.z - rayOrigin.z) / rayDir.z;
	float tzmax = (bbox.max.z - rayOrigin.z) / rayDir.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}

glm::vec3 getRayFromMouse(int mouseX, int mouseY, const glm::mat4& projection, const glm::mat4& view, const glm::ivec2& viewportSize) {
	float x = (2.0f * mouseX) / viewportSize.x - 1.0f;
	float y = 1.0f - (2.0f * mouseY) / viewportSize.y;
	glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

	glm::vec4 rayEye = glm::inverse(projection) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));
	return rayWorld;
}

GameObject* raycastFromMouseToGameObject(int mouseX, int mouseY, const glm::mat4& projection, const glm::mat4& view, const glm::ivec2& viewportSize) {
	glm::vec3 rayOrigin = glm::vec3(glm::inverse(view) * glm::vec4(0, 0, 0, 1));
	glm::vec3 rayDirection = getRayFromMouse(mouseX, mouseY, projection, view, viewportSize);

	GameObject* hitObject = nullptr;

	for (auto& go : gameObjects) {
		if (rayIntersectsBoundingBox(rayOrigin, rayDirection, go.boundingBox())) {
			hitObject = &go;
			break;
		}
	}

	return hitObject;
}

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
	projectionMatrix = camera.projection();
	viewMatrix = camera.view();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(glm::value_ptr(projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(glm::value_ptr(viewMatrix));
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	configureCamera();
	drawFloorGrid(16, 0.25);
	
	for (auto& child : scene.children()) {
		child.draw();
		child.transform().translate(glm::vec3(0, 0.001, 0));
	}

	scene.drawDebug(scene);

	cout << "Number of children: " << scene.children().size() << endl;
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

static void mouseWheel_func(int direction) {
	// Mueve la cámara hacia adelante o hacia atrás
	camera.transform().translate(vec3(0, 0, direction * 0.1));
}

glm::vec2 getMousePosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void orbitCamera(const vec3& target, int deltaX, int deltaY) {
	const float sensitivity = 0.1f;

	yaw += deltaX * sensitivity;
	pitch -= deltaY * sensitivity;
	float distance = glm::length(camera.transform().pos() - target);
	vec3 newPosition;
	newPosition.x = target.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newPosition.y = target.y + distance * sin(glm::radians(pitch));
	newPosition.z = target.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	camera.transform().pos() = newPosition;
	camera.transform().lookAt(target);
}

static void mouseButton_func(int button, int state, int x, int y) {
	if (button == SDL_BUTTON_MIDDLE) {
		middleMousePressed = (state == SDL_PRESSED);
		if (middleMousePressed) {
			lastMousePosition = ivec2(x, y);
		}
	}
	if (button == SDL_BUTTON_RIGHT) {
		rightMousePressed = (state == SDL_PRESSED);
	}
	if (button == SDL_BUTTON_LEFT) {
		if (leftMousePressed) {
			// Guarda la posición del ratón al hacer clic
			lastMousePosition = ivec2(x, y);
			glm::vec2 mouseScreenPos = getMousePosition();
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_SIZE.x / WINDOW_SIZE.y, 0.1f, 100.0f);
			glm::mat4 view = camera.view();
			selectedObject = raycastFromMouseToGameObject(mouseScreenPos.x, mouseScreenPos.y, projection, view, WINDOW_SIZE);

			// Establece el objetivo de la órbita
			if (selectedObject != nullptr) {
				target = selectedObject->transform().pos();
			}
			else {
				target = glm::vec3(0, 0, 0); 
			}
		}
		else {
			
			leftMousePressed = false;
		}
	}
}
static void handleKeyboardInput() {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (rightMousePressed) {

		if (state[SDL_SCANCODE_W]) {
			camera.transform().translate(glm::vec3(0, 0, moveSpeed));
		}
		if (state[SDL_SCANCODE_S]) {
			camera.transform().translate(glm::vec3(0, 0, -moveSpeed));
		}
		if (state[SDL_SCANCODE_A]) {
			camera.transform().translate(glm::vec3(moveSpeed, 0, 0));
		}
		if (state[SDL_SCANCODE_D]) {
			camera.transform().translate(glm::vec3(-moveSpeed, 0, 0));
		}
		if (state[SDL_SCANCODE_E]) {
			camera.transform().translate(glm::vec3(0, -moveSpeed, 0));
		}
		if (state[SDL_SCANCODE_Q]) {
			camera.transform().translate(glm::vec3(0, moveSpeed, 0));
		}
		if (state[SDL_SCANCODE_LSHIFT]) {
			moveSpeed *= 2;
		}
	}
	if (leftMousePressed) {

		if (leftMousePressed && state[SDL_SCANCODE_LALT]) {
			glm::vec2 mouseScreenPos = getMousePosition();
			ivec2 delta = ivec2(mouseScreenPos) - lastMousePosition; // calcula el cambio
			lastMousePosition = ivec2(mouseScreenPos); // actualiza la última posición del ratón

			orbitCamera(target, delta.x, delta.y); // llama a la función de órbita
		}
	}

}



static void mouseMotion_func(int x, int y) {
	if (middleMousePressed) {
		
		ivec2 delta = ivec2(x, y) - lastMousePosition;
		lastMousePosition = ivec2(x, y);

		
		camera.transform().translate(vec3(delta.x * 0.01, 0, -delta.y * 0.01));
		camera.transform().translate(vec3(0, delta.y * 0.01, 0));
	}

	if (leftMousePressed) {
		lastMousePosition = ivec2(x, y);
	}
}


std::string getFileExtension(const std::string& filePath) {
	// Find the last dot in the file path
	size_t dotPosition = filePath.rfind('.');

	// If no dot is found, return an empty string
	if (dotPosition == std::string::npos) {
		return "";
	}

	// Extract and return the file extension
	return filePath.substr(dotPosition + 1);
}

// Function to check if the mouse is over a GameObject
bool isMouseOverGameObject(const GameObject& go, int mouseX, int mouseY) {
	// Obtener la posición de la cámara (origen del rayo)
	glm::vec3 rayOrigin = camera.transform().pos();

	// Convertir las coordenadas del mouse a un rayo en el espacio del mundo
	glm::vec3 rayDir = screenToWorldRay(mouseX, mouseY, WINDOW_SIZE.x, WINDOW_SIZE.y, viewMatrix, projectionMatrix);

	// Obtener el bounding box del GameObject
	BoundingBox bbox = go.boundingBox();

	// Verificar si el rayo intersecta con el bounding box
	return rayIntersectsBoundingBox(rayOrigin, rayDir, bbox);
}

int main(int argc, char* argv[]) {
	
	
	ilInit();
	iluInit();
	ilutInit();
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);
	MyGUI gui(window.windowPtr(), window.contextPtr());
	init_opengl();

	camera.transform().pos() = vec3(0, 1, 4);
	camera.transform().rotate(glm::radians(180.0), vec3(0, 1, 0));

	SDL_Event event;
	char* dropped_filePath;
	auto mesh = make_shared<Mesh>();
	auto imageTexture = make_shared<Image>();
	std::string extension;

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	while (window.processEvents(&gui) && window.isOpen()) {
		const auto t0 = hrclock::now();
		handleKeyboardInput();
		display_func();
		gui.render();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if (dt < FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);

		while (SDL_PollEvent(&event))
		{
			gui.processEvent(event);
			//window.processEvents(&gui);
			
			switch (event.type) {
				case SDL_DROPFILE:
					dropped_filePath = event.drop.file;
					extension = getFileExtension(dropped_filePath);

					if (extension == "obj" || extension == "fbx" || extension == "dae") {
						mesh->LoadFile(dropped_filePath);
						GameObject go;
						go.setMesh(mesh);
						scene.emplaceChild(go);
					}
					else if (extension == "png" || extension == "jpg" || extension == "bmp") {
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						for (auto& child : scene.children()) {
							if (isMouseOverGameObject(child, mouseX, mouseY)) {
								imageTexture->LoadTexture(dropped_filePath);
								child.setTextureImage(imageTexture);
							}
						}
						
					}
					else {
						std::cerr << "Unsupported file extension: " << extension << std::endl;
					}
					SDL_free(dropped_filePath);
					//Hasta aquí
					break;
				case SDL_MOUSEBUTTONDOWN:
					
				case SDL_MOUSEBUTTONUP:
					mouseButton_func(event.button.button, event.button.state, event.button.x, event.button.y);
					break;
				case SDL_MOUSEMOTION:
					mouseMotion_func(event.motion.x, event.motion.y);
					break;

				case SDL_MOUSEWHEEL:
					mouseWheel_func(event.wheel.y);
					break;
			}
		}

		
	}

	return EXIT_SUCCESS;
}
