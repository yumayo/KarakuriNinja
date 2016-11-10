# include "EnemyBoss.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBoss::EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 20.0F, 7 ) )
        , timer( )
        , isAttack( false )
        , isDeadStop( true )
        , isHalfHPSerif( true )
        , isDeadSerif( true )
        , font( u8"���C���I", 74 )
        , serif( u8"" )
        , serifDrawPosition( Vec2f::zero( ) )
    {
        int index = 1;
        �ҋ@ = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �����郂�[�V�����摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        ������摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �U�����[�V�����摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �U���摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        ���Ɉړ� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �E�Ɉړ� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �|�ꂩ���郂�[�V���� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        �|��郂�[�V���� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );

        texture = �ҋ@;

        timer.Advance( 180 ); // �Z���t��f���t���[��
        SetFunction( &EnemyBoss::�o���������̃Z���t );
        SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );

        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
            �e�J�E���g = 0;
        }

    }
    void EnemyBoss::update( cinder::CameraPersp const& camera )
    {
        if ( isDeadStop )
        {
            // �d�͏���
            Gravitate( );
            // ���C������
            behavior( camera );
            // �^�C�}�[����
            timer.Update( );

        }

        // �ȉ� EnemyBaseUpdate
        CameraSee( camera );

        if ( isDeadStop )
        {
            object.PositionAdd( object.Speed( ) );
            DamageEffect( );
            LiveCheck( );
            attackTime.Update( );
        }
        CollideGround( ); // ����ł��Ă����s���܂��B
        //CollideField( );// ����ł��Ă����s���܂��B
        if ( isDeadStop == false ) // �����ŁA���ʂƔ��f����邱�Ƃ�x�点�Ă��܂��B
            Dying( );

        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Kill( );
    }
    void EnemyBoss::draw( )
    {
        EnemyBase::draw( );
    }
    void EnemyBoss::drawUI( const cinder::CameraPersp& camera )
    {
        EnemyBase::drawUI( camera );
        serifBehavior( );
    }
    bool EnemyBoss::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && isDeadStop;
    }
    void EnemyBoss::Gravitate( )
    {
        //�@�W�����v���Ȃ�d�͂������܂��B
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyBoss::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        if ( IsHalfHPSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );

            timer.Advance( 120 ); // �Z���t��f���t���[��
            SetFunction( &EnemyBoss::HP�������ȉ��ɂȂ������̃Z���t );
            return;
        }
        if ( IsDeadSerif( ) )
        {
            texture = �|�ꂩ���郂�[�V����;

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 120 ); // �Z���t��f���t���[��
            SetFunction( &EnemyBoss::���ʎ��̃Z���t );
            return;
        }

        // �^�C�}�[�������玟�̊֐��ցB
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemyBoss::�ǂ����̍U�����o�����̊m�F );
            return;
        }
    }
    void EnemyBoss::�ǂ����̍U�����o�����̊m�F( cinder::CameraPersp const & camera )
    {
        auto �ǂ���̍U�� = randBool( );
        if ( �ǂ���̍U�� && ( ���ڍU���J�E���g < ���ڍU���̏���� ) )
        {
            �e�J�E���g = 0;

            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;

            auto pos = object.Position( );
            pos.y = 0;

            moveSpeed = cameraPos - pos;
            moveSpeed /= 60.0F;

            moveLeftRightSpeed = Vec3f::xAxis( ) * 0.5F;

            timer.Advance( 60 ); // �����ړ��t���[��

            ���ڍU���J�E���g++;
            SetFunction( &EnemyBoss::���E�ɍ����ړ����Ȃ���J�����֋߂Â� );
            return;
        }
        else if ( �e�J�E���g < �e�̏���� )
        {
            ���ڍU���J�E���g = 0;

            timer.Advance( 120 ); // �e�𓊂��铮��
            texture = �����郂�[�V�����摜;

            �e�J�E���g++;
            SetFunction( &EnemyBoss::�e�𓊂���܂ł̃��[�V���� );
            return;
        }
        else // �ǂ���Ƃ��o���Ȃ��ꍇ
        {
            �e�J�E���g = 0;
            ���ڍU���J�E���g = 0;
            �ǂ����̍U�����o�����̊m�F( camera );
        }
    }
    void EnemyBoss::���E�ɍ����ړ����Ȃ���J�����֋߂Â�( cinder::CameraPersp const& camera )
    {
        if ( !IsInTheScreen( camera ) )
        {
            moveLeftRightSpeed *= -1;
            object.PositionAdd( moveLeftRightSpeed );
        }

        auto cameraRightVec = camera.getViewDirection( );
        cameraRightVec.rotate( camera.getWorldUp( ), -M_PI / 2.0 );
        if ( moveLeftRightSpeed.dot( cameraRightVec ) < 0 )
        {
            texture = �E�Ɉړ�;
        }
        else
        {
            texture = ���Ɉړ�;
        }

        object.Speed( moveSpeed + moveLeftRightSpeed );

        if ( timer.IsAction( ) )
        {
            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;
            object.Position( cameraPos );

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 80 ); // �U�����[�V�����t���[������
            attackTime.AttackFrame( 80 );

            texture = �U�����[�V�����摜;
            SetFunction( &EnemyBoss::�U�����[�V���� );
            return;
        }
    }
    void EnemyBoss::�U�����[�V����( cinder::CameraPersp const& camera )
    {
        // �U�����[�V������A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            texture = �U���摜;
            SetFunction( &EnemyBoss::�U�� );
            return;
        }
    }
    void EnemyBoss::�U��( cinder::CameraPersp const& camera )
    {
        // �U�����Ă������̊֐��ցB
        isAttack = true;

        timer.Advance( 20 ); // �d���t���[��
        SetFunction( &EnemyBoss::�U����d�� );
        return;
    }
    void EnemyBoss::�U����d��( cinder::CameraPersp const& camera )
    {
        // �U���t���O���I�t�ɁA�d������10�t���[���̌�A���̊֐��ցB
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            texture = �ҋ@;
            SetFunction( &EnemyBoss::�W�����v�Ŗ߂� );
            return;
        }
    }
    void EnemyBoss::�W�����v�Ŗ߂�( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 4.0F, M_PI / 4.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // �d����A�����ɃW�����v���Ď��̊֐��ցB
        Jump( jumpPower + backDirection );

        SetFunction( &EnemyBoss::���n );
        return;
    }
    void EnemyBoss::���n( cinder::CameraPersp const& camera )
    {
        // �n�ʂɒ�������A���̊֐��ցB
        if ( isLanding )
        {
            // �X�s�[�h��0�ɂ��܂��B�������Ⴄ�̂ŁB
            object.Speed( Vec3f::zero( ) );
            // ���̍U���܂ł̃t���[�������B
            timer.Advance( randInt( 60, 120 ) );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::����( cinder::CameraPersp const & camera )
    {
        isDeadStop = false;
    }
    void EnemyBoss::�e�𓊂���܂ł̃��[�V����( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            texture = ������摜;
            SetFunction( &EnemyBoss::�e�𓊂��� );
        }
    }
    void EnemyBoss::�e�𓊂���( cinder::CameraPersp const & camera )
    {
        //WavyBulletFiring( randFloat( 0, M_PI * 2 ), camera );
        RandomWavyBulletFiring( camera );
        for ( int i = 0; i < 3; i++ )
        {
            se[i]->Play( );
        }
        timer.Advance( 20 ); // �d���t���[��

        �e�𓊂����++;
        SetFunction( &EnemyBoss::�e�𓊂�����̍d�� );
        return;
    }
    void EnemyBoss::�e�𓊂�����̍d��( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            timer.Advance( 120 );

            SetFunction( &EnemyBoss::�e��������x�����邩�̊m�F );
            return;
        }
    }
    void EnemyBoss::�e��������x�����邩�̊m�F( cinder::CameraPersp const & camera )
    {
        if ( �e�𓊂���� < �e�𓊂������� )
        {
            texture = �����郂�[�V�����摜;
            if ( timer.IsAction( ) )
            {
                SetFunction( &EnemyBoss::�e�𓊂���܂ł̃��[�V���� );
                return;
            }
        }
        else
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 180, 240 ) );

            texture = �ҋ@;

            �e�𓊂���� = 0;
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::�o���������̃Z���t( cinder::CameraPersp const & camera )
    {
        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"�����͂��́A���p���M�l", &EnemyBoss::�Z���t );

        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::HP�������ȉ��ɂȂ������̃Z���t( cinder::CameraPersp const & camera )
    {
        isHalfHPSerif = false;

        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"�Ȃ��Ȃ����ȋM�l", &EnemyBoss::�Z���t );

        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::���ʎ��̃Z���t( cinder::CameraPersp const & camera )
    {
        isDeadSerif = false;

        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"�����A��͖łт�", &EnemyBoss::�Z���t );

        if ( timer.IsAction( ) )
        {
            texture = �|��郂�[�V����;
            timer.Advance( 60 );
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::���� );
        }
    }
    void EnemyBoss::�k���Z���t( )
    {
        /*nothing*/
    }
    void EnemyBoss::�Z���t( )
    {
        gl::pushModelView( );
        gl::translate( serifDrawPosition );
        gl::color( Color::white( ) );
        auto size = font.BoundingBox( serif ).getSize( );
        gl::drawSolidRect( Rectf( -size.x / 2.0F, 0, size.x / 2.0F, size.y ) );
        font.Draw( serif, Vec2f::zero( ), Color::black( ), Fonts::Mode::CENTERUP );
        gl::popModelView( );
    }
    bool EnemyBoss::IsHalfHPSerif( )
    {
        return isHalfHPSerif && NormalizedHitPoint( ) <= 0.5F;
    }
    bool EnemyBoss::IsDeadSerif( )
    {
        return isDeadSerif && IsLive( ) == false;
    }
    void EnemyBoss::SetFunction( void( EnemyBoss::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
    void EnemyBoss::SetSerifFunction( std::string const& serif, void( EnemyBoss::* function )( ) )
    {
        this->serif = serif;
        serifBehavior = std::bind( function, this );
    }
    void EnemyBoss::WavyBulletFiring( float radians, cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        auto direction = Vec2f::xAxis( );
        direction.rotate( radians );

        float length = std::min( env.getWindowWidth( ), env.getWindowHeight( ) );
        length /= 2.0F;
        length -= 50;

        {
            float u = 0.5F;
            float v = 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * -length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * -length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }
    }
    void EnemyBoss::RandomWavyBulletFiring( cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        Vec2f pos1, pos2, pos3;

        pos1.x = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.4 ) / env.getWindowWidth( );
        pos1.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( pos1.x, pos1.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos2.x = randFloat( env.getWindowWidth( )*0.6, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        pos2.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        ray = camera.generateRay( pos2.x, pos2.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos3.x = ( pos1.x + pos2.x ) / 2.0F;
        pos3.y = ( pos1.y + pos2.y ) / 2.0F;
        ray = camera.generateRay( pos3.x, pos3.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
    }
}