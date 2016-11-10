#pragma once

#include <list>
#include <memory>
#include <functional>

#include "cinder/Camera.h"
#include "EnemyBase.h"
#include "../Utilitys/Nomoto.h"
#include"../Utilitys/Audio.h"

#include "cinder/ObjLoader.h"        // OBJ�`����ǂݍ���
#include "cinder/TriMesh.h"          // �O�p�|���S���̏W����

#include "../Special/Special.h"

#include "../Utilitys/BossProduction.h"

namespace User
{
    class EnemyManager
    {
        float playerAttackColliedSize = 1.0F;
        float enemyAttackColliedSize = 0.5F;
        int score = 0;
        Audio* gurad_se;
        Audio* playerdamaged_se;
        Audio* adddamage;
        Audio* dead;
        int frame = 0;
        cinder::gl::Texture* attackCircleTex;
        int maxEnemyHitPoint = 0;
        int enemyHitPoint = 0;
    private:
        EnemyList enemyList;
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        EnemyManager( const cinder::CameraPersp& camera, std::string const& path );
        void update( cinder::CameraPersp const& camera );
        void draw( cinder::CameraPersp const& camera );
        void drawUI( cinder::CameraPersp const& camera );
    private:
        BossProduction bossProduction;
    public:
        bool IsMainBGMGainDown( );
        bool IsMainBGMStop( );
        bool IsBossBGMStart( );
        void drawProduction( );
    public:
        // �G�l�~�[����̂ł��U�����Ă����� true �ɂȂ�܂��B
        bool IsAttack( const cinder::CameraPersp& camera );
        // �v���C���[����G�l�~�[�ւ̃_���[�W���C���Ƃ̓����蔻�������
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera, float value = 1.0F, float combo = 0.0F );
        // �v���C���[����̍U�������̂̃G�l�~�[�ɓ��������̂��𔻒肵�܂��B
        int PlayerToEnemyAttackCheck( Line& line_, const cinder::CameraPersp& camera );
        // �v���C���[�̕K�E�Z���G�l�~�[�ւƗ^���܂��B
        int PlayerSpecialAttackToEnemyDamage( int damage, const cinder::CameraPersp& camera, SpecialType specialState, float combo = 0.0F );
        // �G�l�~�[����v���C���[�ւ̍U���i���ڃ_���[�W��ʂ��܂��j
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // �G�l�~�[����v���C���[�ւ̍U���i�v���C���[���K�[�h�����Ă��鎞�̔���Ɏg���܂��j
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // �G�l�~�[����v���C���[�ւ̍U����h���ł��邩�̔�������܂��B(��Ƀ`���[�g���A���Ɏg���܂�)
        bool EnemyToPlayerGuardCheck( Line& line_, const cinder::CameraPersp& camera );
        // �G�l�~�[�����Ȃ����ǂ���
        bool IsEmpty( );
        // �X�R�A��������܂��B���������A���g�̓N���A���܂��B
        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }
        // �G�l�~�[�̍U����������ꏊ��`�悵�܂��B
        void DrawAttackCircle( cinder::CameraPersp const & camera );
        // �G�l�~�[�̍��v�̗͂�\�����܂��B
        void DrawEnemyHitPoint( );
    public:
        // �G�l�~�[�̓����蔻����`�悵�܂��B(�f�o�b�O�p)
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
    private:
        void Each( std::function<void( EnemyBaseRef& )> function );
    private: // �ȉ��A�b�v�f�[�g�ŉ񂵂܂��B
        // �G�l�~�[��HP���O��������폜
        void EnemyEraser( cinder::CameraPersp const& camera );
    private:
        void EnemyIntegration( );
        template <class Ty>
        void EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera );
    public:
        EnemyBulletList BulletRecovery( );
    private:
        void EnemyBulletIntegration( );
    public:
        EffectList EffectRecovery( );
    private:
        void EnemyEffectIntegration( );
        template <class Ty>
        void EffectCreate( Ty const& instans );
    private:
        bool IsDamage( Line line_, cinder::Vec2f pos_, float size_ );
        float NormalizedHitPoint( ) { return static_cast<float>( enemyHitPoint ) / maxEnemyHitPoint; }
    };

    using EnemyManagerRef = std::shared_ptr<EnemyManager>;

    template<class Ty>
    inline void EnemyManager::EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }

    template<class Ty>
    inline void EnemyManager::EffectCreate( Ty const & instans )
    {
        effectList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}
