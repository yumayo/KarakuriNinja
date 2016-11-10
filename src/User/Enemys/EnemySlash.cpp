# include "EnemySlash.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemySlash::EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 8.0F, 3 ) )
        , timer( )
        , isAttack( false )
        , prevMovePosition( Vec3f::zero( ) )
    {
        timer.Advance( randInt( 200, 300 ) );

        int index = 1;
        �ҋ@ = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �U�����[�V�����摜 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �U���摜 = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        ���Ɉړ� = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );
        �E�Ɉړ� = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        texture = �ҋ@;
        knockBackTexture = &GData::FindTexture( "Enemy/Slash/Slash (" + std::to_string( index++ ) + ").png" );

        SetFunction( &EnemySlash::�^�C�}�[����܂őҋ@ );
    }
    void EnemySlash::update( cinder::CameraPersp const& camera )
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
    }
    void EnemySlash::draw( )
    {
        EnemyBase::draw( );
    }
    bool EnemySlash::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && IsLive( ) && !IsKnockBack( );
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
            if ( randInt( 0, 3 ) != 0 )
            {
                prevMovePosition = object.Position( );

                auto cameraRightVec = camera.getViewDirection( );
                cameraRightVec.rotate( camera.getWorldUp( ), -M_PI / 2.0 );
                targetVector = cameraRightVec * randFloat( -1, 1 );

                timer.Advance( 120 );
                SetFunction( &EnemySlash::�J�����֋߂Â� );
                return;
            }
            else
            {
                timer.Advance( randInt( 30, 60 ) );
                if ( randBool( ) )
                {
                    texture = ���Ɉړ�;
                    SetFunction( &EnemySlash::���ֈړ� );
                }
                else
                {
                    texture = �E�Ɉړ�;
                    SetFunction( &EnemySlash::�E�ֈړ� );
                }
            }
        }
    }
    void EnemySlash::�J�����֋߂Â�( cinder::CameraPersp const& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );

        auto targetPos = cameraPos + targetVector;

        auto x = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.x, targetPos.x );
        auto z = EasingLinear( timer.NomalizedFrame( ), prevMovePosition.z, targetPos.z );

        object.Position( Vec3f( x, object.Position( ).y, z ) );

        if ( timer.IsAction( ) )
        {
            timer.Advance( 60 ); // �U�����[�V�����t���[������
            attackTime.AttackFrame( 60 );
            prevAttackHP = status.HP;
            texture = �U�����[�V�����摜;
            SetFunction( &EnemySlash::�U�����[�V���� );
            return;
        }
    }
    void EnemySlash::�U�����[�V����( cinder::CameraPersp const& camera )
    {
        // ���[�V��������60�t���[���̌�A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            texture = �U���摜;
            SetFunction( &EnemySlash::�U�� );
            return;
        }

        if ( prevAttackHP != status.HP )
        {
            texture = �ҋ@;
            timer.Advance( 10 ); // �d��10�t���[��
            SetFunction( &EnemySlash::�U����d�� );
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
            texture = �ҋ@;
            SetFunction( &EnemySlash::�W�����v�Ŗ߂� );
            return;
        }
    }
    void EnemySlash::�W�����v�Ŗ߂�( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * randFloat( 6.0F, 12.0F ) / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 8.0F, M_PI / 8.0F ) );
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

    void EnemySlash::���ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemySlash::�E�ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemySlash::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemySlash::�E�ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemySlash::���ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemySlash::�^�C�}�[����܂őҋ@ );
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