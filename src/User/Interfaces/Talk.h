# pragma once

# include <memory>

# include "../Utilitys/Yumayo.h"
# include "cinder/Vector.h"

namespace User
{
    class Talk
    {
        cinder::Vec2f position;
        Fonts font;
    public:
        Talk( cinder::Vec2f position );
    public:
        bool IsAnyPullTouch( );
        bool IsAnyPushTouch( );
        bool IsAnyPressTouch( );
        void Update( );
        void Draw( );
    };

    using TalkRef = std::shared_ptr<Talk>;

}