#pragma once

#include "EnemyBase.h"
#include "cinder/gl/Texture.h"

namespace User
{
    class BulletEnemy : public EnemyBase
    {
        int time;
        int nextJumpTime;
        cinder::gl::Texture texture;
    public:
        BulletEnemy( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        bool IsAction( );
        bool IsThink( );
        void Thinking( );
        void Advance( );
    };
}