# include "EnemySlash.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    EnemySlash::EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, "EnemySlash.png" )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        SetFunction( &EnemySlash::�^�C�}�[����܂őҋ@ );
    }
    void EnemySlash::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // �d�͏���
            Gravitate( );

            // ���C������
            behavior( camera );

            // �^�C�}�[����
            timer.Update( );
        }

        EnemyBase::update( camera );
    }
    void EnemySlash::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemySlash::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack;
    }
    void EnemySlash::Gravitate( )
    {
        //�@�W�����v���Ȃ�d�͂������܂��B
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemySlash::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        // �^�C�}�[�������玟�̊֐��ցB
        if ( timer.IsAction( ) )
        {
            prevMovePosition = object.Position( );
            timer.Advance( 120 );
            SetFunction( &EnemySlash::�J�����֋߂Â� );
            return;
        }
    }
    void EnemySlash::�J�����֋߂Â�( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, cameraPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, cameraPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // �U�����[�V�����t���[������
            SetFunction( &EnemySlash::�U�����[�V���� );
            return;
        }
    }
    void EnemySlash::�U�����[�V����( cinder::CameraPersp const& camera )
    {
        // ���[�V��������60�t���[���̌�A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemySlash::�U�� );
            return;
        }
    }
    void EnemySlash::�U��( cinder::CameraPersp const& camera )
    {
        // �U�����Ă������̊֐��ցB
        isAttack = true;
        timer.Advance( 10 ); // �d��10�t���[��
        SetFunction( &EnemySlash::�U����d�� );
        return;
    }
    void EnemySlash::�U����d��( cinder::CameraPersp const& camera )
    {
        // �U���t���O���I�t�ɁA�d������10�t���[���̌�A���̊֐��ցB
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemySlash::�W�����v�Ŗ߂� );
            return;
        }
    }
    void EnemySlash::�W�����v�Ŗ߂�( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 4.0F, M_PI / 4.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // �d����A�����ɃW�����v���Ď��̊֐��ցB
        Jump( jumpPower + backDirection );

        SetFunction( &EnemySlash::���n );
        return;
    }
    void EnemySlash::���n( cinder::CameraPersp const& camera )
    {
        // �n�ʂɒ�������A���̊֐��ցB
        if ( isLanding )
        {
            // �X�s�[�h��0�ɂ��܂��B�������Ⴄ�̂ŁB
            object.Speed( Vec3f::zero( ) );
            // ���̍U���܂ł̃t���[�������B
            timer.Advance( randInt( 60, 240 ) );
            SetFunction( &EnemySlash::�^�C�}�[����܂őҋ@ );
            return;
        }
    }

    bool EnemySlash::IsJumping( )
    {
        return isLanding == false;
    }
    void EnemySlash::SetFunction( void( EnemySlash::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}