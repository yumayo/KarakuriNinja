#pragma once

#include <list>
#include <memory>
#include <functional>

#include "cinder/Camera.h"
#include "EnemyBase.h"
#include "../Utilitys/Nomoto.h"
#include"../Utilitys/Audio.h"

namespace User
{
    using EnemyBaseRef = std::shared_ptr<EnemyBase>;
    using EnemyList = std::list<EnemyBaseRef>;

    class EnemyManager
    {
        float playerAttackColliedSize = 1.0F;
        float enemyAttackColliedSize = 0.5F;
        int score = 0;
        Audio* gurad_se;
        Audio* playerdamaged_se;
        Audio* adddamage;
    private:
        EnemyList enemyList;
    private:
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        EnemyManager( const cinder::CameraPersp& camera, std::string const& path );
        void update( cinder::CameraPersp const& camera );
        void draw( cinder::CameraPersp const& camera );
        void drawUI( );
    public:
        // �G�l�~�[�̍쐬���s���܂��B
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera );
        // �G�l�~�[�̍쐬���s���܂��B
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera, std::string const& fieldName );
        // �G�l�~�[����̂ł��U�����Ă����� true �ɂȂ�܂��B
        bool IsAttack( const cinder::CameraPersp& camera );
        // �v���C���[����G�l�~�[�ւ̃_���[�W���C���Ƃ̓����蔻�������
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // �v���C���[�̕K�E�Z���G�l�~�[�ւƗ^���܂��B
        int PlayerSpecialAttackToEnemyDamage( int damage );
        // �G�l�~�[����v���C���[�ւ̍U���i���ڃ_���[�W��ʂ��܂��j
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // �G�l�~�[����v���C���[�ւ̍U���i�v���C���[���K�[�h�����Ă��鎞�̔���Ɏg���܂��j
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // �G�l�~�[�����Ȃ����ǂ���
        bool IsEmpty( );
        // �X�R�A��������܂��B���������A���g�̓N���A���܂��B
        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }
        // �G�l�~�[�̍U����������ꏊ��`�悵�܂��B
        void DrawAttackCircle( cinder::CameraPersp const & camera );
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
        // �G�t�F�N�g��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���G�t�F�N�g��S�ăN���A���܂��B
        EffectList EffectRecovery( );
    public:
        // �G�l�~�[�̓����蔻����`�悵�܂��B(�f�o�b�O�p)
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
    private:
        void Each( std::function<void( EnemyBaseRef& )> function );
    private: // �ȉ��A�b�v�f�[�g�ŉ񂵂܂��B
        // �G�l�~�[��HP���O��������폜
        void EnemyEraser( );
        // �e�G�l�~�[�̒e��������܂��B
        void EnemyBulletsRecovery( );
		
        void EnemyEffectsRecovery( );
    };

    using EnemyManagerRef = std::shared_ptr<EnemyManager>;

    template<class Ty>
    inline void EnemyManager::Create( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }

    template<class Ty>
    inline void EnemyManager::Create( cinder::Vec3f position, const cinder::CameraPersp& camera, std::string const& fieldName )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera, fieldName ) );
    }
}
