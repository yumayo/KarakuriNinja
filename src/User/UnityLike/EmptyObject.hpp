# pragma once

# include "GameObject.hpp"
# include "Hierarchy.hpp"

# include <string>

namespace User
{
    class EmptyObject
    {
    public:
        EmptyObject( const std::string& gameObjectName );
        virtual ~EmptyObject( ) { }
        virtual void update( );
        virtual void draw( );
        virtual void shadowDraw( );
    protected:
        std::string objectName;
        GameObject object;
    };
}