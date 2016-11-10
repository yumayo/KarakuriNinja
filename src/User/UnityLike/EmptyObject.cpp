#include "EmptyObject.hpp"

namespace User
{
    EmptyObject::EmptyObject( const std::string & gameObjectName )
        : objectName( gameObjectName )
    {
        Hierarchy::regist( gameObjectName, object );
    }
    void EmptyObject::update( )
    {
        auto& obj = Hierarchy::find( objectName );
        if( &obj != nullptr ) obj.update( );
    }
    void EmptyObject::draw( )
    {
        auto& obj = Hierarchy::find( objectName );
        if ( &obj != nullptr ) obj.draw( );
    }
    void EmptyObject::shadowDraw( )
    {
        auto& obj = Hierarchy::find( objectName );
        if ( &obj != nullptr ) obj.shadowDraw( );
    }
}