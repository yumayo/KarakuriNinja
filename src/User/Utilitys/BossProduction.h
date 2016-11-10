# pragma once

# include "cinder/Vector.h"
# include "cinder/Color.h"
# include "cinder/gl/Texture.h"

# include "CountFrame.h"
# include "Yumayo.h"

namespace User
{
    class Rect
    {
    protected:
        CountFrame out;
        CountFrame in;
        float height = 0;
        float maxHeight = 200;
        bool isEnd = false;
    public:
        Rect( );
        void End( );
        bool IsEnd( );
        void Update( );
        virtual void Draw( ) = 0;
    };
    class UpRect : public Rect
    {
    public: void Draw( ) override;
    };

    class DownRect : public Rect
    {
    public: void Draw( ) override;
    };

    class WarningRect
    {
        cinder::Vec2f position;
        cinder::Vec2f size;
        float offset_x;
        cinder::gl::Texture* warning;
    public:
        WarningRect( cinder::Vec2f position, cinder::Vec2f size );
        void Update( );
        void Draw( );
    private:
        void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize );
    };
    class WarningLine
    {
        bool isEnd = false;
        CountFrame out;
        cinder::Vec2f initPosition;
        cinder::Vec2f position;
        float angle;
        std::vector<WarningRect> line;
    public:
        WarningLine( cinder::Vec2f position, float angle );
        void Update( );
        void Draw( );
        void End( );
        bool IsEnd( );
    };

    class BossSpawnMoji
    {
        bool isEnd = false;
        CountFrame out;
        cinder::Vec2f position;
        cinder::Vec2f initPosition;
        Fonts font;
    public:
        BossSpawnMoji( );
        void Update( );
        void Draw( );
        void End( );
        bool IsEnd( );
    };

    class BossWarning
    {
        bool isStart = false;
        Audio* se;
        int sePlayNum = 2;
    public:
        BossWarning( );
        void Start( );
        void Update( );
        bool First( );
        bool Second( );
        bool Thread( );
        bool IsEnd( );
    };

    class BossProduction
    {
        bool isStart = false;
        bool isEnd = false;
        UpRect upRect;
        WarningLine upLine;
        DownRect downRect;
        WarningLine downLine;
        BossSpawnMoji spawnMoji;
        BossWarning bossWarning;
    public:
        BossProduction( );
        bool IsMainBGMGainDown( );
        bool IsMainBGMStop( );
        bool IsBossBGMStart( );
        void Start( );
        void Update( );
        void Draw( );
    };
}