#pragma once

#include <memory>
#include <list>
#include <functional>

#include "EnemyBulletBase.h"
#include "../Effect/EffectManager.h"
#include "cinder/Camera.h"
#include "../Utilitys/Nomoto.h"
#include"../Utilitys/Audio.h"
namespace User
{
    using EnemyBulletBaseRef = std::shared_ptr<EnemyBulletBase>;
    using EnemyBulletList = std::list<EnemyBulletBaseRef>;

    class EnemyBulletManager
    {
        float colliedSize = 1.5F;
        int score = 0;
        Audio* guard_se;
        Audio* playerdamaged_se;
        Audio* adddamage;
        int frame = 0;
        cinder::gl::Texture* attackCircleTex;
    private:
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        EnemyBulletManager( );
        void update( );
        void draw( cinder::CameraPersp const& camera );
    public:
        void BulletRegister( EnemyBulletList& bulletList );
        // �e�S�ĂƐ������������Ă��邩�̔���
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // �v���C���[�̃X�y�V�����U�����āA�S�e��������
        int PlayerSpecialAttackToEnemyDamage( );
        // �v���C���[����G�l�~�[�ւ̍U���i���ڃ_���[�W��ʂ��܂��j
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // �G�l�~�[����v���C���[�ւ̍U���i�v���C���[���K�[�h�����Ă��鎞�̔���Ɏg���܂��j
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // �X�R�A��������܂��B���������A���g�̓N���A���܂��B
        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }
        // �e��������ꏊ��`�悵�܂��B
        void DrawBulletCircle( cinder::CameraPersp const & camera );
    public:
        // �e�̓����蔻����`�悵�܂��B�i�f�o�b�O�p�j
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
    private:
        void Each( std::function<void( EnemyBulletBaseRef& )> func );
        void BulletEraser( );

    public: EffectList EffectRecovery( );
    private: template <class Ty>
        void EffectCreate( Ty const& instans );
    };

    using EnemyBulletManagerRef = std::shared_ptr<EnemyBulletManager>;

    template<class Ty>
    inline void EnemyBulletManager::EffectCreate( Ty const & instans )
    {
        effectList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}