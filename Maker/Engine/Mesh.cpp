#include "Mesh.h"

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), num_vertex(0), num_index(0) {}

Mesh::~Mesh() {
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	delete[] vertex;
	delete[] index;
}

void Mesh::LoadFile(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array 
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			
			num_vertex = scene->mMeshes[i]->mNumVertices;
			vertex = new float[num_vertex * 3];
			memcpy(vertex, scene->mMeshes[i]->mVertices, sizeof(float) * num_vertex * 3);

			if (scene->mMeshes[i]->HasFaces())
			{
				num_index = scene->mMeshes[i]->mNumFaces * 3;
				index = new unsigned int[num_index];

				for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					memcpy(&index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
				}
			}

			BufferData();
			
		}

		aiReleaseImport(scene);


	}
	else {
		//LOG(“Error loading scene % s”, file_path);
	}

	
}

void Mesh::BufferData()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, num_vertex * 3 * sizeof(float), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_index * sizeof(unsigned int), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	

}




