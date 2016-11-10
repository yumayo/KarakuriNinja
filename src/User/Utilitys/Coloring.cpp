
#include "Coloring.h"

namespace Utl
{
    using namespace cinder;

    ColorA Coloring::html( const std::string& str )
    {
        ColorA col = ColorA::white( );

        if ( str[0] != '#' ) return col;

        if ( str.size( ) == 7 )
        {
            col.r = std::stof( "0X" + str.substr( 1, 2 ) ) / 255.0f;
            col.g = std::stof( "0X" + str.substr( 3, 2 ) ) / 255.0f;
            col.b = std::stof( "0X" + str.substr( 5, 2 ) ) / 255.0f;
        }
        else if ( str.size( ) == 9 )
        {
            col.r = std::stof( "0X" + str.substr( 1, 2 ) ) / 255.0f;
            col.g = std::stof( "0X" + str.substr( 3, 2 ) ) / 255.0f;
            col.b = std::stof( "0X" + str.substr( 5, 2 ) ) / 255.0f;
            col.a = std::stof( "0X" + str.substr( 7, 2 ) ) / 255.0f;
        }

        return col;
    }
}