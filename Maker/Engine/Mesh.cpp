#include "Mesh.h"

#include <GL/glew.h>
using namespace std;

#define CHECKERS_HEIGHT 32
#define CHECKERS_WIDTH 32

void Mesh::load(const glm::vec3* vertices, size_t num_verts, unsigned int* indices, size_t num_indexs)
{
	_vertices.assign(vertices, vertices + num_verts);
	_indices.assign(indices, indices + num_indexs);
	_vertices_buffer.loadData(vertices, num_verts * sizeof(glm::vec3));
	_indices_buffer.loadIndices(indices, num_indexs);
	_texCoords_buffer.unload();
	_normals_buffer.unload();
	_colors_buffer.unload();

	_boundingBox.min = _vertices.front();
	_boundingBox.max = _vertices.front();

	for (const auto& v : _vertices) {
		_boundingBox.min = glm::min(_boundingBox.min, glm::dvec3(v));
		_boundingBox.max = glm::max(_boundingBox.max, glm::dvec3(v));
	}
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

	if (scene != nullptr && scene->HasMeshes()) {
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			size_t num_vertices = scene->mMeshes[i]->mNumVertices;
			glm::vec3* vertex = new glm::vec3[num_vertices * 3];
			memcpy(vertex, scene->mMeshes[i]->mVertices, sizeof(float) * num_vertices * 3);


			if (scene->mMeshes[i]->HasFaces()) {
				size_t num_index = scene->mMeshes[i]->mNumFaces * 3;
				unsigned int* index = new unsigned int[num_index]; // assume each face is a triangle
				for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
					memcpy(&index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(unsigned int));
				}
				load(vertex, num_vertices, index, num_index);
			}

			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				glm::vec2* texCoords = new glm::vec2[num_vertices];
				for (size_t j = 0; j < num_vertices; ++j) {
					texCoords[j] = glm::vec2(
						scene->mMeshes[i]->mTextureCoords[0][j].x,
						-scene->mMeshes[i]->mTextureCoords[0][j].y
					);
				}
				loadTexCoords(texCoords, num_vertices);
				delete[] texCoords;
			}


		}
		aiReleaseImport(scene);
	}
	else {
		// Handle error
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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	

}

void Mesh::LoadTexture(const std::string& path)
{
	auto img = ilGenImage();
	ilBindImage(img);
	ilLoadImage((const wchar_t*)path.c_str());
	auto width = ilGetInteger(IL_IMAGE_WIDTH);

	auto height = ilGetInteger(IL_IMAGE_HEIGHT);

	auto channels = ilGetInteger(IL_IMAGE_CHANNELS);
	auto data = ilGetData();

	//load image as a texture in VRAM
	texture_id = _texCoords_buffer.id();
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	//now we can delete image from RAM
	ilDeleteImage(img);
}








