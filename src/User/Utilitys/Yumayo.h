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

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

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

    class Fonts
    {
        cinder::Font font;
    public:
        Fonts( std::string const& path = u8"メイリオ", float size = 24.0F );
        void SetFont( std::string const& path = u8"メイリオ", float size = 24.0F );
        void Draw( std::string const& string, cinder::Vec2f position, cinder::ColorA color = cinder::ColorA::white( ) );
    };

    class Timer
    {
        int frame;
        int elapseFrame;
        bool isActive;
    public:
        Timer( );
        Timer( int elapseFrame );
    public:
        void Update( );
    public:
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

    class Audio
    {
    public:
        Audio( const std::string& filePath, const float initialValue = 1.0f );
        ~Audio( );
    public:
        void Play( );
        void Stop( );
        bool IsPlaying( );
        void Looping( const bool islooping );
        void Gain( const float gainSize );
    public:
        void SetLoopBeginTime( const double positionSeconds );
        void SetLoopEndTime( const double positionSeconds );
        double GetLoopBeginTime( ) const;
        double GetLoopEndTime( ) const;
        void SeekToTime( const double positionSeconds );
    private:
        cinder::audio::BufferPlayerNodeRef bufferPlayerRef;
        cinder::audio::GainNodeRef gainRef;
    };
}