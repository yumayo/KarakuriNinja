# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName )
        : EnemyBase( pos, camera, 1.0F )
        , �ҋ@( &GData::FindTexture( fieldName + "/EnemyBullet (1).png" ) )
        , �U�����[�V�����摜( &GData::FindTexture( fieldName + "/EnemyBullet (2).png" ) )
        , �U���摜( &GData::FindTexture( fieldName + "/EnemyBullet (3).png" ) )
        , ���Ɉړ�( &GData::FindTexture( fieldName + "/EnemyBullet (4).png" ) )
        , �E�Ɉړ�( &GData::FindTexture( fieldName + "/EnemyBullet (5).png" ) )
    {
        texture = �ҋ@;

        SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        for ( int i = 0; i < 3; i++ ) {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
        }
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
            if ( randInt( 0, 3 ) != 0 )
            {
                texture = �U�����[�V�����摜;
                timer.Advance( 60 );
                SetFunction( &EnemyBullet::�U�����[�V���� );
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    texture = ���Ɉړ�;
                    SetFunction( &EnemyBullet::���ֈړ� );
                }
                else
                {
                    texture = �E�Ɉړ�;
                    SetFunction( &EnemyBullet::�E�ֈړ� );
                }
            }
        }
    }
    void EnemyBullet::�U�����[�V����( cinder::CameraPersp const & camera )
    {
        // ���[�V��������60�t���[���̌�A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            texture = �U���摜;
            SetFunction( &EnemyBullet::�U�� );
            return;
        }
    }
    void EnemyBullet::�U��( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        auto v = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken3.png" ) );
        EffectCreate( EffectBase( "Textures/Effect/effect4.png",
                                  camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) ),
                                  Vec2f( 240, 240 ),
                                  Vec2f( 240, 240 ),
                                  EffectBase::Mode::CENTERCENTER
        ) );

        // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
        timer.Advance( 10 );
        SetFunction( &EnemyBullet::�U����d�� );
        se[randInt( 3 )]->Play( );


    }
    void EnemyBullet::�U����d��( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 120, 300 ) );
            texture = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBullet::���ֈړ�( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 20, 30 ) );
            texture = �E�Ɉړ�;
            SetFunction( &EnemyBullet::�E�ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBullet::�E�ֈړ�( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 20, 30 ) );
            texture = ���Ɉړ�;
            SetFunction( &EnemyBullet::���ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}