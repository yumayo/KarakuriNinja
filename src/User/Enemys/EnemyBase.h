#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"

namespace User
{
    class EnemyBase
    {
    protected:
        EnemyObject object;
        EnemyObject initObject;
        bool isLanding;
        cinder::gl::TextureRef textureRef;
    private:
        float maxHP;
        float HP; // HPの変動はプレイヤーの攻撃のみなのでprivate.
        cinder::Color hitColor; // カラーの変動はプレイヤーの攻撃のみなのでprivate.
        bool isLive; // 生きているかどうかを判断するのは各自でやらないのでprivate.
        int deadTime; // 死にゆく時間もいじることはないのでprivate.
        int attackPoint; // プレイヤーへの攻撃力もいじることはないのでprivate.
        EnemyBulletList bulletList; // 一時的に弾をエネミーに保存しておくだけなのでprivate.
    protected: // 画像を作成しないコンストラクタ
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, float sizeScale );
    public: // 画像を作成するコンストラクタ
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& path );
        EnemyBase( cinder::Vec3f pos, float sizeScale, const cinder::CameraPersp& camera, std::string const& path );
        EnemyBase( cinder::Vec3f pos, float sizeScale, float HP, int attackPoint, const cinder::CameraPersp& camera, std::string const& path );
        virtual void update( cinder::CameraPersp const& camera );
        virtual void draw( );
        virtual void drawUI( );
        // エネミーが攻撃しているなら true が返って来ます。
        // 継承して攻撃方法を入れてください。
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // ゲッター
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        bool IsLive( ) { return isLive; }
        cinder::Color HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return HP / maxHP; }
    public:
        // 引数 : あたった時の中心からの距離(正規化済み) : 0.0 ~ 1.0(半径)
        int Hit( float length );
        // 強制的にダメージを与える関数
        int Damage( int damage );
        // 敵を強制的に殺す関数
        void Kill( );
        // エネミーを消していいかの確認
        bool IsActive( );
        // 発射した弾を全て回収します。この関数を呼ぶとこのクラスが持っている弾を全てクリアします。
        EnemyBulletList BulletsRecovery( );
    protected:
        bool IsJumping( );
        // 地面の中にいるかどうか。
        bool IsUnderGround( );
        // スクリーンの中にいる時はtrueが返ってきます。
        bool IsInTheScreen( cinder::CameraPersp const& camera );
    protected: // 以下 アップデートに記入推奨
        // 地面の中にいたら地上に引き戻します。
        void CollideGround( );
        // 生きているかを確認します。
        void LiveCheck( );
    protected:
        // HPがなくなったら死ぬまでカウントします。
        void Dying( );
        // ダメージを受けた時に体の色を徐々に元通りにします。
        void DamageEffect( );
    protected:
        // 弾を作成します。作成した弾はエネミーに一時的に保存されます。
        template <class Ty>
        void BulletCreate( Ty const& instans );
    protected:
        void CameraSee( cinder::CameraPersp const& camera );
        void Jump( cinder::Vec3f jumpPower );
    };

    template<class Ty>
    inline void EnemyBase::BulletCreate( Ty const& instans )
    {
        bulletList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}