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

    MoveInput::MoveInput( )
        : isInput( false )
        , tickFrame( 40 )
        , frame( tickFrame )
    {
    }
    void MoveInput::Begin( )
    {
        UpdateAttackMotionOfTouch( );
        UpdateAttackMotionOfZKOO( );
        FrameUpdate( );
        slashEffect.Update( );
    }
    void MoveInput::Draw( )
    {
        slashEffect.Draw( );
    }
    void MoveInput::End( )
    {
        isInput = false;
    }
    bool MoveInput::IsHitCircle( cinder::Vec2f pos, float size )
    {
        return CheckDefLineOfCircle( line, pos, size ) < 1.0F && isInput;
    }
    void MoveInput::FrameUpdate( )
    {
        if ( frame == 0 ) frame = tickFrame;
        frame = std::max( frame - 1, 0 );
    }
    bool MoveInput::IsHandHighSpeedMove( cinder::Vec2f pos )
    {
        CONSOLE << prevPos.distance( pos ) << std::endl;
        return 50 < prevPos.distance( pos );
    }
    void MoveInput::UpdateAttackMotionOfTouch( )
    {
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );
        for ( auto id : ids )
        {
            SetAttackMotionOfTouch( id, touch );
            MakeAttackMotionOfTouch( id, touch );
        }
    }
    void MoveInput::SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch )
    {
        if ( frame == tickFrame && inputs.isPressTouch( id, touch ) )
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
                prevPos = touch.getPos( );
                attackTask.ShiftIsStarted( );
            }
        }
    }
    void MoveInput::MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch )
    {
        if ( frame == 0 && inputs.isPressTouch( id, touch ) && IsHandHighSpeedMove( touch.getPos( ) ) )
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
    void MoveInput::UpdateAttackMotionOfZKOO( )
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
    void MoveInput::SetAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand )
    {
        if ( frame == tickFrame && inputzkoo.isRecognition( id, hand ) )
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
                prevPos = hand.Position( );
                attackTask.ShiftIsStarted( );
            }
        }
    }
    void MoveInput::MakeAttackMotionOfZKOO( uint32_t id, ZKOOHand& hand )
    {
        if ( frame == 0 && inputzkoo.isRecognition( id, hand ) && IsHandHighSpeedMove( hand.Position( ) ) )
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
    void Fonts::Draw( std::string const & string, cinder::Vec2f position, cinder::ColorA color, Mode mode )
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
            boundingLength += fontSize / 4.0F; // �������̌덷
            auto clearance = fontSize - maxBoundingHeight - fontSize / 8.0F;
            auto correction = Vec2f( 0, -clearance ); // �c�����̌덷

            auto size = Vec2f( boundingLength, maxBoundingHeight );
            Vec2f trans;
            switch ( mode )
            {
            case User::Fonts::Mode::LEFTDOWN:
                trans = Vec2f( 0, 0 );
                break;
            case User::Fonts::Mode::CENTERCENTER:
                trans = -size / 2.0F;
                break;
            case User::Fonts::Mode::RIGHTDOWN:
                trans = Vec2f( -size.x, 0 );
                break;
            default:
                break;
            }

            gl::pushModelView( );
            gl::translate( trans );
            gl::drawString( string, position + correction, color, font );

        #ifdef _DEBUG
            gl::drawStrokedRect( Rectf( position, position + size ) );
        #endif // _DEBUG
            gl::popModelView( );
        }
        // ���̂Ƃ��냁�C���I�ȊO�͌덷��␳������Ă��܂���B
        else
        {
            auto size = Vec2f( boundingLength, maxBoundingHeight );
            Vec2f trans;
            switch ( mode )
            {
            case User::Fonts::Mode::LEFTDOWN:
                trans = Vec2f( 0, 0 );
                break;
            case User::Fonts::Mode::CENTERCENTER:
                trans = -size / 2.0F;
                break;
            case User::Fonts::Mode::RIGHTDOWN:
                trans = Vec2f( -size.x, 0 );
                break;
            default:
                break;
            }
            gl::pushModelView( );
            gl::translate( trans );
            gl::drawString( string, position, color, font );

        #ifdef _DEBUG
            gl::drawStrokedRect( Rectf( position, position + size ) );
        #endif // _DEBUG
            gl::popModelView( );
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