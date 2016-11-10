#include "Yumayo.h"

#include "cinder/gl/gl.h"

#include "cinder/Rand.h"

# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    cinder::Vec3f getVec3f( cinder::JsonTree const & params )
    {
        Vec3f v;

        v.x = params.getValueAtIndex<float>( 0 );
        v.y = params.getValueAtIndex<float>( 1 );
        v.z = params.getValueAtIndex<float>( 2 );

        return v;
    }
    cinder::Color getColor( cinder::JsonTree const & params )
    {
        Color c;

        c.r = params.getValueAtIndex<float>( 0 );
        c.g = params.getValueAtIndex<float>( 1 );
        c.b = params.getValueAtIndex<float>( 2 );

        return c;
    }
    cinder::Color getColorA( cinder::JsonTree const & params )
    {
        ColorA c;

        c.r = params.getValueAtIndex<float>( 0 );
        c.g = params.getValueAtIndex<float>( 1 );
        c.b = params.getValueAtIndex<float>( 2 );
        c.a = params.getValueAtIndex<float>( 3 );

        return c;
    }

    SlashInput::SlashInput( )
        : isInput( false )
    {
    }
    void SlashInput::Begin( )
    {
        UpdateAttackMotionOfTouch( );
        UpdateAttackMotionOfZKOO( );
        slashEffect.Update( );
    }
    void SlashInput::Draw( )
    {
        slashEffect.Draw( );
    }
    void SlashInput::End( )
    {
        isInput = false;
    }
    bool SlashInput::IsHitCircle( cinder::Vec2f pos, float size )
    {
        return CheckDefLineOfCircle( line, pos, size ) < 1.0F && isInput;
    }
    void SlashInput::UpdateAttackMotionOfTouch( )
    {
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );
        for ( auto id : ids )
        {
            SetAttackMotionOfTouch( id, touch );
            MakeAttackMotionOfTouch( id, touch );
        }
    }
    void SlashInput::SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch )
    {
        if ( inputs.isPressTouch( id, touch ) )
        {
            attackTask.Update( );

            if ( slashEffect.Active( ) == true )
            {
                return;
            }

            //Motionに入っていない場合
            if ( attackTask.IsMotioning( ) == false && slashEffect.Active( ) == false )
            {
                attackTask.ShiftIsMotioning( );
            }

            //Slashの開始時間になったら右手の位置を描画の最初の位置に入れます
            if ( attackTask.IsStarted( ) == true )
            {
                attackTask.SetStartPos( touch.getPos( ) );
                attackTask.ShiftIsStarted( );
            }
        }
    }
    void SlashInput::MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch )
    {
        if ( inputs.isPullTouch( id, touch ) )
        {
            //終了していいなら
            if ( attackTask.IsEnded( ) == true )
            {
                attackTask.SetEndPos( touch.getPos( ) );
                attackTask.ShiftIsEnded( );

                //Lineが判定のために必要な最低基準を超えたら
                if ( CheckLineDef( attackTask.HitLine( ) ) )
                {
                    line = attackTask.HitLine( );
                    Effect effect = MakeAttackEffect( line.startPos, line.endPos );
                    slashEffect.Set( effect.pos, effect.size, effect.angle );
                    isInput = true;
                }
                //値の初期化
                attackTask.Reset( );

            }
            attackTask.Reset( );
        }
        auto ids = inputs.GetTouchHandleIDs( );
        //例外処理
        //ex) 急に画面外にいったら など
        if ( attackTask.IsMotioning( ) == true && static_cast<int>( ids.size( ) ) == 0 )
        {
            attackTask.Reset( );
        }
    }
    void SlashInput::UpdateAttackMotionOfZKOO( )
    {
        // 新しいZKOOのマクロを使用して動かします。
        // 内部でスクリーンタッチと同じ扱いができるようにしているので、こちらも同じような処理になっています。
        auto ids = inputzkoo.GetHandleIDs( );
        auto hand = inputzkoo.hand( );
        for ( auto id : ids )
        {
            SetAttackMotionOfZKOO( id, hand );
            MakeAttackMotionOfZKOO( id, hand );
        }
    }
    void SlashInput::SetAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand )
    {
        if ( inputzkoo.isPress( id, hand ) )
        {
            attackTask.Update( );

            if ( slashEffect.Active( ) == true )
            {
                return;
            }

            //Motionに入っていない場合
            if ( attackTask.IsMotioning( ) == false && slashEffect.Active( ) == false )
            {
                attackTask.ShiftIsMotioning( );
            }

            //Slashの開始時間になったら右手の位置を描画の最初の位置に入れます
            if ( attackTask.IsStarted( ) == true )
            {
                attackTask.SetStartPos( hand.Position( ) );
                attackTask.ShiftIsStarted( );
            }
        }
    }
    void SlashInput::MakeAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand )
    {
        if ( inputzkoo.isPull( id, hand ) )
        {
            //終了していいなら
            if ( attackTask.IsEnded( ) == true )
            {
                attackTask.SetEndPos( hand.Position( ) );
                attackTask.ShiftIsEnded( );

                //Lineが判定のために必要な最低基準を超えたら
                if ( CheckLineDef( attackTask.HitLine( ) ) )
                {
                    line = attackTask.HitLine( );
                    Effect effect = MakeAttackEffect( line.startPos, line.endPos );
                    slashEffect.Set( effect.pos, effect.size, effect.angle );
                    isInput = true;
                }
                //値の初期化
                attackTask.Reset( );

            }
            attackTask.Reset( );
        }
        auto ids = inputzkoo.GetHandleIDs( );
        //例外処理
        //ex) 急に画面外にいったら など
        if ( attackTask.IsMotioning( ) == true && static_cast<int>( ids.size( ) ) == 0 )
        {
            attackTask.Reset( );
        }
    }

    Fonts::Fonts( std::string const& path, float size )
        : font( path, size )
    {
    }
    void Fonts::SetFont( std::string const& path, float size )
    {
        font = Font( path, size );
    }
    void Fonts::Draw( std::string const& string, cinder::Vec2f position, cinder::ColorA color )
    {
        auto fontSize = font.getSize( );

        auto stringGlyphs = font.getGlyphs( string );

        float boundingLength = 0;
        float maxBoundingHeight = 0;
        for ( auto& obj : stringGlyphs )
        {
            auto box = font.getGlyphBoundingBox( obj );
            boundingLength += box.x1 + box.x2;
            if ( maxBoundingHeight < box.y1 + box.y2 )
            {
                maxBoundingHeight = box.y1 + box.y2;
            }
        }

        // メイリオは細かい誤差を補正しています。
        if ( font.getName( ) == u8"メイリオ" )
        {
            boundingLength += fontSize / 4.0F;

            auto clearance = fontSize - maxBoundingHeight - fontSize / 8.0F;

            auto size = Vec2f( boundingLength, maxBoundingHeight );

            auto correction = Vec2f( 0, -clearance );

            gl::drawString( string, position + correction - size / 2.0F, color, font );

        #ifdef _DEBUG
            gl::drawStrokedRect( Rectf( position - size / 2.0F, position + size - size / 2.0F ) );
        #endif // _DEBUG
        }
        // 今のところメイリオ以外は誤差を補正しきれていません。
        else
        {
            auto size = Vec2f( boundingLength, maxBoundingHeight );

            gl::drawString( string, position - size / 2.0F, color, font );

        #ifdef _DEBUG
            gl::drawStrokedRect( Rectf( position - size / 2.0F, position + size - size / 2.0F ) );
        #endif // _DEBUG
        }
    }

    Timer::Timer( )
        : frame( 0 )
        , elapseFrame( randInt( 60, 120 ) )
        , isActive( false )
    {
        On( );
    }
    Timer::Timer( int elapseFrame )
        : frame( 0 )
        , elapseFrame( elapseFrame )
        , isActive( false )
    {
        On( );
    }
    bool Timer::IsAction( )
    {
        return elapseFrame < frame && isActive;
    }
    void Timer::Update( )
    {
        frame += static_cast<int>( isActive );
    }
    void Timer::Advance( )
    {
        frame = 0;
        elapseFrame = randInt( 60, 120 );
    }
    void Timer::Advance( int elapseFrame )
    {
        frame = 0;
        this->elapseFrame = elapseFrame;
    }
    void Timer::On( )
    {
        isActive = true;
    }
    void Timer::Off( )
    {
        isActive = false;
    }

    double Timer::NomalizedFrame( )
    {
        return static_cast<double>( frame ) / elapseFrame;
    }
}