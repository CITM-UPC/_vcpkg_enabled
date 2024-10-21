#include "Mesh.h"

#include <GL/glew.h>
using namespace std;

#define CHECKERS_HEIGHT 32
#define CHECKERS_WIDTH 32

void Mesh::load(const glm::vec3* vertices, size_t num_verts, unsigned int* indices, size_t num_indexs)
{
	_vertices_buffer.loadData(vertices, num_verts * sizeof(glm::vec3));
	_indices_buffer.loadIndices(indices, num_indexs);
	_texCoords_buffer.unload();
	_normals_buffer.unload();
	_colors_buffer.unload();

	_vertices.clear();
	_indices.clear();
	_vertices.assign(vertices, vertices + num_verts);
	_indices.assign(indices, indices + num_indexs);
}

void Mesh::loadTexCoords(const glm::vec2* tex_coords, size_t num_tex_coords)
{
	_texCoords_buffer.loadData(tex_coords, num_tex_coords * sizeof(glm::vec2));
}

void Mesh::loadNormals(const glm::vec3* normals, size_t num_normals)
{
	_normals_buffer.loadData(normals, num_normals * sizeof(glm::vec3));
}

void Mesh::loadColors(const glm::u8vec3* colors, size_t num_colors)
{
	_colors_buffer.loadData(colors, num_colors * sizeof(glm::u8vec3));
}

void Mesh::draw() const
{

	if (texture_id)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	if (_texCoords_buffer.id())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_texCoords_buffer.bind();
		glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
	}

	if (_normals_buffer.id())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		_normals_buffer.bind();
		glNormalPointer(GL_FLOAT, 0, nullptr);
	}

	if (_colors_buffer.id())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		_colors_buffer.bind();
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, nullptr);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	_vertices_buffer.bind();
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	_indices_buffer.bind();
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	if (_colors_buffer.id()) glDisableClientState(GL_COLOR_ARRAY);
	if (_normals_buffer.id()) glDisableClientState(GL_NORMAL_ARRAY);
	if (_texCoords_buffer.id()) glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (texture_id)
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::LoadFile(const char* file_path)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array 
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			
			size_t num_vertex = scene->mMeshes[i]->mNumVertices;
			glm::vec3* vertex =  new glm::vec3[num_vertex * 3];
			size_t num_texCoords = num_vertex;
			glm::vec2* texCoords = new glm::vec2[num_vertex * 3];

			for (int k = 0; k < scene->mMeshes[i]->mNumVertices; ++k)
			{
				memcpy(texCoords, new glm::vec2(scene->mMeshes[i]->mTextureCoords[0][i].x, scene->mMeshes[i]->mTextureCoords[0][i].y), sizeof(float) * num_texCoords * 3);
			}
			loadTexCoords(texCoords, num_texCoords);
			
			memcpy(vertex, scene->mMeshes[i]->mVertices, sizeof(float) * num_vertex * 3);

			if (scene->mMeshes[i]->HasFaces())
			{
				size_t num_index = scene->mMeshes[i]->mNumFaces * 3;
				unsigned int* index = new unsigned int[num_index];

				for (int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					memcpy(&index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
					
				}
				load(vertex, num_vertex, index, num_index);
				
			}

			
			
		}

		aiReleaseImport(scene);


	}
	else {
		//LOG(“Error loading scene % s”, file_path);
	}

	
}

void Mesh::CheckerTexture()
{
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
	

}

void Mesh::LoadTexture(const std::string& path)
{
	auto il_img_id = ilGenImage();
	ilBindImage(il_img_id);
	
	ilLoadImage((const wchar_t*)path.c_str());
	auto img_width = ilGetInteger(IL_IMAGE_WIDTH);
	auto img_height = ilGetInteger(IL_IMAGE_HEIGHT);
	auto img_bpp = ilGetInteger(IL_IMAGE_BPP);
	auto img_format = ilGetInteger(IL_IMAGE_FORMAT);
	auto img_data = ilGetData();


	CheckerTexture();
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, img_bpp, img_width, img_height, 0, img_format, GL_UNSIGNED_BYTE, img_data);
	ilDeleteImage(il_img_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
}








