#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <functional>

namespace User
{
    class EnemyTutorial : public EnemyBase
    {
        Timer timer;
        bool isAttack;
        cinder::Vec3f prevMovePosition;
        std::function<void( cinder::CameraPersp const& )> behavior;
    private:
        cinder::gl::Texture* 待機;
        cinder::gl::Texture* 攻撃モーション画像;
        cinder::gl::Texture* 攻撃画像;
    public:
        EnemyTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        ~EnemyTutorial( );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        // 重力をかけます。
        void Gravitate( );
    private:
        void 何もしない( cinder::CameraPersp const& camera );
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void カメラへ近づく( cinder::CameraPersp const& camera );
        void 攻撃モーション( cinder::CameraPersp const& camera );
        void 攻撃( cinder::CameraPersp const& camera );
        void 攻撃後硬直( cinder::CameraPersp const& camera );
        void ジャンプで戻る( cinder::CameraPersp const& camera );
        void 着地( cinder::CameraPersp const& camera );
    private:
        bool IsJumping( );
    private:
        void SetFunction( void ( EnemyTutorial::* function ) ( cinder::CameraPersp const& camera ) );
    };
}