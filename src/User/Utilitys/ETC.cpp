
#include "ETC.h"

#include <sstream>

#include "cinder/app/App.h"

namespace Utl
{
    using namespace cinder;

    std::vector<std::string> split( const std::string &str, char sep )
    {
        std::vector<std::string> v;
        std::stringstream ss( str );
        std::string buffer;
        while ( std::getline( ss, buffer, sep ) ) v.push_back( buffer );
        return v;
    }

    std::string getFullPath( const std::string & assetPath )
    {
        return app::loadAsset( assetPath )->getFilePath( ).string( );
    }
}