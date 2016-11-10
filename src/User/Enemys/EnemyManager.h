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
        float colliedSize = 1.0F;
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
        // エネミーの作成を行います。
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera );
        // エネミーの作成を行います。
        template <class Ty>
        void Create( cinder::Vec3f position, const cinder::CameraPersp& camera, std::string const& fieldName );
        // エネミーが一体でも攻撃していたら true になります。
        bool IsAttack( const cinder::CameraPersp& camera );
        // プレイヤーからエネミーへのダメージラインとの当たり判定も込み
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // プレイヤーの必殺技をエネミーへと与えます。
        int PlayerSpecialAttackToEnemyDamage( int damage );
        // エネミーからプレイヤーへの攻撃（直接ダメージを通します）
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（プレイヤーがガードをしている時の判定に使います）
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        
        // エネミーの当たり判定域を描画します。
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
        bool IsEmpty( );

        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }

        // 発射した弾を全て回収します。この関数を呼ぶとこのクラスが持っている弾を全てクリアします。
        EnemyBulletList BulletsRecovery( );

        EffectList EffectRecovery( );
    private:
        void Each( std::function<void( EnemyBaseRef& )> function );
    private: // 以下アップデートで回します。
        // エネミーのHPが０だったら削除
        void EnemyEraser( );
        // 各エネミーの弾を回収します。
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
