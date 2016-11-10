
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"GlobalData.hpp"
# include <memory>

class HandAnimation {
public:
    HandAnimation( );
    void updateAttack( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size );
    void updateGuard( cinder::Vec2f leftpos, cinder::Vec2f rightpos, cinder::Vec2f size );
    void drawAttack( );
    void drawGuard( );
    ///ƒJƒEƒ“ƒgŒn‚ğ‚·‚×‚Ä‰Šú‰»
    void Reset( );
private:
    cinder::Vec2f attack_hand_pos;
    cinder::Vec2f guard_right_pos;
    cinder::Vec2f guard_left_pos;
    cinder::Vec2f handsize;
    cinder::gl::Texture* righthandopen;
    cinder::gl::Texture* righthandclose;
    cinder::gl::Texture* lefthandopen;
    cinder::gl::Texture* lefthandclose;
    cinder::gl::Texture* attackhand;
    cinder::gl::Texture* rightguardhand;
    cinder::gl::Texture* leftguardhand;
    int attackframe;
    int guardframe;
    float movet_;
    float attackdelay_;
    bool isattackanimation;
    void selectGuardTexture( );
    void selectAttackTexture( );
    void moveAttackPos( cinder::Vec2f startpos, cinder::Vec2f endpos, cinder::Vec2f size );
    void attackReset( );
};

using HandAnimationRef = std::shared_ptr<HandAnimation>;