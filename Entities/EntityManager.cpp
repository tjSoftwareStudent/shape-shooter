#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    // Delete entities that are marked inactive in _entities
    removeDeadEntities(_entities);
    // Delete entities that are marked inactive from _entityMap
    for (auto& [tag, entityVec] : _entityMap)
    {
        removeDeadEntities(entityVec);
    }

    // Add entities that were queued up
    for (auto& e : _toAdd)
    {
        _entities.push_back(e);
        _entityMap[e->type()].push_back(
            e);  // If no key of tag exists it will create new key
    }

    _toAdd.clear();  // Need to clear since entities have all been added
}

std::shared_ptr<Entity> EntityManager::addEntity(const EntityType& type)
{
    // manually construct the std::shared_ptr by calling the private constructor
    // directly from within EntityManager. This works because EntityManager is a
    // friend of Entity, and thus it can call the private constructor directly
    auto e = std::shared_ptr<Entity>(new Entity(_totalEntities++, type));
    _toAdd.push_back(e);

    return e;
}

EntityVec& EntityManager::getEntities()
{
    return _entities;
}

EntityVec& EntityManager::getEntities(const EntityType& type)
{
    return _entityMap[type];
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    vec.erase(std::remove_if(vec.begin(),
                             vec.end(),
                             [](const std::shared_ptr<Entity> e)
                             { return !e->isActive(); }),
              vec.end());
}