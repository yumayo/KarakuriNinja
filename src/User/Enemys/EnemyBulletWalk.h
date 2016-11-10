#pragma once

#include "EnemyBase.h"

namespace User
{
    class EnemyBulletWalk : public EnemyBase
    {
        Timer timer;
        cinder::gl::Texture* 待機;
        cinder::gl::Texture* 左に移動;
        cinder::gl::Texture* 右に移動;
        std::function<void( cinder::CameraPersp const& )> behavior;
    public:
        EnemyBulletWalk( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera )override;
    private:
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void 左へ移動( cinder::CameraPersp const& camera );
        void 右へ移動( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBulletWalk::* function ) ( cinder::CameraPersp const& camera ) );
    };
}