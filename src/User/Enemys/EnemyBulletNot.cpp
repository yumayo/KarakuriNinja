# include "EnemyBulletNot.h"

namespace User
{
    using namespace cinder;

    EnemyBulletNot::EnemyBulletNot( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        int index = 0;
        texture = &GData::FindTexture( "Enemy/BulletNot/BulletNot (1).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/BulletNot/BulletNot (6).png" );
    }

    void EnemyBulletNot::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {

        }
        else
        {
            texture = knockBackTexture;
        }

        EnemyBase::update( camera );
    }
}