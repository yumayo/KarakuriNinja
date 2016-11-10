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
        // 弾全てと線分が当たっているかの判定
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera );
        // プレイヤーのスペシャル攻撃して、全弾消す処理
        int PlayerSpecialAttackToEnemyDamage( );
        // プレイヤーからエネミーへの攻撃（直接ダメージを通します）
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（プレイヤーがガードをしている時の判定に使います）
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // スコアを回収します。回収したら、中身はクリアします。
        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }
        // 弾が当たる場所を描画します。
        void DrawBulletCircle( cinder::CameraPersp const & camera );
    public:
        // 弾の当たり判定域を描画します。（デバッグ用）
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