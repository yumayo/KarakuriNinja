# pragma once

# include "Component.hpp"

# include "cinder/gl/Material.h"
# include "cinder/gl/Texture.h"

namespace User
{
    class CPMeshRenderer : public Component, public cinder::gl::Material
    {
    public:
        CPMeshRenderer( );
        CPMeshRenderer( const cinder::Color& color );
        CPMeshRenderer( const cinder::Color& color, const std::string& texture );
        CPMeshRenderer( const cinder::Color& color, const std::string& texture, const std::string& normalMap );
        virtual ~CPMeshRenderer( ) { }
    public:
        virtual void SetMaterial( const cinder::Color& color );
    public:
        virtual void drawBegin( ) override;
        virtual void drawEnd( ) override;
    protected:
        cinder::gl::Texture element;
        bool isCreateElement;
        cinder::gl::Texture elementNormalMap;
        bool isCreateElementNormalMap;
    public: NAME
    };
}