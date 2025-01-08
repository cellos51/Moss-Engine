#pragma once

#include "moss_entity.hpp"

class Camera: public Entity
{
public:
    virtual const char* getClass() const override { return "Camera"; }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), -transform.getPosition());
        glm::mat4 rotation_matrix = glm::mat4_cast(glm::inverse(transform.getRotation()));

        return rotation_matrix * translation;
    }

    float fov = 45.0f;
    float near_clip = 0.1f;
    float far_clip = 100.0f;
};