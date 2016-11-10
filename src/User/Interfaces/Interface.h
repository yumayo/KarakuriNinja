# pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

namespace User
{
    class Interface
    {
    public:
        Interface( );
        void draw( float APNormalized, float HPNormalized );
        void update( );
    private:
        cinder::gl::Texture playerIconEdge;
        cinder::gl::Texture APEdge;
        cinder::gl::Texture HPEdge;
        cinder::gl::Texture score;

        cinder::gl::Texture APEdgeBase;
        cinder::gl::Texture HPEdgeBase;
    private:
        void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position = cinder::Vec2f::zero( ) );
        void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value );
    };
}