#include "Field.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

#include "KarakuriMeshObj.h"
#include "KarakuriRectObj.h"

#include "GlobalData.hpp"
#include "../Utilitys/GlobalDraw.h"

namespace User
{
    using namespace cinder;

    Field::Field( std::string const & path )
        : count( 0 )
        , maxCount( 60 * 4 )
        , isEnd( false )
    {
        JsonTree params( app::loadAsset( path ) );

        ground = &GData::FindTexture( "ground.jpg" );
        skydome = std::make_shared<Skydome>( "skydome_1.bmp", 90.0F );

        int i = 0;
        for ( auto& obj : params["MeshObject"] )
        {
            karakuriMeshObjects.emplace_back( std::make_shared<KarakuriMeshObj>( obj ) );
            //editor->addParam( "[Position]", &karakuris.back( )->Position( ) ).group( std::to_string( i ) );
        }
        for ( auto& obj : params["RectObject"] )
        {
            karakuriRectObjects.emplace_back( std::make_shared<KarakuriRectObj>( obj ) );
        }
    }

    void Field::update( )
    {
        Each( [ this ] ( KarakuriRef& karakuri ) { karakuri->Update( count ); } );

        if ( isEnd == false )
        {
            count = std::min( count + 1, maxCount );
        }
        else
        {
            count = std::max( count - 1, 0 );
        }
    }

    void Field::draw( cinder::CameraPersp const& camera )
    {
        skydome->Draw( camera );

        gl::pushModelView( );
        gl::translate( Vec3f( -ground->getWidth( ) / 2, 0, -ground->getHeight( ) / 2 ) );
        gl::rotate( Vec3f( 90, 0, 0 ) );
        gl::draw( *ground );
        gl::popModelView( );

        for ( auto itr = karakuriMeshObjects.begin( ); itr != karakuriMeshObjects.end( ); ++itr )
        {
            GlobalDraw::Insert( std::bind( &KarakuriActor::Draw, *itr ) );
        }
        for ( auto itr = karakuriRectObjects.begin( ); itr != karakuriRectObjects.end( ); ++itr )
        {
            GlobalDraw::InsertAlphaObject( std::make_pair( camera.worldToEyeDepth( ( *itr )->Position( ) ), std::bind( &KarakuriActor::Draw, *itr ) ) );
        }
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

    bool Field::IsMoveing( )
    {
        return count != maxCount;
    }

    void Field::Each( std::function<void( KarakuriRef& karakuri )> func )
    {
        std::for_each( karakuriMeshObjects.begin( ), karakuriMeshObjects.end( ), func );
        std::for_each( karakuriRectObjects.begin( ), karakuriRectObjects.end( ), func );
    }
}