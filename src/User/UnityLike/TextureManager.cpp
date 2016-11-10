# include "TextureManager.hpp"

# include <fstream>
# include <string>
# include <vector>

# include "Utility.hpp"
# include "cinder/ImageIo.h"
# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    TextureManager & TextureManager::Geti( )
    {
        static TextureManager instans;
        return instans;
    }
    TextureManager::TextureManager( )
    {
        std::fstream stream( Utl::getFullPath( "Manager/TextureManager.csv" ), std::ios::in );

        std::string valueString;
        size_t pos;

        if ( stream )
        {
            while ( std::getline( stream, valueString ) )
            {
                std::string key;
                if ( ( pos = valueString.find( "," ) ) != std::string::npos )
                {
                    key = valueString.substr( 0, pos );
                    valueString = valueString.substr( pos + 1 );

                    while ( ( pos = valueString.find( "," ) ) != std::string::npos )
                    {
                        csvData[key].push_back( valueString.substr( 0, pos ) );
                        valueString = valueString.substr( pos + 1 );
                    }

                    if ( valueString != "" )
                    {
                        csvData[key].push_back( valueString );
                    }
                }
            }
        }
        else
        {
            throw( !"Exception : TextureManager ~File is not found~\n" );
        }

        for ( auto& obj : csvData )
        {
            std::string filepath = "Image/" + obj.first + '.' + obj.second[0];
            textureData[obj.first] = loadImage( app::loadAsset( filepath ) );
        }
    }

    const cinder::gl::Texture & TextureManager::find( const std::string & asssetPath )
    {
        return Geti().textureData[asssetPath];
    }
}