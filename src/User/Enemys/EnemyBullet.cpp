# include "EnemyBullet.h"

# include "Framework.hpp"
# include "cinder/Rand.h"

# include "EnemyBulletTexture.h"

namespace User
{
    using namespace cinder;
    EnemyBullet::EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName )
        : EnemyBase( pos, camera )
        , �ҋ@( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (1).png" ) ) ) )
        , �U�����[�V�����摜( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (2).png" ) ) ) )
        , �U���摜( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (3).png" ) ) ) )
        , ���Ɉړ�( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (4).png" ) ) ) )
        , �E�Ɉړ�( std::make_shared<gl::Texture>( loadImage( app::loadAsset( fieldName + "/EnemyBullet (5).png" ) ) ) )
    {
        textureRef = �ҋ@;

        SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        for ( int i = 0; i < 3; i++ ) {
            se.push_back( Audio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
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
                textureRef = �U�����[�V�����摜;
                timer.Advance( 60 );
                SetFunction( &EnemyBullet::�U�����[�V���� );
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    textureRef = ���Ɉړ�;
                    SetFunction( &EnemyBullet::���ֈړ� );
                }
                else
                {
                    textureRef = �E�Ɉړ�;
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
            textureRef = �U���摜;
            SetFunction( &EnemyBullet::�U�� );
            return;
        }
    }
    void EnemyBullet::�U��( cinder::CameraPersp const & camera )
    {
        auto u = randFloat( 0 + 100, env.getWindowWidth( ) - 100 ) / env.getWindowWidth( );
        auto v = randFloat( 0 + 100, env.getWindowHeight( ) - 300 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken2.png" ) );

        // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
        timer.Advance( 10 );
        SetFunction( &EnemyBullet::�U����d�� );
        se[randInt( 3 )].Play( );
    }
    void EnemyBullet::�U����d��( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 120, 300 ) );
            textureRef = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBullet::���ֈړ�( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInTheScreen( camera ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 20, 30 ) );
            textureRef = �E�Ɉړ�;
            SetFunction( &EnemyBullet::�E�ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            textureRef = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBullet::�E�ֈړ�( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInTheScreen( camera ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 20, 30 ) );
            textureRef = ���Ɉړ�;
            SetFunction( &EnemyBullet::���ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            textureRef = �ҋ@;
            SetFunction( &EnemyBullet::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBullet::SetFunction( void( EnemyBullet::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}