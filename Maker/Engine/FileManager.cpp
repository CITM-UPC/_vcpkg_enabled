#include "FileManager.h"

void FileManager::LoadFile(const char* path, GameObject& go)
{
	// Load file
	std::string extension = getFileExtension(path);

	if (extension == "obj" || extension == "fbx" || extension == "dae") {
		// Load Mesh
		auto mesh = std::make_shared<Mesh>();
		mesh->LoadFile(path);
		go.AddComponent<MeshLoader>()->SetMesh(mesh);
		go.setMesh(mesh);
	}
	else if (extension == "png" || extension == "jpg" || extension == "bmp") {
		// Load Texture
		auto imageTexture = std::make_shared<Image>();
		auto texture = std::make_shared<Texture>();
		imageTexture->LoadTexture(path);
		texture->setImage(imageTexture);
		go.GetComponent<MeshLoader>()->GetMesh()->deleteCheckerTexture();
		go.GetComponent<MeshLoader>()->SetImage(imageTexture);
		go.GetComponent<MeshLoader>()->SetTexture(texture);
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


