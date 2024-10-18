#include "Mesh.h"

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), num_vertex(0), num_index(0) {}

Mesh::~Mesh() {
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	delete[] vertex;
	delete[] index;
}

std::vector<Mesh*> Mesh::LoadFile(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	std::vector<Mesh*> meshes;

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array 
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			Mesh* ourMesh = new Mesh();
			ourMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
			ourMesh->vertex = new float[ourMesh->num_vertex * 3];
			memcpy(ourMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * ourMesh->num_vertex * 3);

			if (scene->mMeshes[i]->HasFaces())
			{
				ourMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
				ourMesh->index = new unsigned int[ourMesh->num_index];

				for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					memcpy(&ourMesh->index[i * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
				}
			}

			BufferData();
			meshes.push_back(ourMesh);
		}

		aiReleaseImport(scene);


	}
	else {
		//LOG(“Error loading scene % s”, file_path);
	}

	return meshes;
}

void Mesh::BufferData()
{
	glGenVertexArrays(1, &id_vertex);
	glGenBuffers(1, &id_vertex);
	

	glBindVertexArray(id_vertex);

	glBufferData(GL_ARRAY_BUFFER, num_vertex * 3 * sizeof(float), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	if (index) {
		glGenBuffers(1, &id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_index, index, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


}

void Mesh::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindVertexArray(id_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glDrawElements(GL_TRIANGLES, num_index, GL_FLOAT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	
	glBindVertexArray(0);
	

}




