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

            //Motion�ɓ����Ă��Ȃ��ꍇ
            if ( attackTask.IsMotioning( ) == false && slashEffect.Active( ) == false )
            {
                attackTask.ShiftIsMotioning( );
            }

            //Slash�̊J�n���ԂɂȂ�����E��̈ʒu��`��̍ŏ��̈ʒu�ɓ���܂�
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
            //�I�����Ă����Ȃ�
            if ( attackTask.IsEnded( ) == true )
            {
                attackTask.SetEndPos( touch.getPos( ) );
                attackTask.ShiftIsEnded( );

                //Line������̂��߂ɕK�v�ȍŒ��𒴂�����
                if ( CheckLineDef( attackTask.HitLine( ) ) )
                {
                    line = attackTask.HitLine( );
                    Effect effect = MakeAttackEffect( line.startPos, line.endPos );
                    slashEffect.Set( effect.pos, effect.size, effect.angle );
                    isInput = true;
                }
                //�l�̏�����
                attackTask.Reset( );

            }
            attackTask.Reset( );
        }
        auto ids = inputs.GetTouchHandleIDs( );
        //��O����
        //ex) �}�ɉ�ʊO�ɂ������� �Ȃ�
        if ( attackTask.IsMotioning( ) == true && static_cast<int>( ids.size( ) ) == 0 )
        {
            attackTask.Reset( );
        }
    }
    void SlashInput::UpdateAttackMotionOfZKOO( )
    {
        // �V����ZKOO�̃}�N�����g�p���ē������܂��B
        // �����ŃX�N���[���^�b�`�Ɠ����������ł���悤�ɂ��Ă���̂ŁA������������悤�ȏ����ɂȂ��Ă��܂��B
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

            //Motion�ɓ����Ă��Ȃ��ꍇ
            if ( attackTask.IsMotioning( ) == false && slashEffect.Active( ) == false )
            {
                attackTask.ShiftIsMotioning( );
            }

            //Slash�̊J�n���ԂɂȂ�����E��̈ʒu��`��̍ŏ��̈ʒu�ɓ���܂�
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
            //�I�����Ă����Ȃ�
            if ( attackTask.IsEnded( ) == true )
            {
                attackTask.SetEndPos( hand.Position( ) );
                attackTask.ShiftIsEnded( );

                //Line������̂��߂ɕK�v�ȍŒ��𒴂�����
                if ( CheckLineDef( attackTask.HitLine( ) ) )
                {
                    line = attackTask.HitLine( );
                    Effect effect = MakeAttackEffect( line.startPos, line.endPos );
                    slashEffect.Set( effect.pos, effect.size, effect.angle );
                    isInput = true;
                }
                //�l�̏�����
                attackTask.Reset( );

            }
            attackTask.Reset( );
        }
        auto ids = inputzkoo.GetHandleIDs( );
        //��O����
        //ex) �}�ɉ�ʊO�ɂ������� �Ȃ�
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

        // ���C���I�ׂ͍����덷��␳���Ă��܂��B
        if ( font.getName( ) == u8"���C���I" )
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
        // ���̂Ƃ��냁�C���I�ȊO�͌덷��␳������Ă��܂���B
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