#pragma once

#include <memory>
#include <list>

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
        // 弾全てと線分が当たっているかの判定
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // プレイヤーからエネミーへの攻撃（直接ダメージを通します）
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（プレイヤーがガードをしている時の判定に使います）
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
    private:
        void BulletEraser( );
    private:
        EnemyBulletList bulletList;
    };

    using EnemyBulletManagerRef = std::shared_ptr<EnemyBulletManager>;
}