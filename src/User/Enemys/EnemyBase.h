#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"
#include "../Effect/EffectManager.h"

namespace User
{
    class AttackTime
    {
        bool isAttackMotion;
        int attackFrame;
        int maxAttackFrame;
    public:
        AttackTime( )
            : isAttackMotion( false )
        {
            AttackFrame( 0 );
        }
        void Update( )
        {
            attackFrame = std::min( attackFrame + 1, maxAttackFrame );
            if ( attackFrame == maxAttackFrame ) isAttackMotion = false;
        }
        void AttackFrame( int attackFrame )
        {
            if ( attackFrame <= 0 ) // 0除算回避
            {
                isAttackMotion = false;
                this->attackFrame = 1;
                maxAttackFrame = 1;
            }
            else
            {
                isAttackMotion = true;
                this->attackFrame = 0;
                maxAttackFrame = attackFrame;
            }
        }
        bool IsAttackMotion( ) { return isAttackMotion; }
        bool IsAttackOneFramePrev( ) { return maxAttackFrame == attackFrame + 1; }
        float NormalizedAttackFrame( ) { return static_cast<float>( attackFrame ) / maxAttackFrame; }
    };

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
        // 非公開情報
    private:
        cinder::Color hitColor;
        int deadTime;
        Status status;
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        // 発射した弾を全て回収します。この関数を呼ぶとこのクラスが持っている弾を全てクリアします。
        EnemyBulletList BulletsRecovery( );
        EffectList EffectRecovery( );
    public:
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
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
        cinder::Color HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return status.HP / status.maxHP; }
        Status GetStatus( ) { return status; }
    public:
        bool IsAttackMotion( ) { return attackTime.IsAttackMotion( ); }
        bool IsAttackOneFramePrev( ) { return attackTime.IsAttackOneFramePrev( ); }
        float NormalizedAttackFrame( ) { return attackTime.NormalizedAttackFrame( ); }
    public:
        // 引数 : あたった時の中心からの距離(正規化済み) : 0.0 ~ 1.0(半径)
        int Hit( cinder::CameraPersp const& camera, float length, int scoreRate, float value = 1.0F);
        // 強制的にダメージを与える関数
        int Damage( int damage );
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
    protected: // 以下 アップデートに記入推奨
        void CollideField( );
        // 地面の中にいたら地上に引き戻します。
        void CollideGround( );
        // HPがなくなったら死ぬまでカウントします。
        void Dying( );
    protected:
        // 生きているかを確認します。
        void LiveCheck( );
        // ダメージを受けた時に体の色を徐々に元通りにします。
        void DamageEffect( );
    protected:
        // 弾を作成します。作成した弾はエネミーに一時的に保存されます。
        template <class Ty>
        void BulletCreate( Ty const& instans );
    protected:
        // 弾を作成します。作成した弾はエネミーに一時的に保存されます。
        template <class Ty>
        void EffectCreate( Ty const& instans );
    protected:
        void CameraSee( cinder::CameraPersp const& camera );
        void Jump( cinder::Vec3f jumpPower );
    private:
        void SpawnEffect( cinder::CameraPersp const& camera );
    };

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