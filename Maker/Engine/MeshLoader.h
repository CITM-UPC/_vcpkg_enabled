#pragma once


#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Mesh.h"
#include "TreeExt.h"

class MeshLoader
{

	//std::shared_ptr<Mesh> _mesh_ptr;

public:
	// Load Meshes from a file
	void LoadFile(const char* file_path);

	//const auto& GetMeshData() const { return *_mesh_ptr; }
};

