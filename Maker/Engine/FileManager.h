#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "GameObject.h"

class FileManager
{

public:

	void LoadFile(const char* path, GameObject& go);
	std::string getFileExtension(const std::string& filePath);
	bool isMouseOverGameObject(const GameObject& go, int mouseX, int mouseY);
	bool rayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const BoundingBox& bbox);
	glm::vec3 screenToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};

