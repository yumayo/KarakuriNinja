#pragma once

#include "EnemyBase.h"

namespace User
{
    class EnemyBulletNot : public EnemyBase
    {
    public:
        EnemyBulletNot( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera )override;
    };
}