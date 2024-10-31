#include <iostream>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<EntityType, EntityVec> EntityMap;

class EntityManager
{
    EntityVec _entities;
    EntityMap _entityMap;
    std::vector<std::shared_ptr<Entity>> _toAdd;
    size_t _totalEntities = 0;

    void init();

   public:
    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const EntityType& type);
    EntityVec& getEntities();
    EntityVec& getEntities(const EntityType& type);
    void removeDeadEntities(EntityVec& vec);
};