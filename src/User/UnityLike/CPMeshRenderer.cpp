#include "CPMeshRenderer.hpp"

namespace User
{
    using namespace cinder;

    CPMeshRenderer::CPMeshRenderer( )
        : gl::Material( Color::white( ) / 2.0f,
                        Color::white( ),
                        Color::white( ),
                        80.0f,
                        Color::black( ) )
        , element( )
        , isCreateElement( false )
        , elementNormalMap( )
        , isCreateElementNormalMap( false )
    { }
    CPMeshRenderer::CPMeshRenderer( const cinder::Color & color )
        : gl::Material( color / 2.0f,
                        color,
                        color,
                        80.0f,
                        Color::black( ) )
        , element( )
        , isCreateElement( false )
        , elementNormalMap( )
        , isCreateElementNormalMap( false )
    { }
    CPMeshRenderer::CPMeshRenderer( const cinder::Color & color, const std::string & texture )
        : gl::Material( color / 2.0f,
                        color,
                        color,
                        80.0f,
                        Color::black( ) )
        //, element( TextureManager::find( texture ) )
        //, isCreateElement( true )
        , elementNormalMap( )
        , isCreateElementNormalMap( false )
    {
    }
    CPMeshRenderer::CPMeshRenderer( const cinder::Color & color, const std::string & texture, const std::string & normalMap )
        : gl::Material( color / 2.0f,
                        color,
                        color,
                        80.0f,
                        Color::black( ) )
        //, element( TextureManager::find( texture ) )
        //, isCreateElement( true )
        //, elementNormalMap( TextureManager::find( normalMap ) )
        //, isCreateElementNormalMap( true )
    {
    }
    void CPMeshRenderer::SetMaterial( const cinder::Color& color )
    {
        setAmbient( color / 2.0f );
        setDiffuse( color );
        setSpecular( color );
    }
    void CPMeshRenderer::drawBegin( )
    {
        //if ( isCreateElementNormalMap ) elementNormalMap.bind( Defines::Texture::NORMALMAP );
        //if ( isCreateElement ) element.bind( Defines::Texture::TEXTURE );
        apply( );
    }
    void CPMeshRenderer::drawEnd( )
    {
        //if ( isCreateElement ) element.unbind( Defines::Texture::TEXTURE );
        //if ( isCreateElementNormalMap ) elementNormalMap.unbind( Defines::Texture::NORMALMAP );
    }
}