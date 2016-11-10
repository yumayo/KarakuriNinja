#include"HundAnimation.h"

using namespace cinder;

HundAnimation::HundAnimation( )
{
    attackframe = 0;
    guardframe = 0;
    attackangle = 0.0f;
    guardangle = 0.0f;
    movet_ = 0.0f;
    attackdelay_ = 0.0f;
    isattackanimation = true;
    righthandopen = &GData::FindTexture( "ZKOO/Right_OpenHand.png" );
    righthandclose = &GData::FindTexture( "ZKOO/Right_CloseHand.png" );
    lefthandopen = &GData::FindTexture( "ZKOO/Left_OpenHand.png" );
    lefthandclose = &GData::FindTexture( "ZKOO/Left_CloseHand.png" );
    yazirushi = &GData::FindTexture( "Textures/yazirushi.png" );
    guardyazirushi = &GData::FindTexture( "Textures/guardyazirushi.png" );
    attackhand = righthandopen;
    leftguardhand = lefthandopen;
    rightguardhand = righthandopen;

    updateAttack( Vec2f::one( ), Vec2f::one( ), Vec2f( 256, 256 ) );
    updateGuard( Vec2f::one( ), Vec2f::one( ), Vec2f( 256, 256 ) );
}



void HundAnimation::updateAttack( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size )
{
    selectAttackTexture( );
    moveAttackPos( startpos, endpos, size );
    setcenterpos( ( startpos + endpos ) / 2.f, attackcenterpos );
    attackangle = atan2f( endpos.y - startpos.y, endpos.x - startpos.x )*( 360.f / ( 2.f * M_PI ) );
    attackReset( );
    attackframe++;
}

void HundAnimation::updateGuard( cinder::Vec2f leftpos, cinder::Vec2f rightpos, cinder::Vec2f size )
{
    selectGuardTexture( );
    guard_left_pos = leftpos;
    guard_right_pos = rightpos;
    guardangle = atan2f( leftpos.y - rightpos.y, leftpos.x - rightpos.x )*( 360.f / ( 2.f * M_PI ) );
    setcenterpos( ( leftpos + rightpos ) / 2.f, guardcenterpos );
    handsize = size;
    guardframe++;
}



void HundAnimation::drawAttack( cinder::Vec2f linesize )
{
    gl::pushModelView( );
    gl::translate( attackcenterpos );
    gl::rotate( attackangle );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    yazirushi->enableAndBind( );
    gl::drawSolidRect( Rectf( -linesize / 2, linesize / 2 ) );
    yazirushi->disable( );
    gl::popModelView( );

    gl::pushModelView( );
    gl::translate( attack_hund_pos );
    attackhand->enableAndBind( );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    gl::drawSolidRect( Rectf( -handsize / 2, handsize / 2 ) );
    attackhand->disable( );
    gl::popModelView( );
}

void HundAnimation::drawGuard( cinder::Vec2f linesize )
{
    gl::pushModelView( );
    gl::translate( guardcenterpos );
    gl::rotate( guardangle );
    gl::scale( linesize );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    guardyazirushi->enableAndBind( );
    gl::drawSolidRect( Rectf( -Vec2f( 0.5f, 0.5f ), Vec2f( 0.5f, 0.5f ) ) );
    guardyazirushi->disable( );
    gl::popModelView( );


    gl::pushModelView( );
    gl::translate( guard_left_pos );
    leftguardhand->enableAndBind( );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    gl::drawSolidRect( Rectf( -handsize / 2, handsize / 2 ) );
    leftguardhand->disable( );
    gl::popModelView( );

    gl::pushModelView( );
    gl::translate( guard_right_pos );
    rightguardhand->enableAndBind( );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    gl::drawSolidRect( Rectf( -handsize / 2, handsize / 2 ) );
    rightguardhand->disable( );
    gl::popModelView( );
}

void HundAnimation::Reset( )
{
    attackframe = 0;
    movet_ = 0.0f;
    attackdelay_ = 0.0f;
    guardframe = 0;
}

void HundAnimation::selectGuardTexture( )
{
    /////３秒のうち４０フレームだけ開く画像、それ以外は握る
    if ( guardframe % ( 60 * 5 ) < 40 ) {
        rightguardhand = righthandopen;
        leftguardhand = lefthandopen;
    }
    else {
        rightguardhand = righthandclose;
        leftguardhand = lefthandclose;
    }
}

void HundAnimation::selectAttackTexture( )
{
    attackhand = righthandopen;
    const int opentime = 40;///初期から４０フレーㇺ後に握る画像に
    if ( attackframe < opentime ) {
        attackhand = righthandopen;
    }
    else {
        attackhand = righthandclose;
    }
}

void HundAnimation::moveAttackPos( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size )
{
    handsize = size;
    attack_hund_pos.x = Easing::getEasing[Easing::Linear]( movet_, startpos.x, endpos.x );
    attack_hund_pos.y = Easing::getEasing[Easing::Linear]( movet_, startpos.y, endpos.y );
    const int movestartframe = 80;////初期から８０フレーㇺで動き出す
    if ( attackframe > movestartframe )
        Easing::tCount( movet_, 1.0f );////１秒で視点から終点へ
}


void HundAnimation::attackReset( )
{
    if ( movet_ >= 1.0f )
        Easing::tCount( attackdelay_, 0.5f );///ムーブが終わった後０．５秒でリセット

    if ( attackdelay_ >= 1.0f ) {
        attackframe = 0;
        movet_ = 0.0f;
        attackdelay_ = 0.0f;
    }
}

void HundAnimation::setcenterpos( cinder::Vec2f centerpos, cinder::Vec2f & pos )
{
    pos = centerpos;
}

