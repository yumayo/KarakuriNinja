#include "KarakuriMeshObj.h"

#include "cinder/gl/gl.h"

namespace User
{
    using namespace cinder;

    KarakuriMeshObj::KarakuriMeshObj( cinder::JsonTree const& params )
        : KarakuriActor( params )
        , objRender( params.getValueForKey<std::string>( "texture" ), params.getValueForKey<std::string>( "obj" ) )
    {
        objRender.Position( Position( ) );
        objRender.Scale( Scale( ) );
        objRender.Rotate( Rotate( ) );
    }

    void KarakuriMeshObj::Update( int count )
    {
        KarakuriActor::Update( count );
        objRender.Position( Position( ) );
    }

    void KarakuriMeshObj::Draw( )
    {
        objRender.Draw( );
    }
}
