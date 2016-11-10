# include "SceneTitle.h"
#include "SceneGame.h"


using namespace ci;

namespace User
{
    SceneTitle::SceneTitle( )
        : font( u8"メイリオ", 80 )
        , logoAlpha( 0.0f )
        , logoAlphaSpeed( 0.0125f )
        , isEnd( false )
    {
        env.setWindowSize( 1280, 720 );

        Izanami::FileReader fileReader;
        Izanami::TextureMaker tex;

        //Title Texture読込
        fileReader.make( "Json/TitleTexture.json" );

        for ( auto& path : fileReader.members( ) )
        {
            textures.add( path.first, tex.Find( path.second ) );
        }

        startButtonPosition = env.getWindowCenter( ) + Vec2f( 0, 100 );
    }

    SceneTitle::~SceneTitle( )
    {
        textures.clear( );
    }

    void SceneTitle::resize( )
    {
        startButtonPosition = env.getWindowCenter( ) + Vec2f( 0, 100 );
    }

    void SceneTitle::update( )
    {
        slashInput.Begin( );

        UpdateLogoAlpha( );

        // スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
        if ( slashInput.IsHitCircle( startButtonPosition, 125 ) ) isEnd = true;

        slashInput.End( );
    }
    void SceneTitle::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTitle::select( )
    {
        if ( isEnd == false ) return;
        create( new SceneGame( ) );
    }

    void SceneTitle::UpdateLogoAlpha( )
    {
        logoAlpha += logoAlphaSpeed;

        if ( logoAlpha <= 0 || logoAlpha >= 1 )
            logoAlphaSpeed *= -1;
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

        // drawTextureでアルファブレンディングをOFFにしているので。
        ci::gl::enableAlphaBlending( );

        // スタートボタン
        gl::color( ci::ColorA( 1, 1, 1, logoAlpha ) );
        gl::draw( *textures.find( "start" ), Rectf( startButtonPosition.x - 125.0F, startButtonPosition.y - 125.0F,
                                                    startButtonPosition.x + 125.0F, startButtonPosition.y + 125.0F ) );
        font.Draw( u8"スタート", startButtonPosition );

        slashInput.Draw( );
    }
    void SceneTitle::endDrawUI( )
    {

    }
}