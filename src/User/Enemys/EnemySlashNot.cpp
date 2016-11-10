# include "EnemySlashNot.h"

namespace User
{
    using namespace cinder;

    EnemySlashNot::EnemySlashNot( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        int index = 0;
        texture = &GData::FindTexture( "Enemy/SlashNot/SlashNot (1).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/SlashNot/SlashNot (6).png" );
    }

    void EnemySlashNot::update( cinder::CameraPersp const& camera )
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