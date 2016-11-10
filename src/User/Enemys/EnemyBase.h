#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"
#include "../Effect/EffectManager.h"
#include "AttackTime.h"

namespace User
{
    class EnemyBase;
    using EnemyBaseRef = std::shared_ptr<EnemyBase>;
    using EnemyList = std::list<EnemyBaseRef>;

    class EnemyBase
    {
    public:
        class Status
        {
        public:
            Status( )
                : maxHP( 1 )
                , HP( maxHP )
                , attackPoint( 0 )
            { }
            Status( float HP, int attackPoint )
                : maxHP( HP )
                , HP( maxHP )
                , attackPoint( attackPoint )
            { }
            float maxHP;
            float HP;
            int attackPoint;
        };
        // 公開情報
    protected:
        EnemyObject object;
        EnemyObject initObject;
        bool isLanding;
        bool isLive;
        cinder::gl::Texture* texture;
        cinder::gl::Texture* knockBackTexture;
        AttackTime attackTime;
        int frame = 0;
        cinder::ColorA hitColor;
        int deadTime;
        Status status;
        int mutekiFrame;
        int maxMutekiFrame;

    private: EnemyList enemyList;
    public: EnemyList EnemyRecovery( );
    protected:  template <class Ty>
        void EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera );

    private: EnemyBulletList bulletList;
    public: EnemyBulletList BulletRecovery( );
    protected: template <class Ty>
        void BulletCreate( Ty const& instans );

    private: EffectList effectList;
    public: EffectList EffectRecovery( );
    protected: template <class Ty>
        void EffectCreate( Ty const& instans );

    public:
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, Status status, bool bullet );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, Status status, float scale = 1.0F );
        virtual void update( cinder::CameraPersp const& camera );
        virtual void draw( );
        virtual void drawUI( cinder::CameraPersp const& camera );
        // エネミーが攻撃しているなら true が返って来ます。
        // 継承して攻撃方法を入れてください。
        virtual bool Attack( const cinder::CameraPersp& camera );
        // 生きているかの状態
        virtual bool IsLive( ) { return isLive; }
    public: // ゲッター
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return status.attackPoint; }
        cinder::ColorA HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return status.HP / status.maxHP; }
        Status GetStatus( ) { return status; }
    public:
        bool IsAttackMotion( ) { return attackTime.IsAttackMotion( ); }
        bool IsAttackOneFramePrev( ) { return attackTime.IsAttackOneFramePrev( ); }
        float NormalizedAttackFrame( ) { return attackTime.NormalizedAttackFrame( ); }
    public:
        // 引数 : あたった時の中心からの距離(正規化済み) : 0.0 ~ 1.0(半径)
        int Hit( cinder::CameraPersp const& camera, float length, int scoreRate, float value = 1.0F );
        // 強制的にダメージを与える関数
        int Damage( cinder::CameraPersp const& camera, int damage, int scoreRate );
        // 敵を強制的に殺す関数
        void Kill( );
        // エネミーを消していいかの確認
        bool IsActive( );
    protected:
        bool IsJumping( );
        // 地面の中にいるかどうか。
        bool IsUnderGround( );
        // スクリーンの中にいる時はtrueが返ってきます。
        bool IsInTheScreen( cinder::CameraPersp const& camera );
        // フィールドの中にいる時はtrueが返ってきます。
        bool IsInField( );
        // ノックバックしているかどうか。
        bool IsKnockBack( );

        bool IsMuteki( );
    protected: // 以下 アップデートに記入推奨
        void CollideField( );
        // 地面の中にいたら地上に引き戻します。
        void CollideGround( );
        // HPがなくなったら死ぬまでカウントします。
        void Dying( );
    protected:
        // 生きているかを確認します。
        void LiveCheck( );

        void MutekiEffect( );
        // ダメージを受けた時に体の色を徐々に元通りにします。
        void DamageEffect( );
        // カメラの方向を向きます。
        void CameraSee( cinder::CameraPersp const& camera );
    protected:
        void Jump( cinder::Vec3f jumpPower );
    private:
        void SpawnEffect( cinder::CameraPersp const& camera );
    };

    template<class Ty>
    inline void EnemyBase::EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }
    template<class Ty>
    inline void EnemyBase::BulletCreate( Ty const& instans )
    {
        bulletList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
    template<class Ty>
    inline void EnemyBase::EffectCreate( Ty const & instans )
    {
        effectList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}