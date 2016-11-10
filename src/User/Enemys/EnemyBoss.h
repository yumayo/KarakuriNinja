#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <vector>
#include <functional>
#include"../Utilitys/Audio.h"
namespace User
{
    class EnemyBoss : public EnemyBase
    {
        Timer timer;
        bool isAttack;
        cinder::Vec3f moveSpeed;
        cinder::Vec3f moveLeftRightSpeed;
        bool isBulletFiring;
        bool isDeadStop;
        std::function<void( cinder::CameraPersp const& )> behavior;
    private:
        Fonts font;
        cinder::gl::Texture* messageBoxLeft;
        cinder::gl::Texture* messageBoxCenter;
        cinder::gl::Texture* messageBoxRight;
        std::string serif;
        std::function<void( )> serifBehavior;
    private:
        std::list<EnemyObject> objects;
        cinder::Vec3f prevPosition = cinder::Vec3f::zero( );
    private:
        cinder::JsonTree jsonEnemys;
        cinder::JsonTree::Iter jsonItr;
    private:
        cinder::gl::Texture* 待機;
        cinder::gl::Texture* 投げるモーション画像;
        cinder::gl::Texture* 投げる画像;
        cinder::gl::Texture* 攻撃モーション画像;
        cinder::gl::Texture* 攻撃画像;
        cinder::gl::Texture* 左に移動;
        cinder::gl::Texture* 右に移動;
        cinder::gl::Texture* 倒れかけるモーション;
        cinder::gl::Texture* 倒れるモーション;
    private:
        cinder::gl::Texture* オーラ1;
        cinder::gl::Texture* オーラ2;
        cinder::gl::Texture* auraTex;
        void DrawAura( );
    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( const cinder::CameraPersp& camera ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
        bool IsLive( ) override;
    private:
        // 重力をかけます。
        void Gravitate( );
        void DrawAfterimage( EnemyObject& object, int index );
    private:
        void タイマーが鳴るまで待機( cinder::CameraPersp const& camera );
        void どっちの攻撃を出すかの確認( cinder::CameraPersp const& camera );
    private:
        void 左右に高速移動しながらカメラへ近づく( cinder::CameraPersp const& camera );
        void 攻撃モーション( cinder::CameraPersp const& camera );
        void 攻撃( cinder::CameraPersp const& camera );
        void 攻撃後硬直( cinder::CameraPersp const& camera );
        void ジャンプで戻る( cinder::CameraPersp const& camera );
        void 着地( cinder::CameraPersp const& camera );
        void 死ぬ時のモーション( cinder::CameraPersp const& camera );
        void 死ぬ( cinder::CameraPersp const& camera );
    private:
        void 弾を投げるまでのモーション( cinder::CameraPersp const& camera );
        void 弾を投げる( cinder::CameraPersp const& camera );
        void 弾を投げた後の硬直( cinder::CameraPersp const& camera );
        void 弾をもう一度投げるかの確認( cinder::CameraPersp const& camera );
    private:
        void ボス出現のエフェクトが終了するまで待機( cinder::CameraPersp const& camera );
        void 出現した時にエネミーをスポーンさせる( cinder::CameraPersp const& camera );
        bool isFrieldKilledSerif = false;
        bool IsFrieldKilledSerif( );
        void 部下を倒された時のセリフ( cinder::CameraPersp const& camera );
        bool isDeadSerif = false;
        bool IsDeadSerif( );
        void 死ぬ時のセリフ( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBoss::* function ) ( cinder::CameraPersp const& camera ) );
    private:
        void ヌルセルフ( );
        void セリフ( );
    private:
        void SetSerifFunction( std::string const& serif, void ( EnemyBoss::* function ) ( ) );
    private:
        // 画面を中心に回転で決める波状バレット攻撃
        void WavyBulletFiring( float angle, cinder::CameraPersp const& camera );
        // ランダムに決めた二点から中点を求める波状バレット攻撃
        void RandomWavyBulletFiring( cinder::CameraPersp const& camera );
        std::vector<Audio*> se;
        int 弾を投げる回数 = 0;
        int 弾を投げる上限回数 = 2;
        int 弾の上限回数 = 2;
        int 弾カウント = 0;
        int 直接攻撃の上限回数 = 2;
        int 直接攻撃カウント = 0;
    };
}