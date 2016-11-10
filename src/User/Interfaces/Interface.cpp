#include "Interface.h"
#include "Framework.hpp"

#include "GlobalData.hpp"
#include"../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    Interface::Interface( )
        : APEdge( &GData::FindTexture( "UI/APEdge.png" ) )
        , HPEdge( &GData::FindTexture( "UI/HPEdge.png" ) )
        , score( &GData::FindTexture( "UI/Score.png" ) )
        , APEdgeBase( &GData::FindTexture( "UI/APEdgeBase.png" ) )
        , APEdgeBasemax( &GData::FindTexture( "UI/APEdgeBasemax.png" ) )
        , HPEdgeBase( &GData::FindTexture( "UI/HPEdgeBase.png" ) )
        , HPEdgeBasedamage( &GData::FindTexture( "UI/HPEdgeBase2.png" ) )
        , specialeffect( &GData::FindTexture( "Textures/Effect/effectgage.png" ) )
        , makimono( &GData::FindTexture( "Textures/makimono.png" ) )
        , font( u8"HG行書体", 120 )
        , touchfont( u8"HG行書体", 30 )
        , scorePoint( 0 )
        , count( 0 )
        , maxcount( 0 )
        , nomarl_t( 1.f )
        , savenormal( 1.0f )
        , easingnormal( 1.0f )
        , beginnormal( 1.0f )
        , delay_t( 1.0f )
    { }

    void Interface::draw( float APNormalized, float HPNormalized, bool ismpmax, int specialsubtime )
    {
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );

        drawSpecialEffect( specialsubtime );
        drawchargeEffect( ismpmax );

        drawScore( );

        textureDraw( *HPEdge, leftDown );


        {
            if ( savenormal != HPNormalized ) {
                nomarl_t = 0.0f;
                delay_t = 0.0f;
                beginnormal = easingnormal;
                savenormal = HPNormalized;
            }
            Easing::tCount( delay_t, 1.0f );
            if ( delay_t >= 1.0f )
                Easing::tCount( nomarl_t, 0.5f );
            easingnormal = Easing::getEasing[Easing::Linear]( nomarl_t, beginnormal, savenormal );
            textureDraw( *HPEdgeBasedamage, leftDown,
                         Area( 225, 0, 639, HPEdgeBase->getHeight( ) ), easingnormal );
        }

        textureDraw( *HPEdgeBase, leftDown,
                     Area( 225, 0, 639, HPEdgeBase->getHeight( ) ), HPNormalized );

        auto translateAPEdgePosition = Vec2f( 226, -101 );
        textureDraw( *APEdge, leftDown + translateAPEdgePosition );
        textureDraw( *APEdgeBase, leftDown + translateAPEdgePosition,
                     Area( 15, 0, 340, APEdgeBase->getHeight( ) ), APNormalized );
        if ( ismpmax ) {
            maxbaseDraw( *APEdgeBasemax, leftDown + translateAPEdgePosition,
                         Area( 15, 0, 340, APEdgeBase->getHeight( ) ), APNormalized );
            maxcount++;
        }
        else {
            maxcount = 0.0f;
        }

        drawInfo( specialsubtime );
        drawchargeInfo( ismpmax );


        ranking.draw( );
    }

    void Interface::update( int nowHP )
    {
        combo.Update( );
        UpdateRanking( nowHP );
    }

    void Interface::PlusCombo( int attackSuccessNum )
    {
        combo.PlusCombo( attackSuccessNum );
    }

    void Interface::ResetCombo( )
    {
        combo.ResetCombo( );
    }

    void Interface::drawScore( )
    {
        // 全体移動
        gl::pushModelView( );
        gl::translate( env.getWindowSize( ) - Vec2f( 30, 30 ) );

        // スコアベース表示
        gl::pushModelView( );
        gl::translate( -score->getSize( ) );
        gl::color( Color::white( ) );
        gl::draw( *score, Rectf( Vec2f::zero( ), score->getSize( ) ) );
        score->unbind( );
        gl::popModelView( );

        // スコアの文字を表示
        gl::translate( Vec2f( -154, -score->getHeight( ) + 30 ) );
        gl::color( Color::white( ) );
        font.Draw( std::to_string( scorePoint ), Vec2f::zero( ), Color::white( ), Fonts::Mode::RIGHTUP );

        combo.Draw( Vec2f( -100, -180 ) );

        // 全体移動終了
        gl::popModelView( );
    }

    void Interface::UpdateRanking( int nowHP )
    {
        ranking.update( Score( ), MaxComboNumber( ), nowHP, true );
    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position + Vec2f( texture.getWidth( ) / 2.0F, -texture.getHeight( ) / 2.0F ) );
        gl::color( Color::white( ) );
        gl::draw( texture );
        texture.unbind( );
        gl::popModelView( );
    }

    void Interface::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Area area, float size )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position + Vec2f( texture.getWidth( ) / 2.0F, -texture.getHeight( ) / 2.0F ) );
        gl::color( Color::white( ) );
        gl::draw( texture,
                  Area( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ),
                  Rectf( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ) );
        texture.unbind( );
        gl::popModelView( );
    }
    void Interface::maxbaseDraw( cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Area area, float size )
    {
        gl::pushModelView( );
        gl::translate( -texture.getSize( ) / 2.0F + position + Vec2f( texture.getWidth( ) / 2.0F, -texture.getHeight( ) / 2.0F ) );
        gl::color( ColorA( 1, 1, 1, 0.5 + 0.5*sin( float( maxcount )*0.6f ) ) );
        gl::draw( texture,
                  Area( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ),
                  Rectf( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ) );
        texture.unbind( );
        gl::color( Color::white( ) );
        gl::popModelView( );
    }
    void Interface::drawSpecialEffect( int specialsubtime )
    {

        if ( specialsubtime == 3 ) {//何も選ばれていない
            count = 0;
            return;
        }
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );
        Vec2f texsize = Vec2f( 370, 340 );
        const int CUTNUM = 10;
        gl::pushModelView( );
        gl::translate( leftDown + Vec2f( 120, -140 ) );
        gl::scale( Vec2f( texsize.x / ( specialeffect->getSize( ).x / CUTNUM ), texsize.y / specialeffect->getSize( ).y ) );
        cinder::ColorA color;
        float alfa = ( count < ( 60 * 25 ) ) ? 0.7f : 0.35 + 0.35*sin( float( count ) / 2.f );
        switch ( specialsubtime )
        {
        case 0:
            color = cinder::ColorA( 1, 0.3, 0, alfa );
            break;
        case 1:
            color = cinder::ColorA( 0, 0.5f, 1.f, alfa );
            break;
        case 2:
            color = cinder::ColorA( 0, 1, 0, alfa );
            break;
        default:
            cinder::gl::color( cinder::ColorA( 1, 1, 1, 1 ) );
            break;
        }

        int countspeed = 3;
        int index = ( count / countspeed ) % CUTNUM;
        float start_x = ( index % CUTNUM )*( specialeffect->getSize( ).x / CUTNUM );
        gl::color( color );
        cinder::gl::draw( *specialeffect,
                          cinder::Area( start_x, 0, start_x + int( specialeffect->getSize( ).x / CUTNUM ), specialeffect->getSize( ).y ),
                          cinder::Rectf( -texsize / 2, texsize / 2 ) );
        gl::popModelView( );
        count++;
    }
    void Interface::drawchargeEffect( bool ismpmax )
    {
        if ( !ismpmax ) {
            return;
        }
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );
        Vec2f texsize = Vec2f( 370, 340 );
        const int CUTNUM = 10;
        gl::pushModelView( );
        gl::translate( leftDown + Vec2f( 120, -140 ) );
        gl::scale( Vec2f( texsize.x / ( specialeffect->getSize( ).x / CUTNUM ), texsize.y / specialeffect->getSize( ).y ) );
        float alfa = 0.7f;
        cinder::ColorA color = ColorA( 1, 1, 0, alfa );
        int countspeed = 3;
        int index = ( maxcount / countspeed ) % CUTNUM;
        float start_x = ( index % CUTNUM )*( specialeffect->getSize( ).x / CUTNUM );
        gl::color( color );
        cinder::gl::draw( *specialeffect,
                          cinder::Area( start_x, 0, start_x + int( specialeffect->getSize( ).x / CUTNUM ), specialeffect->getSize( ).y ),
                          cinder::Rectf( -texsize / 2, texsize / 2 ) );
        gl::popModelView( );
    }
    void Interface::drawInfo( int specialsubtime )
    {

        if ( specialsubtime == 3 ) {//何も選ばれていない
            return;
        }
        auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );
        std::string str;
        cinder::ColorA color;
        float alfa = ( count < ( 60 * 25 ) ) ? 1.f : 0.5 + 0.5*sin( float( count ) / 1.5f );
        Vec2f size = Vec2f( 225, 75 );
        Vec2f pos = leftDown + Vec2f( 320, -210 + 5 * sin( ( float( count ) ) / 10.f ) );

        gl::pushModelView( );
        makimono->enableAndBind( );
        gl::translate( pos + Vec2f( -4, 7 ) );
        gl::color( ColorA( 1, 1, 1, alfa ) );
        gl::drawSolidRect( cinder::Rectf( -size / 2, size / 2 ) );
        makimono->disable( );
        gl::popModelView( );
        switch ( specialsubtime )
        {
        case 0:
            str = u8"攻撃力アップ！";
            color = cinder::ColorA( 1, 0.3, 0, alfa );
            break;
        case 1:
            str = u8"HP徐々に回復！";
            color = cinder::ColorA( 0, 0.5f, 1.f, alfa );
            break;
        case 2:
            str = u8"一定時間無敵！";
            color = cinder::ColorA( 0, 1, 0, alfa );
            break;
        default:
            str = u8"エラー";
            break;
        }
        //color = ColorA(0, 0, 0, alfa);

        touchfont.Draw( str, pos, color, Fonts::Mode::CENTERUP );
    }
    void Interface::drawchargeInfo( bool _ismpmax )
    {
        if ( !_ismpmax ) {//何も選ばれていない
            return;
        }
        /* auto leftDown = Vec2f( 0, env.getWindowHeight( ) );
        auto rightDown = env.getWindowSize( );
        std::string str;
        cinder::ColorA color;
        float alfa = 0.7f;
        Vec2f size = Vec2f( 265, 75 );
        Vec2f pos = leftDown + Vec2f( 120, -230 + 5 * sin( ( float( maxcount ) ) / 10.f ) );
        gl::pushModelView( );
        makimono->enableAndBind( );
        gl::translate( pos + Vec2f( 0, 7 ) );
        gl::color( ColorA( 1, 1, 1, alfa ) );
        gl::drawSolidRect( cinder::Rectf( -size / 2, size / 2 ) );
        makimono->disable( );
        gl::popModelView( );*/
        //color = ColorA(0, 0, 0, alfa);

        //touchfont.Draw( u8"タッチで必殺発動", pos, ColorA( 0, 0, 0, 1 ), Fonts::Mode::CENTERUP );
    }
}