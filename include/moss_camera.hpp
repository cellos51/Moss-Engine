#pragma once

#include "moss_entity.hpp"

class Camera: public Entity
{
public:
    // Camera derives from Entity, so we don't need to redefine the transform
    float fov = 45.0f;
    float near_clip = 0.1f;
    float far_clip = 100.0f;

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), -transform.getPosition());
        glm::mat4 rotation_matrix = glm::mat4_cast(glm::inverse(transform.getRotation()));

        return rotation_matrix * translation;
    }

    virtual const char* getClass() const override { return "Camera"; }
};