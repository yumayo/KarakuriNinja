# pragma once

# include "Scene.hpp"
# include "../Assets/Assets.h"
#include "../SlashEffect/SlashEffect.h"
#include "../Deffence/Deffence.h"
#include "../Attack/AttackFactory.h"

namespace User
{
    class SceneTitle : public SceneBase
    {
    public:
        SceneTitle( );
        ~SceneTitle( );
    public:
        void resize( );
        void update( );
        void draw( );
        void select( );
    public:
        void UpdateLogoAlpha( );
        //今はTouchでやっています
        //後でZkooに変えます
        void Touch( );
        void SetAttackMotionOfTouch( cinder::app::TouchEvent::Touch touch, uint32_t id );
        void MakeAttackMotionOfTouch( cinder::app::TouchEvent::Touch touch, uint32_t id );

        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );

        

        
    private:
        Slash slashEffect;
        AttackFactory attackTask;
        Line line;
        bool isAttack;

        //スタートボタンのLogoのAlpha値です
        float logoAlpha;
        //スタートボタンのLogoのAlpha移動のSpeed値です
        float logoAlphaSpeed;

        Izanami::Holder<ci::gl::Texture> textures;
        

        bool isEnd;
    };
}