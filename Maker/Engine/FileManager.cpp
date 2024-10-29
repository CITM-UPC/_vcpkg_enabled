#include "FileManager.h"

void FileManager::LoadFile(const char* path, GameObject& go)
{
	// Load file
	std::string extension = getFileExtension(path);

	if (extension == "obj" || extension == "fbx" || extension == "dae") {
		// Load Mesh
		auto mesh = std::make_shared<Mesh>();
		mesh->LoadFile(path);
		go.setMesh(mesh);
	}
	else if (extension == "png" || extension == "jpg" || extension == "bmp") {
		// Load Texture
		auto imageTexture = std::make_shared<Image>();
		imageTexture->LoadTexture(path);
		go.setTextureImage(imageTexture);
	}
	else {
		std::cerr << "Unsupported file extension: " << extension << std::endl;
	}
}

std::string FileManager::getFileExtension(const std::string& filePath)
{
	// Find the last dot in the file path
	size_t dotPosition = filePath.rfind('.');

	// If no dot is found, return an empty string
	if (dotPosition == std::string::npos) {
		return "";
	}

	// Extract and return the file extension
	return filePath.substr(dotPosition + 1);
}


