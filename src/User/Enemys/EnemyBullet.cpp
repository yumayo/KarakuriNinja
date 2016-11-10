# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, "EnemyBullet.png" )
    {
        SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
    }
    void EnemyBullet::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // ���C������
            behavior( camera );

            // �^�C�}�[�X�V
            timer.Update( );
        }

        EnemyBase::update( camera );
    }
    void EnemyBullet::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyBullet::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    void EnemyBullet::�^�C�}�[����܂őҋ@( cinder::CameraPersp const & camera )
    {
        // �s�����ԂɂȂ�����e�𓊂��܂��B
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemyBullet::�e�𓊂��� );
        }
    }
    void EnemyBullet::�e�𓊂���( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( 0 + 50, env.getWindowWidth( ) - 50 ) / env.getWindowWidth( );
        auto v = randFloat( 0 + 50, env.getWindowHeight( ) - 50 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken2.png" ) );

        // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
        timer.Advance( randInt( 120, 300 ) );
        SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}