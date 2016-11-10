# pragma once


# include "Moji.h"
# include <list>

namespace User
{

    class MojiManager
    {
        Fonts font;
        int frame = 0;
        std::list<Moji> mojiList;

        bool isEnd = false;
        int clearCount = 60;
    public:
        MojiManager( );
        void ReCall( std::string const& jsonPath );
        void Setup( std::string const& jsonPath );
        void End( );
        bool IsEnded( );
    public:
        void Update( );
        void Draw( cinder::Vec2f position, float mojiWidth );
    };
}