#include <memory>
#include <string>

#include "../Components/Components.h"
#include "EntityType.h"

class Entity
{
    friend class EntityManager;

    bool _active = true;
    size_t _id = 0;
    EntityType _type;

    // constructor and destructor
    Entity(const size_t id, const EntityType& type);

   public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;

    // private member access functions
    bool isActive() const;
    const EntityType& type() const;
    const size_t id() const;
    void destroy();
};