# include "SceneTitle.h"
# include "SceneTutorial.h"
# include "SceneGame.h"

#include "GlobalData.hpp"

using namespace ci;

namespace User
{
    SceneTitle::SceneTitle( )
        : isEnd( false )
        , endt_( 0.0f )
        , isblackout( false )
    {
        bgm = &GData::FindAudio( "BGM/mainbgm1.wav" );
        bgm->Looping( true );
        bgm->Gain( 0.5f );
        bgm->Play( );
    }

    SceneTitle::~SceneTitle( )
    {
        bgm->Stop( );
    }

    void SceneTitle::resize( )
    {

    }

    void SceneTitle::update( )
    {
        moveInput.Begin( );

        // �X���b�V���Ƃ̓����蔻������ɂ́A�~�̒��S�|�W�V�����Ɣ��a�����܂��B
        //if (slashInput.IsHitCircle(startButtonPosition, 125)) isEnd = true;
        if ( !isblackout ) {
            isblackout = isStartTouch( );
        }
        if ( isblackout ) {
            Easing::tCount( endt_, 2.0f );
        }
        if ( endt_ >= 1.0f )isEnd = true;

        moveInput.End( );
    }
    void SceneTitle::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTitle::select( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_g ) )
        {
            create( new SceneTutorial( ) );
            return;
        }

        if ( isEnd )
        {
            create( new SceneTutorial( ) );
            return;
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
        drawTexture( ci::Vec2f( 0, 0 ), env.getWindowSize( ), GData::FindTexture( "Textures/Title/title.png" ), ci::ColorA::white( ) );

        // drawTexture�ŃA���t�@�u�����f�B���O��OFF�ɂ��Ă���̂ŁB
        ci::gl::enableAlphaBlending( );

        moveInput.Draw( );
        drawfade( );
    }
    void SceneTitle::endDrawUI( )
    {

    }
    bool SceneTitle::isStartTouch( )
    {
        auto touch = inputs.touch( );
        auto ids = inputs.GetTouchHandleIDs( );

        for ( auto id : ids )
        {
            //////�Ƃ肠��������Ńv���C���[�A�C�R���̋߂����v�b�V������ƃX�y�V�������[�h�ɓ����悤�ɂ��Ă����܂���
            if ( inputs.isPushTouch( id, touch ) )
            {
                return true;
            }
            //////
        }
        return false;
    }
    void SceneTitle::drawfade( )
    {
        gl::pushModelView( );
        gl::translate( env.getWindowSize( ) / 2 );
        gl::color( ColorA( 0, 0, 0, endt_ ) );
        gl::drawSolidRect( Rectf( -env.getWindowSize( ) / 2, env.getWindowSize( ) / 2 ) );
        gl::popModelView( );
    }
}