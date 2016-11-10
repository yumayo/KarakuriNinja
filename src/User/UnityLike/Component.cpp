#include "Component.hpp"

# include "Hierarchy.hpp"

namespace User
{
    GameObjectPtr Component::GetGameObjectPtr( )
    {
        return gameObjectPtr;
    }
    GameObject & Component::GetGameObject( )
    {
        return *gameObjectPtr;
    }
    void Component::destroy( GameObjectPtr gameObjectPtr )
    {
        Hierarchy::destroy( gameObjectPtr );
    }
}