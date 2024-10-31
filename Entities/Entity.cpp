#include "Entity.h"

Entity::Entity(const size_t id, const EntityType& type) : _id(id), _type(type)
{
}

bool Entity::isActive() const
{
    return _active;
}

const EntityType& Entity::type() const
{
    return _type;
}
const size_t Entity::id() const
{
    return _id;
}
void Entity::destroy()
{
    _active = false;
}