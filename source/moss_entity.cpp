#include "moss_entity.hpp"
#include "moss_camera.hpp" // this is a bandaid solution but it works for now

#include <vector>
#include <memory>

static std::vector<std::unique_ptr<Entity>> entities;

// Entity functions
Entity::Entity() : transform()
{
    entities.push_back(std::unique_ptr<Entity>(this));
}

void Entity::destroy() // This doesn't work
{
    for (auto it = entities.begin(); it != entities.end(); it++)
    {
        if (it->get() == this)
        {
            entities.erase(it);
            return;
        }
    }
}

std::vector<Entity*> entity::getEntities()
{
    std::vector<Entity*> result;
    for (auto& entity : entities)
    {
        result.push_back(entity.get());
    }
    return result;
}

void entity::clearEntities()
{
    entities.clear();
}