# pragma once

# include <memory>

# include "cinder/Vector.h"
# include "cinder/gl/Texture.h"

namespace User
{
    class Description
    {
    public:
        enum Mode
        {
            NORMAL, KOUGEKI, BOUGYO
        };
    private:
        int frame = 0;
        float scale = 1.0F;
    private:
        // ���ʂɐi�ގ��̉摜
        cinder::gl::Texture* touchToNext;
        // �U������Ƃ��̉摜
        cinder::gl::Texture* kougeki;
        // �h�䂷��Ƃ��̉摜
        cinder::gl::Texture* bougyo;

        Mode mode = Mode::NORMAL;
    public:
        Description( );
    public:
        // ������N�_�ɕ`�悵�܂��B
        void Draw( cinder::Vec2f position, cinder::Vec2f size = cinder::Vec2f(768, 256) / 1.5F );
        void SetMode( Mode mode ) { this->mode = mode; }
    private:
        void DrawNORMAL( cinder::Vec2f position, cinder::Vec2f size );
        void DrawKOUGEKI( cinder::Vec2f position, cinder::Vec2f size );
        void DrawBOUGYO( cinder::Vec2f position, cinder::Vec2f size );
    private:
        void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize );
    };

    using DescriptionRef = std::shared_ptr<Description>;

}