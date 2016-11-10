#pragma once

#include "EnemyBase.h"

namespace User
{
    class EnemySlashNot : public EnemyBase
    {
    public:
        EnemySlashNot( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera )override;
    };
}