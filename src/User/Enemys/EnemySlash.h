#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <functional>

namespace User
{
    class EnemySlash : public EnemyBase
    {
        Timer timer;
        bool isAttack;
        cinder::Vec3f prevMovePosition;
        cinder::Vec3f targetVector;
        std::function<void( cinder::CameraPersp const& )> behavior;
    private:
        cinder::gl::Texture* 待機;
        cinder::gl::Texture* 攻撃モーション画像;
        cinder::gl::Texture* 攻撃画像;
        cinder::gl::Texture* 左に移動;
        cinder::gl::Texture* 右に移動;
    private:
        Audio* guard_se;
        float prevAttackHP = 0.0F;
    public:
        EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        // 重力をかけます。
        void Gravitate( );
    private:
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void カメラへ近づく( cinder::CameraPersp const& camera );
        void 攻撃モーション( cinder::CameraPersp const& camera );
        void 攻撃( cinder::CameraPersp const& camera );
        void 攻撃後硬直( cinder::CameraPersp const& camera );
        void ジャンプで戻る( cinder::CameraPersp const& camera );
        void 着地( cinder::CameraPersp const& camera );
    private:
        void 左へ移動( cinder::CameraPersp const& camera );
        void 右へ移動( cinder::CameraPersp const& camera );
    private:
        bool IsJumping( );
    private:
        void SetFunction( void ( EnemySlash::* function ) ( cinder::CameraPersp const& camera ) );
    };
}