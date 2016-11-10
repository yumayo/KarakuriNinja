# include "EnemyTutorial.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "GlobalData.hpp"

# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    EnemyTutorial::EnemyTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 6.5F, 3 ) )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        TRData::Reset( );

        int index = 1;
        �ҋ@ = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �U�����[�V�����摜 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �U���摜 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        ���Ɉړ� = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �E�Ɉړ� = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        texture = �ҋ@;
        knockBackTexture = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemyTutorial::�^�C�}�[����܂őҋ@ );
    }
    EnemyTutorial::~EnemyTutorial( )
    {
        if ( !TRData::enemyKill.IsComplete( ) )
        {
            TRData::enemyKill.TutorialStart( );
        }
    }
    void EnemyTutorial::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // �d�͏���
            Gravitate( );

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

        // EnemyBase::update�������Ƃ̏����ł���K�v������܂��B
        // �^�C�}�[�n�̍X�V���K�v�Ȃ��߁B
        if ( IsAttackMotion( ) )
        {
            if ( !TRData::attackCircle.IsComplete( ) )
            {
                TRData::attackCircle.TutorialStart( );
            }
        }

        if ( IsAttackOneFramePrev( ) )
        {
            if ( !TRData::guard.IsComplete( ) )
            {
                TRData::guard.TutorialStart( );
            }
        }
    }
    void EnemyTutorial::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemyTutorial::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && IsLive( );
    }
    void EnemyTutorial::Gravitate( )
    {
        //�@�W�����v���Ȃ�d�͂������܂��B
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyTutorial::�������Ȃ�( cinder::CameraPersp const & camera )
    {

    }
    void EnemyTutorial::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        // �G�l�~�[���o���������Ƀ`���[�g���A�����Ă�
        if ( !TRData::spawn.IsComplete( ) )
        {
            TRData::spawn.TutorialStart( );
        }

        prevMovePosition = object.Position( );
        timer.Advance( 120 );
        SetFunction( &EnemyTutorial::�J�����֋߂Â� );
        return;
    }
    void EnemyTutorial::�J�����֋߂Â�( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, cameraPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, cameraPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // �U�����[�V�����t���[������
            attackTime.AttackFrame( 60 );
            texture = �U�����[�V�����摜;
            SetFunction( &EnemyTutorial::�U�����[�V���� );
            return;
        }
    }
    void EnemyTutorial::�U�����[�V����( cinder::CameraPersp const& camera )
    {
        // ���[�V��������60�t���[���̌�A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            texture = �U���摜;
            SetFunction( &EnemyTutorial::�U�� );
            return;
        }
    }
    void EnemyTutorial::�U��( cinder::CameraPersp const& camera )
    {
        // �U�����Ă������̊֐��ցB
        isAttack = true;
        timer.Advance( 10 ); // �d��10�t���[��
        SetFunction( &EnemyTutorial::�U����d�� );
        return;
    }
    void EnemyTutorial::�U����d��( cinder::CameraPersp const& camera )
    {
        // �U���t���O���I�t�ɁA�d������10�t���[���̌�A���̊֐��ցB
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            texture = �ҋ@;
            SetFunction( &EnemyTutorial::�W�����v�Ŗ߂� );
            return;
        }
    }
    void EnemyTutorial::�W�����v�Ŗ߂�( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 8.0F, M_PI / 8.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // �d����A�����ɃW�����v���Ď��̊֐��ցB
        Jump( jumpPower + backDirection );

        SetFunction( &EnemyTutorial::���n );
        return;
    }
    void EnemyTutorial::���n( cinder::CameraPersp const& camera )
    {
        // �n�ʂɒ�������A���̊֐��ցB
        if ( isLanding )
        {
            // �U���̃`���[�g���A�����s���܂��B
            if ( !TRData::playerAttack.IsComplete( ) )
            {
                TRData::playerAttack.TutorialStart( );
            }

            // �X�s�[�h��0�ɂ��܂��B�������Ⴄ�̂ŁB
            object.Speed( Vec3f::zero( ) );
            // ���̍U���܂ł̃t���[�������B
            timer.Advance( randInt( 60, 240 ) );
            SetFunction( &EnemyTutorial::�������Ȃ� );
            return;
        }
    }

    bool EnemyTutorial::IsJumping( )
    {
        return isLanding == false;
    }
    void EnemyTutorial::SetFunction( void( EnemyTutorial::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}