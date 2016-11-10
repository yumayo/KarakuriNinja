# pragma once

# include "cinder/Json.h"
# include "cinder/Vector.h"
# include "cinder/Color.h"

// スラッシュに必要なヘッダーファイルです。

# include "../SlashEffect/SlashEffect.h"
# include "../Attack/AttackFactory.h"
# include "cinder/app/TouchEvent.h"
# include "ZKOO.hpp"
//

# include "cinder/Font.h"

namespace User
{
    // JSONの配列からVec3fへ
    cinder::Vec3f getVec3f( cinder::JsonTree const& params );

    // JSONの配列からColorへ
    cinder::Color getColor( cinder::JsonTree const& params );

    // JSONの配列からColorAへ
    cinder::Color getColorA( cinder::JsonTree const& params );

    class SlashInput
    {
        //============================================
        // この４つをスラッシュを発動させるために使います。
        //============================================
        Slash slashEffect;
        AttackFactory attackTask;
        Line line;
        bool isInput;
        //============================================
    public:
        SlashInput( );
    public:
        // スラッシュのエフェクトを表示します。
        void Draw( );
        // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
        void Begin( );
        // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
        void End( );
        // スラッシュとの当たり判定を取るオブジェクトを引数に入れてください。
        // CheckDefLineOfCircle のラップです。
        // Nomoto.hを参照。
        bool IsHitCircle( cinder::Vec2f pos, float size );
    private:
        // スクリーンタッチでのアップデート
        // 以下のスラッシュをアップデートします。
        void UpdateAttackMotionOfTouch( );
        // どの位置をタッチしたのかを覚えさせます。
        void SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
        // どの位置で線を引き終えたのかを判断し、スラッシュの値をセットします。
        void MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
    private:
        // ZKOOでのアップデート
        // 以下のスラッシュをアップデートします。
        void UpdateAttackMotionOfZKOO( );
        // どの位置をタッチしたのかを覚えさせます。
        void SetAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand );
        // どの位置で線を引き終えたのかを判断し、スラッシュの値をセットします。
        void MakeAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand );
    };

    class MoveInput
    {
        //============================================
        // この４つをスラッシュを発動させるために使います。
        //============================================
        Slash slashEffect;
        AttackFactory attackTask;
        Line line;
        bool isInput;
        int frame;
        int tickFrame;
        cinder::Vec2f prevPos;
        //============================================
    public:
        MoveInput( );
    public:
        // スラッシュのエフェクトを表示します。
        void Draw( );
        // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
        void Begin( );
        // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
        void End( );
        // スラッシュとの当たり判定を取るオブジェクトを引数に入れてください。
        // CheckDefLineOfCircle のラップです。
        // Nomoto.hを参照。
        bool IsHitCircle( cinder::Vec2f pos, float size );
    private:
        void FrameUpdate( );
        bool IsHandHighSpeedMove( cinder::Vec2f pos );
    private:
        // スクリーンタッチでのアップデート
        // 以下のスラッシュをアップデートします。
        void UpdateAttackMotionOfTouch( );
        // どの位置をタッチしたのかを覚えさせます。
        void SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
        // どの位置で線を引き終えたのかを判断し、スラッシュの値をセットします。
        void MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
    private:
        // ZKOOでのアップデート
        // 以下のスラッシュをアップデートします。
        void UpdateAttackMotionOfZKOO( );
        // どの位置をタッチしたのかを覚えさせます。
        void SetAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand );
        // どの位置で線を引き終えたのかを判断し、スラッシュの値をセットします。
        void MakeAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand );
    };

    class Fonts
    {
        cinder::Font font;
    public:
        enum class Mode
        {
            LEFTUP, CENTERUP, RIGHTUP
        };
        Fonts( std::string const& path = u8"メイリオ", float size = 24.0F );
        void SetFont( std::string const& path = u8"メイリオ", float size = 24.0F );
        void Draw( std::string const& string, cinder::Vec2f position, cinder::ColorA color = cinder::ColorA::white( ), Mode mode = Mode::LEFTUP );
    };

    class Timer
    {
        int frame;
        int elapseFrame;
        bool isCount;
        bool isActive;
    public:
        Timer( );
        Timer( int elapseFrame );
    public:
        void Update( );
    public:
        bool IsCount( );
        bool IsAction( );
    public:
        void Advance( );
        void Advance( int elapseFrame );
    public:
        void On( );
        void Off( );
    public:
        double NomalizedFrame( );
    };
}