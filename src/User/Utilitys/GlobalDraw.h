# pragma once

# include <map>
# include <functional>
# include <vector>

namespace User
{
    class GlobalDraw
    {
        GlobalDraw( ) { /*nothing*/ }
    private:
        static std::vector<std::function<void( )>> drawArray;
        static std::multimap<float, std::function<void( )>> drawMapAlphaObject;
    public:
        static void Insert( std::function<void( )> const& function );
        static void InsertAlphaObject( std::pair<float, std::function<void( )>> const& pair );
        static void Draw( );
    };
}