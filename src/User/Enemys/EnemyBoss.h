#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <vector>
#include <functional>
#include"../Utilitys/Audio.h"
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
    private:
        cinder::gl::Texture* �ҋ@;
        cinder::gl::Texture* �����郂�[�V�����摜;
        cinder::gl::Texture* ������摜;
        cinder::gl::Texture* �U�����[�V�����摜;
        cinder::gl::Texture* �U���摜;
        cinder::gl::Texture* ���Ɉړ�;
        cinder::gl::Texture* �E�Ɉړ�;
        cinder::gl::Texture* �|�ꂩ���郂�[�V����;
        cinder::gl::Texture* �|��郂�[�V����;

    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( const cinder::CameraPersp& camera ) override;
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
        void ����( cinder::CameraPersp const& camera );
    private:
        void �e�𓊂���܂ł̃��[�V����( cinder::CameraPersp const& camera );
        void �e�𓊂���( cinder::CameraPersp const& camera );
        void �e�𓊂�����̍d��( cinder::CameraPersp const& camera );
        void �e��������x�����邩�̊m�F( cinder::CameraPersp const& camera );
    private:
        void �o���������̃Z���t( cinder::CameraPersp const& camera );
        bool IsHalfHPSerif( );
        void HP�������ȉ��ɂȂ������̃Z���t( cinder::CameraPersp const& camera );
        bool IsDeadSerif( );
        void ���ʎ��̃Z���t( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBoss::* function ) ( cinder::CameraPersp const& camera ) );
    private:
        void �k���Z���t( );
        void �Z���t( );
    private:
        void SetSerifFunction( std::string const& serif, void ( EnemyBoss::* function ) ( ) );
    private:
        // ��ʂ𒆐S�ɉ�]�Ō��߂�g��o���b�g�U��
        void WavyBulletFiring( float angle, cinder::CameraPersp const& camera );
        // �����_���Ɍ��߂���_���璆�_�����߂�g��o���b�g�U��
        void RandomWavyBulletFiring( cinder::CameraPersp const& camera );
        std::vector<Audio*> se;
        int �e�̏���� = 3;
        int �e�J�E���g;
    };
}