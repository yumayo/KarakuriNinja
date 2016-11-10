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
        Fonts font;
        cinder::gl::Texture* messageBoxLeft;
        cinder::gl::Texture* messageBoxCenter;
        cinder::gl::Texture* messageBoxRight;
        std::string serif;
        std::function<void( )> serifBehavior;
    private:
        std::list<EnemyObject> objects;
        cinder::Vec3f prevPosition = cinder::Vec3f::zero( );
    private:
        cinder::JsonTree jsonEnemys;
        cinder::JsonTree::Iter jsonItr;
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
    private:
        cinder::gl::Texture* �I�[��1;
        cinder::gl::Texture* �I�[��2;
        cinder::gl::Texture* auraTex;
        void DrawAura( );
    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( const cinder::CameraPersp& camera ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
        bool IsLive( ) override;
    private:
        // �d�͂������܂��B
        void Gravitate( );
        void DrawAfterimage( EnemyObject& object, int index );
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
        void ���ʎ��̃��[�V����( cinder::CameraPersp const& camera );
        void ����( cinder::CameraPersp const& camera );
    private:
        void �e�𓊂���܂ł̃��[�V����( cinder::CameraPersp const& camera );
        void �e�𓊂���( cinder::CameraPersp const& camera );
        void �e�𓊂�����̍d��( cinder::CameraPersp const& camera );
        void �e��������x�����邩�̊m�F( cinder::CameraPersp const& camera );
    private:
        void �{�X�o���̃G�t�F�N�g���I������܂őҋ@( cinder::CameraPersp const& camera );
        void �o���������ɃG�l�~�[���X�|�[��������( cinder::CameraPersp const& camera );
        bool isFrieldKilledSerif = false;
        bool IsFrieldKilledSerif( );
        void ������|���ꂽ���̃Z���t( cinder::CameraPersp const& camera );
        bool isDeadSerif = false;
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
        int �e�𓊂���� = 0;
        int �e�𓊂������� = 2;
        int �e�̏���� = 2;
        int �e�J�E���g = 0;
        int ���ڍU���̏���� = 2;
        int ���ڍU���J�E���g = 0;
    };
}