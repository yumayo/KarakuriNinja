#pragma once

#include <list>
#include <memory>

#include "cinder/Camera.h"
#include "EnemyBase.h"
#include "../Utilitys/Nomoto.h"

namespace User
{
    using EnemyBaseRef = std::shared_ptr<EnemyBase>;
    using EnemyList = std::list<EnemyBaseRef>;

    class EnemyController
    {
        EnemyList enemyList;
        EnemyBulletList bulletList;
    public:
        EnemyController( const cinder::CameraPersp& camera );
        void update( );
        void draw( );
    public:
        // エネミーの作成を行います。
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera );
        // エネミーが一体でも攻撃していたら true になります。
        bool IsAttack( const cinder::CameraPersp& camera );
        // プレイヤーからエネミーへのダメージラインとの当たり判定も込み
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（直接ダメージを通します）
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（プレイヤーがガードをしている時の判定に使います）
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // エネミーが攻撃したら、元の位置に戻します。
        void ResetPosition( const cinder::CameraPersp& camera );
        // 発射した弾を全て回収します。この関数を呼ぶとこのクラスが持っている弾を全てクリアします。
        EnemyBulletList BulletsRecovery( );
    private:
        // エネミーのHPが０だったら削除
        void EnemyEraser( );
        // 各エネミーの弾を回収します。
        void EnemyBulletsRecovery( );
    };

    using EnemyControllerRef = std::shared_ptr<EnemyController>;

    template<class Ty>
    inline void EnemyController::Create( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }
}
