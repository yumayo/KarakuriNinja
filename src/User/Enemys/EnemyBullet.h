#pragma once

#include "EnemyBase.h"

# include "../Utilitys/Yumayo.h"
#include"../Utilitys/Audio.h"
namespace User
{
    class EnemyBullet : public EnemyBase
    {
        Timer timer;
        std::function<void( cinder::CameraPersp const& camera )> behavior;
    private:
        cinder::gl::Texture* 待機;
        cinder::gl::Texture* 攻撃モーション画像;
        cinder::gl::Texture* 攻撃画像;
        cinder::gl::Texture* 左に移動;
        cinder::gl::Texture* 右に移動;
    public:
        EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void 攻撃モーション( cinder::CameraPersp const& camera );
        void 攻撃( cinder::CameraPersp const& camera );
        void 攻撃後硬直( cinder::CameraPersp const& camera );
        void 左へ移動( cinder::CameraPersp const& camera );
        void 右へ移動( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBullet::* function ) ( cinder::CameraPersp const& camera ) );
        std::vector<Audio*> se;
    };
}