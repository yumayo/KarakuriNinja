# include "EnemySlashWalk.h"

namespace User
{
    using namespace cinder;

    EnemySlashWalk::EnemySlashWalk( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        �ҋ@ = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (1).png" );


        ���Ɉړ� = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (4).png" );
        �E�Ɉړ� = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (5).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (6).png" );

        texture = �ҋ@;

        SetFunction( &EnemySlashWalk::�^�C�}�[����܂őҋ@ );
    }

    void EnemySlashWalk::update( cinder::CameraPersp const& camera )
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
    void EnemySlashWalk::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        // �^�C�}�[�������玟�̊֐��ցB
        if ( timer.IsAction( ) )
        {
            timer.Advance( randInt( 30, 60 ) );
            if ( randBool( ) )
            {
                texture = ���Ɉړ�;
                SetFunction( &EnemySlashWalk::���ֈړ� );
            }
            else
            {
                texture = �E�Ɉړ�;
                SetFunction( &EnemySlashWalk::�E�ֈړ� );
            }
        }
    }
    void EnemySlashWalk::���ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemySlashWalk::�E�ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemySlashWalk::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemySlashWalk::�E�ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemySlashWalk::���ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemySlashWalk::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemySlashWalk::SetFunction( void( EnemySlashWalk::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}