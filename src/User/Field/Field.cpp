#include "Field.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

namespace User
{
    using namespace cinder;

    Field::Field( std::string const & path )
        : count( 0 )
        , isEnd( false )
    {
        JsonTree params( app::loadAsset( path ) );

        for ( auto& obj : params["karakuri"] )
        {
            karakuris.emplace_back( std::make_shared<KarakuriBase>( obj ) );
        }
    }

    void Field::update( )
    {
        Each( [ this ] ( KarakuriRef& karakuri ) { karakuri->update( count ); } );

        if ( isEnd == false )
        {
            count = std::min( count + 1, 60 * 4 );
        }
        else
        {
            count = std::max( count - 1, 0 );
        }
    }

    void Field::draw( )
    {
        Each( [ ] ( KarakuriRef& karakuri ) { karakuri->draw( ); } );
    }

    void Field::End( )
    {
        if ( isEnd ) return;
        isEnd = true;
    }

    bool Field::IsNextField( )
    {
        return count == 0 && isEnd;
    }

    void Field::Each( std::function<void( KarakuriRef& karakuri )> func )
    {
        std::for_each( karakuris.begin( ), karakuris.end( ), func );
    }
}