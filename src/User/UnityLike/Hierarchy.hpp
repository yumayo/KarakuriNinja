# pragma once

# include <map>
# include <vector>
# include "GameObject.hpp"

namespace User
{
    class Hierarchy
    {
    public:
        static void clear( );
    public:
        static GameObject& regist( const std::string& objectName, GameObject& gameObject );
        static void destroy( GameObjectPtr object );
        static GameObject& find( const std::string& objectName );
    public:
        static void update( );
    public:
        static std::map<std::string, GameObjectPtr> objects;
        static std::vector<std::map<std::string, GameObjectPtr>::iterator> destroyList;
    };
}
