# pragma once

# include <memory>

# include "../Utilitys/Yumayo.h"
# include "cinder/Vector.h"

namespace User
{
    class Talk
    {
        Fonts font;
    public:
        Talk( );
    public:
        bool IsAnyPullTouch( );
        bool IsAnyPushTouch( );
        bool IsAnyPressTouch( );
        void Update( );
        void Draw( cinder::Vec2f position );
    };

    using TalkRef = std::shared_ptr<Talk>;

}