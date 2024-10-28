#include "MeshLoader.h"
#include "Mesh.h"

void MeshLoader::LoadFile(const char* file_path)
{
	//const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	//if (scene != nullptr && scene->HasMeshes()) {
	//	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
	//		size_t num_vertices = scene->mMeshes[i]->mNumVertices;
	//		glm::vec3* vertex = new glm::vec3[num_vertices * 3];
	//		memcpy(vertex, scene->mMeshes[i]->mVertices, sizeof(float) * num_vertices * 3);


	//		if (scene->mMeshes[i]->HasFaces()) {
	//			size_t num_index = scene->mMeshes[i]->mNumFaces * 3;
	//			unsigned int* index = new unsigned int[num_index]; // assume each face is a triangle
	//			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
	//				memcpy(&index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
	//			}
	//			_mesh_ptr->load(vertex, num_vertices, index, num_index);
	//		}

	//		if (scene->mMeshes[i]->HasTextureCoords(0)) {
	//			glm::vec2* texCoords = new glm::vec2[num_vertices];
	//			for (size_t j = 0; j < num_vertices; ++j) {
	//				texCoords[j] = glm::vec2(
	//					scene->mMeshes[i]->mTextureCoords[0][j].x,
	//					-scene->mMeshes[i]->mTextureCoords[0][j].y
	//				);
	//			}
	//			_mesh_ptr->loadTexCoords(texCoords, num_vertices);
	//			delete[] texCoords;
	//		}


	//	}
	//	aiReleaseImport(scene);

	//	
	//}
	//else {
	//	// Handle error
	//}


}
