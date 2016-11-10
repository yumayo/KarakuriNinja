# include "EnemyBulletWalk.h"

namespace User
{
    using namespace cinder;

    EnemyBulletWalk::EnemyBulletWalk( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        �ҋ@ = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (1).png" );


        ���Ɉړ� = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (4).png" );
        �E�Ɉړ� = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (5).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (6).png" );

        texture = �ҋ@;

        SetFunction( &EnemyBulletWalk::�^�C�}�[����܂őҋ@ );
    }

    void EnemyBulletWalk::update( cinder::CameraPersp const& camera )
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
    void EnemyBulletWalk::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        // �^�C�}�[�������玟�̊֐��ցB
        if ( timer.IsAction( ) )
        {
            timer.Advance( randInt( 30, 60 ) );
            if ( randBool( ) )
            {
                texture = ���Ɉړ�;
                SetFunction( &EnemyBulletWalk::���ֈړ� );
            }
            else
            {
                texture = �E�Ɉړ�;
                SetFunction( &EnemyBulletWalk::�E�ֈړ� );
            }
        }
    }
    void EnemyBulletWalk::���ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemyBulletWalk::�E�ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemyBulletWalk::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBulletWalk::�E�ֈړ�( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemyBulletWalk::���ֈړ� );
        }

        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 60, 180 ) );
            texture = �ҋ@;
            SetFunction( &EnemyBulletWalk::�^�C�}�[����܂őҋ@ );
        }
    }
    void EnemyBulletWalk::SetFunction( void( EnemyBulletWalk::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}