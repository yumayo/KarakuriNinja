#pragma once

#include <list>
#include <memory>
#include <functional>

#include "cinder/Camera.h"
#include "EnemyBase.h"
#include "../Utilitys/Nomoto.h"
#include"../Utilitys/Audio.h"

#include "cinder/ObjLoader.h"        // OBJ形式を読み込む
#include "cinder/TriMesh.h"          // 三角ポリゴンの集合体

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
        // エネミーが一体でも攻撃していたら true になります。
        bool IsAttack( const cinder::CameraPersp& camera );
        // プレイヤーからエネミーへのダメージラインとの当たり判定も込み
        int PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera, float value = 1.0F, float combo = 0.0F );
        // プレイヤーからの攻撃が何体のエネミーに当たったのかを判定します。
        int PlayerToEnemyAttackCheck( Line& line_, const cinder::CameraPersp& camera );
        // プレイヤーの必殺技をエネミーへと与えます。
        int PlayerSpecialAttackToEnemyDamage( int damage, const cinder::CameraPersp& camera, SpecialType specialState, float combo = 0.0F );
        // エネミーからプレイヤーへの攻撃（直接ダメージを通します）
        int EnemyToPlayerDamage( const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃（プレイヤーがガードをしている時の判定に使います）
        int EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera );
        // エネミーからプレイヤーへの攻撃を防いでいるかの判定をします。(主にチュートリアルに使います)
        bool EnemyToPlayerGuardCheck( Line& line_, const cinder::CameraPersp& camera );
        // エネミーがいないかどうか
        bool IsEmpty( );
        // スコアを回収します。回収したら、中身はクリアします。
        int ScoreRecovery( ) { auto temp = score; score = 0; return temp; }
        // エネミーの攻撃が当たる場所を描画します。
        void DrawAttackCircle( cinder::CameraPersp const & camera );
        // エネミーの合計体力を表示します。
        void DrawEnemyHitPoint( );
    public:
        // エネミーの当たり判定域を描画します。(デバッグ用)
        void DrawCollisionCircle( cinder::CameraPersp const& camera );
    private:
        void Each( std::function<void( EnemyBaseRef& )> function );
    private: // 以下アップデートで回します。
        // エネミーのHPが０だったら削除
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
