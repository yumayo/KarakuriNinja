# include "EnemyBulletTutorial.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;
    EnemyBulletTutorial::EnemyBulletTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 80.0F, 0 ) )
    {
        timer.Advance( 60 );

        int index = 1;
        �ҋ@ = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        �U�����[�V�����摜 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        �U���摜 = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );
        index++;
        index++;

        texture = �ҋ@;
        knockBackTexture = &GData::FindTexture( "Enemy/Bullet/Bullet (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemyBulletTutorial::�^�C�}�[����܂őҋ@ );
        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
        }
    }
    void EnemyBulletTutorial::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // �m�b�N�o�b�N���͈ꎞ�I�ɏ��������܂���B
            if ( !IsKnockBack( ) )
            {
                // ���C������
                behavior( camera );

                // �^�C�}�[����
                timer.Update( );
            }
        }
        else
        {
            texture = knockBackTexture;
        }

        EnemyBase::update( camera );
    }
    void EnemyBulletTutorial::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyBulletTutorial::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    void EnemyBulletTutorial::�^�C�}�[����܂őҋ@( cinder::CameraPersp const & camera )
    {
        // �s�����ԂɂȂ�����e�𓊂��܂��B
        if ( timer.IsAction( ) )
        {
            texture = �U�����[�V�����摜;
            timer.Advance( 60 );
            SetFunction( &EnemyBulletTutorial::�U�����[�V���� );
        }
    }
    void EnemyBulletTutorial::�U�����[�V����( cinder::CameraPersp const & camera )
    {
        // ���[�V��������60�t���[���̌�A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            texture = �U���摜;
            SetFunction( &EnemyBulletTutorial::�U�� );
            return;
        }
    }
    void EnemyBulletTutorial::�U��( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        auto v = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png" ) );
        EffectCreate( EffectBase( "Textures/Effect/effect4.png",
                                  camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) ),
                                  camera.worldToEyeDepth( object.Position( ) ),
                                  Vec2f( 240, 240 ),
                                  Vec2f( 240, 240 ),
                                  EffectBase::Mode::CENTERCENTER
        ) );

        // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
        timer.Advance( 10 );
        SetFunction( &EnemyBulletTutorial::�U����d�� );
        se[randInt( 3 )]->Play( );
    }
    void EnemyBulletTutorial::�U����d��( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( 90 );
            texture = �ҋ@;
            SetFunction( &EnemyBulletTutorial::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBulletTutorial::SetFunction( void( EnemyBulletTutorial::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}