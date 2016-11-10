# pragma once

# include "cinder/gl/Texture.h"

# include "CountFrame.h"

namespace User
{
    class GameStartGauge
    {
        bool isMaxed = false;
        CountFrame effectCountFrame;
        cinder::ColorA effectColorA;
        cinder::Vec2f size;
        cinder::Vec2f effectSize;
        CountFrame countFrame;
        cinder::gl::Texture* wakuTexture;
        cinder::gl::Texture* gaugeTexture;
        cinder::gl::Texture* nigiriTexture;

        cinder::Vec2f gaugeSize;
    public:
        GameStartGauge( );
        int GetLoadFrame( );
        bool IsEnd( );
        void Update( bool isSuccess );
        void Draw( );
        void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize );
    };
}