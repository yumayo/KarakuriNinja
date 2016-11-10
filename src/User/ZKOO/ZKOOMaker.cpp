# include "ZKOOMaker.h"

# include "Utility.hpp"

# include "Framework.hpp"

namespace User
{
    ZKOOMaker::ZKOOMaker( )
        : recv( 52525 )
    {

    }

    ZKOOMaker & ZKOOMaker::Geti( )
    {
        static ZKOOMaker instans;
        return instans;
    }

    void ZKOOMaker::update( )
    {
        std::string str;
        while ( true )
        {
            if ( recv.recv( str ) ) break;
        }

        hands = handMake( Utl::split( str, '/' ) );
    }

    bool ZKOOMaker::IsHandUsing( )
    {
        return hands.empty( ) == false;
    }

    ZKOOHand* ZKOOMaker::Left( )
    {
        if ( !IsHandUsing( ) ) return nullptr;
        return &hands[1];
    }

    ZKOOHand* ZKOOMaker::Right( )
    {
        if ( !IsHandUsing( ) ) return nullptr;
        return &hands[0];
    }

    std::vector<ZKOOHand> ZKOOMaker::handMake( std::vector<std::string>& splited )
    {
        std::vector<ZKOOHand> hands;
        if ( splited.size( ) != ( 10 ) ) return hands;
        hands = std::vector<ZKOOHand>( 2 );

        int index = 0;

        for ( int i = 0; i < 2; ++i )
        {
            auto posx = std::stod( splited[index++] );
            auto posy = std::stod( splited[index++] );
            hands[i].Position( cinder::Vec2d( posx, posy ) + env.getWindowPos( ) );
            hands[i].Rotation( std::stod( splited[index++] ) );
            hands[i].IsTouching( splited[index++] == "True" ? true : false );
            hands[i].IsTracking( splited[index++] == "True" ? true : false );

            if ( i == 0 )
            {
                CONSOLE << "œœœœœœœœœœœœœœ" << std::endl;
                CONSOLE << hands[i].Position( ) << std::endl;
                CONSOLE << hands[i].Rotation( ) << std::endl;
                CONSOLE << hands[i].IsTouching( ) << std::endl;
                CONSOLE << hands[i].IsTracking( ) << std::endl;
            }
            else
            {
                CONSOLE << "››››››››››››››" << std::endl;
                CONSOLE << hands[i].Position( ) << std::endl;
                CONSOLE << hands[i].Rotation( ) << std::endl;
                CONSOLE << hands[i].IsTouching( ) << std::endl;
                CONSOLE << hands[i].IsTracking( ) << std::endl;
            }
        }

        return hands;
    }
}