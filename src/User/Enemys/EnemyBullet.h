#pragma once

#include "EnemyBase.h"

# include "../Utilitys/Yumayo.h"

namespace User
{
    class EnemyBullet : public EnemyBase
    {
        Timer timer;
        std::function<void( cinder::CameraPersp const& camera )> behavior;
    public:
        EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void 弾を投げる( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBullet::* function ) ( cinder::CameraPersp const& camera ) );
    };
}