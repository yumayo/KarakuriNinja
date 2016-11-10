#pragma once

#include "EnemyBase.h"

namespace User
{
    class EnemyBulletWalk : public EnemyBase
    {
        Timer timer;
        cinder::gl::Texture* �ҋ@;
        cinder::gl::Texture* ���Ɉړ�;
        cinder::gl::Texture* �E�Ɉړ�;
        std::function<void( cinder::CameraPersp const& )> behavior;
    public:
        EnemyBulletWalk( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera )override;
    private:
        void �^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera );
        void ���ֈړ�( cinder::CameraPersp const& camera );
        void �E�ֈړ�( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBulletWalk::* function ) ( cinder::CameraPersp const& camera ) );
    };
}