#pragma once
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BufferObject.h"
#include "BoundingBox.h"
#include "MeshLoader.h"



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

	BoundingBox _boundingBox;

	//MeshLoader* _meshLoader;

public:

	Mesh();

	const auto& vertices() const { return _vertices; }
	const auto& indices() const { return _indices; }
	const auto& boundingBox() const { return _boundingBox; }
	//const auto& meshLoader() const { return *_meshLoader; }

	void load(const glm::vec3* vertices, size_t num_verts, unsigned int* indices, size_t num_indexs);
	void loadTexCoords(const glm::vec2* tex_coords, size_t num_tex_coords);
	void loadNormals(const glm::vec3* normals, size_t num_normals);
	void loadColors(const glm::u8vec3* colors, size_t num_colors);
	void draw() const;
	void drawNormals(float length) const;

	void LoadFile(const char* filePath);

	//
	void CheckerTexture();
	void deleteCheckerTexture();

	
	

};

