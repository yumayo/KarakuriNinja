# include "SceneResult.h"
# include "SceneTitle.h"

# include "cinder/Rand.h"

using namespace ci;

namespace User
{
    SceneResult::SceneResult( int score )
        : score( score )
        , font( u8"メイリオ", 200 )
        , audio( "SE/result.wav" )
        , isEnd( false )
        , logoAlpha( 0.0f )
        , logoAlphaSpeed( 0.0125f )
        , titleTex( loadImage( app::loadAsset( "title.png" ) ) )
    {
        audio.Play( );
    }

    SceneResult::~SceneResult( )
    {

    }

    void SceneResult::resize( )
    {

    }

    void SceneResult::update( )
    {
        inputzkoo.Resumption( );
        if ( !inputzkoo.IsActive( ) ) return;

        slashInput.Begin( );

        UpdateLogoAlpha( );

        // スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
        if ( slashInput.IsHitCircle( env.getWindowCenter( ) + Vec2f( 0, 200 ), 125 ) ) isEnd = true;

        slashInput.End( );
    }
    void SceneResult::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }

    void SceneResult::select( )
    {
        if ( isEnd ) create( new SceneTitle( ) );
    }

    void SceneResult::UpdateLogoAlpha( )
    {
        logoAlpha += logoAlphaSpeed;

        if ( logoAlpha <= 0 || logoAlpha >= 1 )
            logoAlphaSpeed *= -1;
    }

    void SceneResult::beginDrawUI( )
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
    void SceneResult::drawUI( )
    {
        gl::draw( titleTex, Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        gl::color( ColorA( 1, 1, 1, logoAlpha ) );
        gl::drawSolidCircle( env.getWindowCenter( ) + Vec2f( 0, 200 ), 125 );

        Color color( randFloat( ), randFloat( ), randFloat( ) );
        font.Draw( std::to_string( score ), env.getWindowCenter( ) + Vec2f( 0, 200 ), color );

        slashInput.Draw( );

        // 画面を薄い黒で塗りつぶす。
        if ( !inputzkoo.IsActive( ) )
        {
            gl::color( ColorA( 0, 0, 0, 0.5 ) );
            gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );
            gl::color( Color( 1, 1, 1 ) );
        }

        // ZKOOの表示
        auto hand = inputzkoo.hand( );
        for ( auto& i : inputzkoo.GetHandleIDs( ) )
        {
            if ( inputzkoo.isRecognition( i, hand ) )
            {
                gl::color( Color( 1, 1, 0 ) );
                gl::drawSolidCircle( hand.Position( ), 100, 50 );
            }
            if ( inputzkoo.isPress( i, hand ) )
            {
                gl::color( Color( 1, 1, 1 ) );
                gl::drawSolidCircle( hand.Position( ), 50, 50 );
            }
            if ( inputzkoo.isPush( i, hand ) )
            {
                if ( !inputzkoo.IsActive( ) )
                {
                    if ( inputzkoo.IsHandsActive( ) ) inputzkoo.Resumption( );
                }
            }
        }
    }
    void SceneResult::endDrawUI( )
    {

    }
}