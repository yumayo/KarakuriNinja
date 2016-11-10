#include "Yumayo.h"

#include "cinder/gl/gl.h"

#include "cinder/Rand.h"

# include "cinder/app/App.h"

# include "cinder/gl/TextureFont.h"

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
        , slashEffect( 0 )
    {
    }
    void SlashInput::Begin( )
    {
        UpdateAttackMotionOfTouch( );
    }
    void SlashInput::Draw( )
    {
        slashEffect.Update( );
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

    SlashLine::SlashLine( ::Line line, Effect effect, int combo )
        : isActive( true )
        , slashEffect( combo )
    {
        this->line = line;
        slashEffect.Set( effect.pos, effect.size, effect.angle );
    }

    MoveTouch::MoveTouch( cinder::Vec2f pos )
        : isActive( true )
        , prevPos( pos )
    {
    }

    void MoveInput::Begin( int combo )
    {
        UpdateAttackMotionOfTouch( combo );

        slashEffect.remove_if( [ ] ( SlashLine& slashLine ) { return !slashLine.IsThisErase( ); } );
        for ( auto& obj : slashEffect ) obj.Update( );
    }
    void MoveInput::Draw( )
    {
        for ( auto& obj : slashEffect ) obj.Draw( );
    }
    void MoveInput::End( )
    {
        for ( auto& obj : slashEffect )
        {
            obj.UsedLine( );
        }
    }
    bool MoveInput::IsHitCircle( cinder::Vec2f pos, float size )
    {
        bool isHit = false;
        for ( auto& obj : slashEffect )
        {
            if ( obj.IsActive( ) )
            {
                isHit = isHit && ( CheckDefLineOfCircle( obj.Line( ), pos, size ) < 1.0F );
            }
        }
        return isHit;
    }
    std::vector<Line> MoveInput::Lines( )
    {
        std::vector<Line> lines;
        for ( auto& obj : slashEffect )
        {
            if ( obj.IsActive( ) )
            {
                lines.emplace_back( obj.Line( ) );
            }
        }
        return lines;
    }
    void MoveInput::InputInvalidation( )
    {
        moveTouch.clear( );
    }
    bool MoveInput::IsHandMove( uint32_t id, cinder::Vec2f pos )
    {
        return 300 < moveTouch[id].prevPos.distance( pos );
    }
    void MoveInput::UpdateAttackMotionOfTouch( int combo )
    {
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );
        for ( auto id : ids )
        {
            SetAttackMotionOfTouch( id, touch );
            MakeAttackMotionOfTouch( id, touch, combo );
        }
    }
    void MoveInput::SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch )
    {
        if ( inputs.isPushTouch( id, touch ) )
        {
            moveTouch.insert( std::make_pair( id, MoveTouch( touch.getPos( ) ) ) );
        }

        if ( inputs.isPullTouch( id, touch ) )
        {
            moveTouch.erase( id );
        }
    }
    void MoveInput::MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch, int combo )
    {
        auto findItr = moveTouch.find( id );
        if ( findItr == moveTouch.end( ) ) return;

        auto& moveTouch = findItr->second;
        if ( !moveTouch.isActive ) return;

        if ( inputs.isPressTouch( id, touch ) && IsHandMove( id, touch.getPos( ) ) )
        {
            Line line;
            line.startPos = moveTouch.prevPos;
            line.endPos = touch.getPos( );

            moveTouch.isActive = false;
            slashEffect.emplace_back( line, MakeAttackEffect( line.startPos, line.endPos ), combo );
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
    void Fonts::Draw( std::string const & string, cinder::Vec2f position, cinder::ColorA color, Mode mode )
    {
        switch ( mode )
        {
        case User::Fonts::Mode::LEFTUP:
            gl::drawString( string, position, color, font );
            break;
        case User::Fonts::Mode::CENTERUP:
            gl::drawStringCentered( string, position, color, font );
            break;
        case User::Fonts::Mode::RIGHTUP:
            gl::drawStringRight( string, position, color, font );
            break;
        default:
            break;
        }
    }

    Rectf Fonts::BoundingBox( std::string const & string )
    {
        // cinder/gl/gl.cppより

        if ( string.empty( ) ) return Rectf( );

        float baselineOffset;

        gl::Texture tex( renderString( string, font, Color::white( ), &baselineOffset ) );

        return Rectf( 0, 0, tex.getWidth( ), tex.getHeight( ) - baselineOffset );
    }

    Timer::Timer( )
        : frame( 0 )
        , elapseFrame( randInt( 60, 120 ) )
        , isActive( false )
        , isCount( false )
    {
        On( );
    }
    Timer::Timer( int elapseFrame )
        : frame( 0 )
        , elapseFrame( elapseFrame )
        , isActive( false )
        , isCount( false )
    {
        On( );
    }
    bool Timer::IsCount( )
    {
        return isCount && isActive;
    }
    bool Timer::IsActionOnePrevFrame( )
    {
        return elapseFrame == frame + 1 && isActive;
    }
    bool Timer::IsAction( )
    {
        return elapseFrame == frame && isActive;
    }
    void Timer::Update( )
    {
        frame = std::min( frame + static_cast<int>( isActive ), elapseFrame );
        isCount = !( elapseFrame != frame );
    }
    void Timer::Advance( )
    {
        frame = 0;
        elapseFrame = randInt( 60, 120 );
        isCount = true;
    }
    void Timer::Advance( int elapseFrame )
    {
        frame = 0;
        this->elapseFrame = elapseFrame;
        isCount = true;
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