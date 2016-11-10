# pragma once

# include <memory>

# include "../Utilitys/Yumayo.h"
# include "cinder/Vector.h"
# include "cinder/gl/Texture.h"

namespace User
{
    class Talk
    {
        cinder::gl::Texture* messageBoxLeft;
        cinder::gl::Texture* messageBoxCenter;
        cinder::gl::Texture* messageBoxRight;
        Fonts font;
    public:
        Talk( );
    public:
        bool IsAnyPullTouch( );
        bool IsAnyPushTouch( );
        bool IsAnyPressTouch( );
        void Update( );

        // �������N�_�ɕ`�悵�܂��B
        void Draw( cinder::Vec2f position );
    };

    using TalkRef = std::shared_ptr<Talk>;

}