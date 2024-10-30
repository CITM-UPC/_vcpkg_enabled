#include "TransformComponent.h"
#include "GameObject.h"

#include <glm/gtc/matrix_transform.hpp>

TransformComponent::TransformComponent(std::weak_ptr<GameObject> owner) : Component(owner) {}

void TransformComponent::Translate(const glm::dvec3& translation)
{
	matrix = glm::translate(matrix, translation);
}

void TransformComponent::Rotate(double rads, const glm::dvec3& axis)
{
	matrix = glm::rotate(matrix, rads, axis);
}

void TransformComponent::Scale(const glm::dvec3& scale)
{
	matrix = glm::scale(matrix, scale);
}

void TransformComponent::LookAt(const glm::dvec3& target)
{
	matrix = glm::lookAt(position, target, up);
}