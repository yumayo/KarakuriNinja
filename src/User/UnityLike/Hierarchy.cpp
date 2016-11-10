#include "Hierarchy.hpp"
#include <algorithm>

namespace User
{
    std::map<std::string, GameObjectPtr> Hierarchy::objects;
    std::vector<std::map<std::string, GameObjectPtr>::iterator> Hierarchy::destroyList;

    void Hierarchy::clear( )
    {
        objects.clear( );
        destroyList.clear( );
    }

    GameObject & Hierarchy::regist( const std::string& objectName, GameObject & gameObject )
    {
        return *( objects[objectName] = &gameObject );
    }
    void Hierarchy::destroy( GameObjectPtr object )
    {
        for ( auto itr = objects.begin( ); itr != objects.end( ); ++itr )
        {
            if ( itr->second == object )
            {
                destroyList.push_back( itr );
                return;
            }
        }
    }
    GameObject & Hierarchy::find( const std::string & objectName )
    {
        auto itr = objects.find(objectName);
        if ( itr != objects.end( ) )
        {
            return *itr->second;
        }

        static GameObjectPtr null = nullptr;
        return *null;
    }
    void Hierarchy::update( )
    {
        for ( auto& obj : destroyList )
        {
            objects.erase( obj );
        }
        destroyList.clear( );
    }
}
