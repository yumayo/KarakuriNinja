# pragma once

# include "Object.hpp"
# include "Actor.hpp"

# include "Types.hpp"

# include <memory>
# include <vector>

# define NAME virtual std::string name( ) override { return typeid( *this ).name( ); }

namespace User
{
    class Component : public Object, public Actor
    {
    public:
        Component( ) : gameObjectPtr( nullptr ) { }
        virtual ~Component( ) { }
    public:
        GameObjectPtr GetGameObjectPtr( );
        GameObject& GetGameObject( );
    public:
        static void destroy( GameObjectPtr gameObjectPtr );
    private:
        friend class GameObject;
        GameObjectPtr gameObjectPtr;
    public: NAME
    };
}