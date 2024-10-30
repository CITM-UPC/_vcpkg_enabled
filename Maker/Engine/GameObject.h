#pragma once

#include <memory>
#include <string> 
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "TreeExt.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "Texture.h"
#include "BoundingBox.h"
#include "Mesh.h"

class GameObject : public std::enable_shared_from_this<GameObject>, public TreeExt<GameObject>
{

public:
	std::shared_ptr<TransformComponent> _transform;
	std::shared_ptr<Mesh> _mesh_ptr;
	glm::u8vec3 _color = glm::u8vec3(255, 255, 255);
	Texture _texture;
	
	std::string name;
	bool drawTexture = true;
	

public:
	GameObject(const std::string& name = "GameObject");
	~GameObject();

	template <IsComponent T, typename... Args>
	std::shared_ptr<T> AddComponent(Args&&... args);

	template <IsComponent T>
	std::shared_ptr<T> GetComponent() const;

	template <IsComponent T>
	void RemoveComponent();

	template <IsComponent T>
	bool HasComponent() const;

	std::string GetName() const;
	void SetName(const std::string& name);

	bool CompareTag(const std::string& tag) const;

	std::shared_ptr<TransformComponent> GetTransform() const { return _transform; }

	/*const auto& transform() const { return _transform; }
	auto& transform() { return _transform; }*/
	const auto& color() const { return _color; }
	auto& color() { return _color; }
	const auto& texture() const { return _texture; }
	auto& texture() { return _texture; }
	const auto& mesh() const { return *_mesh_ptr; }
	auto& mesh() { return *_mesh_ptr; }

	BoundingBox boundingBox() const;
	BoundingBox localBoundingBox() const { return _mesh_ptr ? _mesh_ptr->boundingBox() : BoundingBox(); }

	void drawWiredQuad(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3);
	void drawBoundingBox(const BoundingBox& box);

	void setTextureImage(const std::shared_ptr<Image>& img_ptr) { _texture.setImage(img_ptr); }
	void setMesh(const std::shared_ptr<Mesh>& mesh_ptr) { _mesh_ptr = mesh_ptr; }

	bool hasTexture() const { return _texture.id(); }
	bool hasMesh() const { return _mesh_ptr != nullptr; }

	// Basic Shapes
	static std::shared_ptr<GameObject> createCube();
	static std::shared_ptr<GameObject> createSphere();
	static std::shared_ptr<GameObject> createCylinder();

	void draw() const;
	void drawAxis(double size);
	void drawDebug(const GameObject& obj);

private:
	
	
	std::string tag = "Untagged";
	bool active = true;
	bool destroyed = false;

	std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

	mutable std::type_index cachedComponentType;
	mutable std::shared_ptr<Component> cachedComponent;
};

template <IsComponent T, typename... Args>
std::shared_ptr<T> GameObject::AddComponent(Args&&... args) {
	std::shared_ptr<T> newComponent = std::make_shared<T>(weak_from_this(), std::forward<Args>(args)...);
	components[typeid(T)] = newComponent;
	return newComponent;
}

template <IsComponent T>
std::shared_ptr<T> GameObject::GetComponent() const {
	if (cachedComponentType == typeid(T) && cachedComponent) {
		return std::static_pointer_cast<T>(cachedComponent);
	}

	auto it = components.find(typeid(T));
	if (it != components.end()) {
		cachedComponentType = typeid(T);
		cachedComponent = it->second;
		return std::static_pointer_cast<T>(cachedComponent);
	}
	else {
		throw std::runtime_error("Component not found on GameObject: " + this->GetName());
	}
}

template <IsComponent T>
void GameObject::RemoveComponent() {
	auto it = components.find(typeid(T));
	if (it != components.end()) {
		components.erase(it);
	}
	else {
		//Log a warning 
	}
}

template <IsComponent T>
bool GameObject::HasComponent() const {
	return components.find(typeid(T)) != components.end();
}
