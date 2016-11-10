#pragma once

#include <memory>
#include <list>
#include <functional>

#include "EnemyBulletBase.h"

#include "cinder/Camera.h"
#include "../Utilitys/Nomoto.h"

namespace User
{
    using EnemyBulletBaseRef = std::shared_ptr<EnemyBulletBase>;
    using EnemyBulletList = std::list<EnemyBulletBaseRef>;

    class EnemyBulletManager
    {
    public:
        EnemyBulletManager( );
        void update( );
        void draw( );
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
        // �e�̓����蔻����`�悵�܂��B
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
        // �e��������ꏊ��`�悵�܂��B
        void DrawBulletCircle( cinder::CameraPersp const & camera );
    private:
        void Each( std::function<void( EnemyBulletBaseRef& )> func );
        void BulletEraser( );
    private:
        EnemyBulletList bulletList;
    };

    using EnemyBulletManagerRef = std::shared_ptr<EnemyBulletManager>;
}