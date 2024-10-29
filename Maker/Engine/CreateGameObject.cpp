#include "GameObject.h"
#include "Mesh.h"
#include "types.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::shared_ptr<GameObject> GameObject::createCube() 
{
	
	auto mesh = std::make_shared<Mesh>();

    std::vector<glm::vec3> vertices = {
        // Front face
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        // Back face
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Top face
        3, 2, 6, 6, 7, 3,
        // Back face
        7, 6, 5, 5, 4, 7,
        // Bottom face
        4, 5, 1, 1, 0, 4,
        // Left face
        4, 0, 3, 3, 7, 4,
        // Right face
        1, 5, 6, 6, 2, 1,
    };

    std::vector<glm::vec2> texCoords = {
        // Front face
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
        // Back face
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };

    mesh->load(vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh->loadTexCoords(texCoords.data(), texCoords.size());

    auto gameObject = std::make_shared<GameObject>();
    gameObject->setMesh(mesh);

    return gameObject;
}

std::shared_ptr<GameObject> GameObject::createSphere() {
    auto mesh = std::make_shared<Mesh>();

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec2> texCoords;

    const int latitudeSegments = 18;
    const int longitudeSegments = 36;
    const float radius = 1.0f;

    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = lat * M_PI / latitudeSegments;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = lon * 2.0f * M_PI / longitudeSegments;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;
            float u = 1.0f - (float(lon) / longitudeSegments);
            float v = 1.0f - (float(lat) / latitudeSegments);

            vertices.push_back({ radius * x, radius * y, radius * z });
            texCoords.push_back({ u, v });
        }
    }

    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            int first = (lat * (longitudeSegments + 1)) + lon;
            int second = first + longitudeSegments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    mesh->load(vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh->loadTexCoords(texCoords.data(), texCoords.size());

    auto gameObject = std::make_shared<GameObject>();
    gameObject->setMesh(mesh);

    return gameObject;
}

std::shared_ptr<GameObject> GameObject::createCylinder()
{
    auto mesh = std::make_shared<Mesh>();

    std::vector<glm::vec3> vertices;
    const int segments = 36;
    const float radius = 1.0f;
    const float height = 2.0f;

    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);
        vertices.push_back({ x, -height / 2, z }); // Bottom circle
        vertices.push_back({ x,  height / 2, z }); // Top circle
    }
    vertices.push_back({ 0.0f, -height / 2, 0.0f }); // Center of bottom circle
    vertices.push_back({ 0.0f,  height / 2, 0.0f }); // Center of top circle

    std::vector<unsigned int> indices;
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        // Side faces
        indices.push_back(i * 2);
        indices.push_back(next * 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(next * 2);
        indices.push_back(next * 2 + 1);
        indices.push_back(i * 2 + 1);
    }
    // Bottom face
    int bottomCenterIndex = segments * 2;
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        indices.push_back(bottomCenterIndex);
        indices.push_back(i * 2);
        indices.push_back(next * 2);
    }
    // Top face
    int topCenterIndex = segments * 2 + 1;
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        indices.push_back(topCenterIndex);
        indices.push_back(next * 2 + 1);
        indices.push_back(i * 2 + 1);
    }

    std::vector<glm::vec2> texCoords;
    for (int i = 0; i < segments; ++i) {
        float u = float(i) / float(segments);
        texCoords.push_back({ u, 0.0f }); // Bottom circle
        texCoords.push_back({ u, 1.0f }); // Top circle
    }
    texCoords.push_back({ 0.5f, 0.0f }); // Center of bottom circle
    texCoords.push_back({ 0.5f, 1.0f }); // Center of top circle

    mesh->load(vertices.data(), vertices.size(), indices.data(), indices.size());
    mesh->loadTexCoords(texCoords.data(), texCoords.size());

    auto gameObject = std::make_shared<GameObject>();
    gameObject->setMesh(mesh);

    return gameObject;
}