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
    private:
        // 普通に進む時の画像
        cinder::gl::Texture* touchToNext;
        // 攻撃するときの画像
        cinder::gl::Texture* kougeki;
        // 防御するときの画像
        cinder::gl::Texture* bougyo;

        Mode mode = Mode::NORMAL;
    public:
        Description( );
    public:
        // 右下を起点に描画します。
        void Draw( cinder::Vec2f position );
        void SetMode( Mode mode ) { this->mode = mode; }
    private:
        void DrawNORMAL( cinder::Vec2f position );
        void DrawKOUGEKI( cinder::Vec2f position );
        void DrawBOUGYO( cinder::Vec2f position );
    private:
        void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f offset, cinder::Vec2f cutSize );
    };

    using DescriptionRef = std::shared_ptr<Description>;

}