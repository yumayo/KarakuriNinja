#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <vector>
#include <functional>

namespace User
{
    class EnemyBoss : public EnemyBase
    {
        Timer timer;
        bool isAttack;
        cinder::Vec3f moveSpeed;
        cinder::Vec3f moveLeftRightSpeed;
        bool isBulletFiring;
        bool isDeadStop;
        std::function<void( cinder::CameraPersp const& )> behavior;
    private:
        bool isHalfHPSerif;
        bool isDeadSerif;
        Fonts font;
        std::string serif;
        cinder::Vec2f serifDrawPosition;
        std::function<void( )> serifBehavior;
    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        // �d�͂������܂��B
        void Gravitate( );
    private:
        void �^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera );
        void �ǂ����̍U�����o�����̊m�F( cinder::CameraPersp const& camera );
    private:
        void ���E�ɍ����ړ����Ȃ���J�����֋߂Â�( cinder::CameraPersp const& camera );
        void �U�����[�V����( cinder::CameraPersp const& camera );
        void �U��( cinder::CameraPersp const& camera );
        void �U����d��( cinder::CameraPersp const& camera );
        void �W�����v�Ŗ߂�( cinder::CameraPersp const& camera );
        void ���n( cinder::CameraPersp const& camera );
    private:
        void �e�𓊂���( cinder::CameraPersp const& camera );
        void �e��������x�����邩�̊m�F( cinder::CameraPersp const& camera );
    private:
        void �o���������̃Z���t( cinder::CameraPersp const& camera );
        void HP�������ȉ��ɂȂ������̃Z���t( cinder::CameraPersp const& camera );
        void ���ʎ��̃Z���t( cinder::CameraPersp const& camera );
    private:
        void �k���Z���t( );
        void �Z���t( );
    private:
        bool IsJumping( );
        // �X�N���[���̒��ɂ��鎞��true���Ԃ��Ă��܂��B
        bool IsInTheScreen( cinder::CameraPersp const& camera );
        bool IsHalfHPSerif( );
        bool IsDeadSerif( );
    private:
        void SetFunction( void ( EnemyBoss::* function ) ( cinder::CameraPersp const& camera ) );
    private:
        // �Z���t���荞��
        void SerifInterrupt( );
        void SetSerifFunction( std::string const& serif, void ( EnemyBoss::* function ) ( ) );
    private:
        // ��ʂ𒆐S�ɉ�]�Ō��߂�g��o���b�g�U��
        void WavyBulletFiring( float angle, cinder::CameraPersp const& camera );
        // �����_���Ɍ��߂���_���璆�_�����߂�g��o���b�g�U��
        void RandomWavyBulletFiring( cinder::CameraPersp const& camera );
    };
}