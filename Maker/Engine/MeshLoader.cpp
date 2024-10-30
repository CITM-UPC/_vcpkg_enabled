#include "MeshLoader.h"
#include "GameObject.h" 
#include "Transform.h"
#include "Mesh.h" 
#include "Texture.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"

MeshLoader::MeshLoader(std::weak_ptr<GameObject> owner) : Component(owner) {}

void MeshLoader::SetMesh(std::shared_ptr<Mesh> mesh)
{
    this->mesh = mesh;
}

std::shared_ptr<Mesh> MeshLoader::GetMesh() const
{
    return mesh;
}

void MeshLoader::SetColor(const glm::vec3& color)
{
    this->color = color;
}

glm::vec3 MeshLoader::GetColor() const
{
    return color;
}

void MeshLoader::SetTexture(std::shared_ptr<Texture> texture)
{
    this->texture = texture;
}

std::shared_ptr<Texture> MeshLoader::GetTexture() const
{
    return texture;
}

void MeshLoader::SetImage(std::shared_ptr<Image> image)
{
	this->image = image;
}

std::shared_ptr<Image> MeshLoader::GetImage() const
{
    return image;
}

void MeshLoader::Render() const
{
    if (texture && drawTexture)
    {
        glEnable(GL_TEXTURE_2D);
        texture->bind();
	}
	else if (!texture || !drawTexture)
	{
		mesh->CheckerTexture();
	}

    if (mesh) mesh->draw();
    if (drawNormals) mesh->drawNormals(0.1f);

    if (texture)
    {
        glDisable(GL_TEXTURE_2D);
    }
}
