#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Component.h"

class TransformComponent : public Component
{
public:
    TransformComponent(std::weak_ptr<GameObject> owner);
    ~TransformComponent() override = default;

    const auto& GetMatrix() const { return matrix; }
    const auto& GetLeft() const { return left; }
    const auto& GetUp() const { return up; }
    const auto& GetForward() const { return forward; }
    const auto& GetPosition() const { return position; }
    const auto& GetRotation() const { return rotation; }
    const auto& GetScale() const { return scale; }
    auto& GetPosition() { return position; }
    auto& GetRotation() { return rotation; }
    auto& GetScale() { return scale; }

    const auto* GetData() const { return &matrix[0][0]; }

    void Translate(const glm::dvec3& translation);
    void Rotate(double rads, const glm::dvec3& axis);
    void Scale(const glm::dvec3& scale);
    void LookAt(const glm::dvec3& target);

    
private:

    union
    {
        glm::dmat4 matrix = glm::dmat4(1.0);
        struct
        {
            glm::dvec3 left; glm::dmat4::value_type left_w;
            glm::dvec3 up; glm::dmat4::value_type up_w;
            glm::dvec3 forward; glm::dmat4::value_type fwd_w;
            glm::dvec3 position; glm::dmat4::value_type pos_w;
            glm::dvec3 rotation; glm::dmat4::value_type rot_w;
            glm::dvec3 scale; glm::dmat4::value_type scale_w;
        };
    };
};

