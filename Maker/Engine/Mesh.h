#pragma once
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GameObject.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include "Transform.h"
#include <vector>
#include <memory>
#include <string>

#include "BufferObject.h"

class Mesh
{
	std::vector<glm::vec3> _vertices;
	std::vector<unsigned int> _indices;

	BufferObject _vertices_buffer;
	BufferObject _indices_buffer;
	BufferObject _texCoords_buffer;
	BufferObject _normals_buffer;
	BufferObject _colors_buffer;

	unsigned int texture_id = 0;

public:

	const auto& vertices() const { return _vertices; }
	const auto& indices() const { return _indices; }

	void load(const glm::vec3* vertices, size_t num_verts, unsigned int* indices, size_t num_indexs);
	void loadTexCoords(const glm::vec2* tex_coords, size_t num_tex_coords);
	void loadNormals(const glm::vec3* normals, size_t num_normals);
	void loadColors(const glm::u8vec3* colors, size_t num_colors);
	void draw() const;

	// Load Meshes from a file
	void LoadFile(const char* file_path);

	//
	void CheckerTexture();

	// Load Texture
	void LoadTexture(const std::string& path);

	

};

