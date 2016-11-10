# include "SceneTitle.h"
#include "SceneGame.h"


using namespace ci;

namespace User
{
    SceneTitle::SceneTitle( ) : isEnd( false ), logoAlpha( 0.0f ), logoAlphaSpeed( 0.0125f ), isAttack( false )
    {
        env.setWindowSize( 1280, 720 );

        Izanami::FileReader fileReader;
        Izanami::TextureMaker tex;

        //Title Texture�Ǎ�
        fileReader.make( "Json/TitleTexture.json" );

        for ( auto& path : fileReader.members( ) )
        {
            textures.add( path.first, tex.Find( path.second ) );
        }

        fileReader.clear( );
    }

    SceneTitle::~SceneTitle( )
    {
        textures.clear( );
    }

    void SceneTitle::resize( )
    {

    }

    void SceneTitle::update( )
    {
        UpdateLogoAlpha( );
        Touch( );
        slashEffect.Update( );
        if ( isAttack == true )
        {
            if ( CheckDefLineOfCircle( line, ci::Vec2f( 680, 500 ), 125.0f ) < 0.7f )
                isEnd = true;
            else
                isAttack = false;
        }

    }
    void SceneTitle::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTitle::select( )
    {
        if ( isEnd == false )return;
        textures.clear( );
        create( new SceneGame( ) );
    }

    void SceneTitle::UpdateLogoAlpha( )
    {
        logoAlpha += logoAlphaSpeed;

        if ( logoAlpha <= 0 || logoAlpha >= 1 )
            logoAlphaSpeed *= -1;
    }

    void SceneTitle::Touch( )
    {
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );
        for ( auto id : ids )
        {
            SetAttackMotionOfTouch( touch, id );
            MakeAttackMotionOfTouch( touch, id );
        }

    }
    void SceneTitle::SetAttackMotionOfTouch( cinder::app::TouchEvent::Touch touch, uint32_t id )
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

    void SceneTitle::MakeAttackMotionOfTouch( cinder::app::TouchEvent::Touch touch, uint32_t id )
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
                    isAttack = true;
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

    void SceneTitle::beginDrawUI( )
    {
        ci::gl::enable( GL_TEXTURE_2D );
        ci::gl::disable( GL_CULL_FACE );
        ci::gl::disable( GL_NORMALIZE );
        ci::gl::disable( GL_LIGHTING );
        ci::gl::enableAlphaBlending( );
        ci::gl::disableDepthRead( );
        ci::gl::disableDepthWrite( );

        ci::gl::setViewport( env.getWindowBounds( ) );
        ci::gl::setMatricesWindow( env.getWindowSize( ) );
        ci::gl::color( Color::white( ) );
    }
    void SceneTitle::drawUI( )
    {
        drawTexture( ci::Vec2f( 0, 0 ), env.getWindowSize( ), *textures.find( "view" ), ci::ColorA::white( ) );
        drawTexture( ci::Vec2f( 150, 250 ), ci::Vec2f( 400, 400 ), *textures.find( "player" ), ci::ColorA::white( ) );
        drawTexture( ci::Vec2f( 800, 150 ), ci::Vec2f( 600, 600 ), *textures.find( "explanationText" ), ci::ColorA::white( ) );
        drawTexture( ci::Vec2f( 600, 450 ), ci::Vec2f( 280, 140 ), *textures.find( "start" ), ci::ColorA( 1, 1, 1, logoAlpha ) );

        slashEffect.Draw( );
        gl::drawSolidCircle( ci::Vec2f( 680, 500 ), 125 );
    }
    void SceneTitle::endDrawUI( )
    {

    }
}