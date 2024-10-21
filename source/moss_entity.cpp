#include "moss_entity.hpp"

#include <vector>
#include <memory>

std::vector<std::unique_ptr<Entity>> entities;

Entity* entity::createEntity()
{
    entities.push_back(std::make_unique<Entity>());
    return entities.back().get();
}

void entity::destroyEntity(Entity* entity)
{
    for (auto it = entities.begin(); it != entities.end(); it++)
    {
        if (it->get() == entity)
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