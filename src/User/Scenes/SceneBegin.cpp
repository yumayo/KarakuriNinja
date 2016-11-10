# include "SceneBegin.h"

# include "GlobalData.hpp"
# include "SceneTitle.h"

using namespace cinder;

namespace User
{
    SceneBegin::SceneBegin( )
        : nowLoading( loadImage( app::loadAsset( "load.png" ) ) )
        , loadBar( loadImage( app::loadAsset( "Textures/spgage.png" ) ) )
        , kuriko( loadImage( app::loadAsset( "Textures/backninja.png" ) ) )
        , font( u8"メイリオ", 32 )
    {
        timer.Advance( 0 );
        timer.Off( );
    }
    SceneBegin::~SceneBegin( )
    {
    }
    void SceneBegin::resize( )
    {
    }
    void SceneBegin::update( )
    {
        timer.Update( );

        if ( isDraw )
        {
            GData::Start( );
        }

        if ( GData::IsSetuped( ) && !isLoad )
        {
            isLoad = true;
            timer.On( );
            timer.Advance( 120 );
        }
    }
    void SceneBegin::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneBegin::select( )
    {
        if ( timer.IsAction( ) )
            create( new SceneTitle( ) );
    }
    void SceneBegin::beginDrawUI( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::disable( GL_CULL_FACE );
        gl::disable( GL_NORMALIZE );
        gl::disable( GL_LIGHTING );
        gl::enableAlphaBlending( );
        gl::disableDepthRead( );
        gl::disableDepthWrite( );

        gl::setViewport( env.getWindowBounds( ) );
        gl::setMatricesWindow( env.getWindowSize( ) );
        gl::color( Color::white( ) );
    }
    void SceneBegin::drawUI( )
    {
        // クリコ
        gl::pushModelView( );
        gl::translate( env.getWindowCenter( ) - kuriko.getSize( ) / 4 );
        gl::draw( kuriko, Rectf( Vec2f::zero( ), kuriko.getSize( ) / 2 ) );
        kuriko.unbind( );
        gl::popModelView( );

        // NowLoading
        gl::pushModelView( );
        gl::translate( env.getWindowCenter( ) - Vec2f( nowLoading.getWidth( ), -nowLoading.getHeight( ) ) / 2 );
        gl::draw( nowLoading );
        nowLoading.unbind( );
        gl::popModelView( );

        // ロードバー
        gl::pushModelView( );
        gl::translate( env.getWindowCenter( ) - Vec2f( 500, 30 ) / 2 );
        gl::color( ColorA( 1, 1, 1, 0.25 ) );
        gl::draw( loadBar, Area( Vec2f::zero( ), Vec2f( 500, 30 ) ), Area( Vec2f::zero( ), Vec2f( 500, 30 ) ) );
        loadBar.unbind( );
        gl::color( Color::white( ) );
        gl::draw( loadBar, Area( Vec2f::zero( ), Vec2f( 500 * GData::NormalizedLoadIndex( ), 30 ) ), Area( Vec2f::zero( ), Vec2f( 500 * GData::NormalizedLoadIndex( ), 30 ) ) );
        loadBar.unbind( );
        gl::drawStrokedRect( Rectf( Vec2f::zero( ), Vec2f( 500, 30 ) ) );
        font.Draw( std::to_string( GData::NowLoadIndex( ) ) + u8"/" + std::to_string( GData::MaxLoadIndex( ) ), Vec2f( 500, 30 ), Color::white( ), Fonts::Mode::RIGHTUP );
        gl::popModelView( );

        // フェード画像
        gl::color( ColorA( 0, 0, 0, timer.NomalizedFrame( ) ) );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        isDraw = true;
    }
    void SceneBegin::endDrawUI( )
    {
        gl::popMatrices( );
    }
}