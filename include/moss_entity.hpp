#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform() : position(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f) {}

    glm::mat4 getMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

        return translation * rotation_matrix * scaling;
    }
};

class Entity
{
public:
    Transform transform;
};

