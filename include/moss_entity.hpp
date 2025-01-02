#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
    Transform() : position(0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), scale(1.0f), parent(nullptr) {}

    glm::vec3 getLocalEulerAngles() const { return glm::eulerAngles(rotation); }
    void setLocalEulerAngles(const glm::vec3& value) { rotation = glm::quat(value); }
    glm::vec3 getLocalPosition() const { return position; }
    void setLocalPosition(const glm::vec3& value) { position = value; }
    glm::quat getLocalRotation() const { return rotation; }
    void setLocalRotation(const glm::quat& value) { rotation = value; }
    glm::vec3 getLocalScale() const { return scale; }
    void setLocalScale(const glm::vec3& value) { scale = value; }

    glm::vec3 getEulerAngles() const { if (parent) { return glm::eulerAngles(rotation * parent->getRotation()); } else { return glm::eulerAngles(rotation); } }
    void setEulerAngles(const glm::vec3& value) { if (parent) { rotation = glm::quat(value) * glm::inverse(parent->getRotation()); } else { rotation = glm::quat(value); } }
    glm::vec3 getPosition() const { if (parent) { return position + parent->getPosition(); } else { return position; } }
    void setPosition(const glm::vec3& value) { position = value; }
    glm::quat getRotation() const { if (parent) { return rotation * parent->getRotation(); } else { return rotation; } }
    void setRotation(const glm::quat& value) { rotation = value; }

    Transform* getParent() const { return parent; }
    void setParent(Transform* value) { parent = value; }

    glm::mat4 getMatrix() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

        if (parent)
        {
            return parent->getMatrix() * translation * rotation_matrix * scaling;
        }
        else
        {
            return translation * rotation_matrix * scaling;
        }
    }
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform* parent;
};

class Entity
{
public:
    Transform transform;
    virtual const char* getClass() const { return "Entity"; }
};

namespace entity
{
    Entity* createEntity();
    void destroyEntity(Entity* entity);
    std::vector<Entity*> getEntities();
    void clearEntities();
}