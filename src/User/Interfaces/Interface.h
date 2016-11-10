# pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"

#include "../Utilitys/Yumayo.h"

namespace User
{
    class Interface
    {
    public:
        Interface( );
        void draw( float APNormalized, float HPNormalized );
        void update( );
        void AddScore( int score ) { scorePoint += score; }
        int Score( ) { return scorePoint; }
    private:
        cinder::gl::Texture* APEdge;
        cinder::gl::Texture* HPEdge;
        cinder::gl::Texture* score;

        cinder::gl::Texture* APEdgeBase;
        cinder::gl::Texture* HPEdgeBase;

        Fonts font;
        int scorePoint;
    private:
        void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position = cinder::Vec2f::zero( ) );
        void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value );
    };

    using InterfaceRef = std::shared_ptr<Interface>;
}