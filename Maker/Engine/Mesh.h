#pragma once
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GameObject.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Transform.h"
#include <vector>
#include <memory>
#include <string>

class Mesh
{
	Transform transform;
	glm::u8vec3 color;

	unsigned int id_index = 0; // index in VRAM
	unsigned int num_index = 0;
	unsigned int* index = nullptr;

	unsigned int id_vertex = 0; // unique vertex in VRAM
	unsigned int num_vertex = 0;
	float* vertex = nullptr;

	bool drawWireframe = true;

private:
	unsigned int VAO, VBO, EBO;

public:

	Mesh();
	virtual ~Mesh();

	// Load Meshes from a file
	std::vector<Mesh*> LoadFile(const char* file_path);

	// Get Buffer data
	void BufferData();

	// Draw the mesh
	void Draw();

	

};

