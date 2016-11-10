#include "HandAnimation.h"

using namespace cinder;

HandAnimation::HandAnimation( )
{
    attackframe = 0;
    guardframe = 0;
    movet_ = 0.0f;
    attackdelay_ = 0.0f;
    isattackanimation = true;
    righthandopen = &GData::FindTexture( "ZKOO/Right_OpenHand.png" );
    righthandclose = &GData::FindTexture( "ZKOO/Right_CloseHand.png" );
    lefthandopen = &GData::FindTexture( "ZKOO/Left_OpenHand.png" );
    lefthandclose = &GData::FindTexture( "ZKOO/Left_CloseHand.png" );

    attackhand = nullptr;
    leftguardhand = nullptr;
    rightguardhand = nullptr;
}



void HandAnimation::updateAttack( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size )
{
    selectAttackTexture( );
    moveAttackPos( startpos, endpos, size );
    attackReset( );
    attackframe++;
}

void HandAnimation::updateGuard( cinder::Vec2f leftpos, cinder::Vec2f rightpos, cinder::Vec2f size )
{
    selectGuardTexture( );
    guard_left_pos = leftpos;
    guard_right_pos = rightpos;
    handsize = size;
    guardframe++;
}



void HandAnimation::drawAttack( )
{
    if ( !attackhand ) return;

    gl::pushModelView( );
    gl::translate( attack_hand_pos );
    attackhand->enableAndBind( );
    gl::color( ColorA( 1, 1, 1, 1 ) );
    gl::drawSolidRect( Rectf( -handsize / 2, handsize / 2 ) );
    attackhand->disable( );
    gl::popModelView( );
}

void HandAnimation::drawGuard( )
{
    if ( !leftguardhand || !rightguardhand ) return;

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

void HandAnimation::Reset( )
{
    attackframe = 0;
    movet_ = 0.0f;
    attackdelay_ = 0.0f;
    guardframe = 0;
}

void HandAnimation::selectGuardTexture( )
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

void HandAnimation::selectAttackTexture( )
{
    const int opentime = 40;///初期から４０フレーㇺ後に握る画像に
    if ( attackframe < opentime ) {
        attackhand = righthandopen;
    }
    else {
        attackhand = righthandclose;
    }
}

void HandAnimation::moveAttackPos( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size )
{
    handsize = size;
    attack_hand_pos.x = Easing::getEasing[Easing::Linear]( movet_, startpos.x, endpos.x );
    attack_hand_pos.y = Easing::getEasing[Easing::Linear]( movet_, startpos.y, endpos.y );
    const int movestartframe = 80;////初期から８０フレーㇺで動き出す
    if ( attackframe > movestartframe )
        Easing::tCount( movet_, 1.0f );////１秒で視点から終点へ
}


void HandAnimation::attackReset( )
{
    if ( movet_ >= 1.0f )
        Easing::tCount( attackdelay_, 0.5f );///ムーブが終わった後０．５秒でリセット

    if ( attackdelay_ >= 1.0f ) {
        attackframe = 0;
        movet_ = 0.0f;
        attackdelay_ = 0.0f;
    }
}
