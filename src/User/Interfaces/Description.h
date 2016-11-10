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
        // •’Ê‚Éi‚ÞŽž‚Ì‰æ‘œ
        cinder::gl::Texture* touchToNext;
        // UŒ‚‚·‚é‚Æ‚«‚Ì‰æ‘œ
        cinder::gl::Texture* kougeki;
        // –hŒä‚·‚é‚Æ‚«‚Ì‰æ‘œ
        cinder::gl::Texture* bougyo;

        Mode mode = Mode::NORMAL;
    public:
        Description( );
    public:
        // ‰E‰º‚ð‹N“_‚É•`‰æ‚µ‚Ü‚·B
        void Draw( cinder::Vec2f position );
        void SetMode( Mode mode ) { this->mode = mode; }
    private:
        void DrawNORMAL( cinder::Vec2f position );
        void DrawKOUGEKI( cinder::Vec2f position );
        void DrawBOUGYO( cinder::Vec2f position );
    private:
        void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize );
    };

    using DescriptionRef = std::shared_ptr<Description>;

}