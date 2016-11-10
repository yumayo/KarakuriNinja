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
        bool isHalfHPSerif;
        bool isDeadSerif;
        Fonts font;
        std::string serif;
        cinder::Vec2f serifDrawPosition;
        std::function<void( )> serifBehavior;
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

    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( const cinder::CameraPersp& camera ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        // 重力をかけます。
        void Gravitate( );
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
        void 死ぬ( cinder::CameraPersp const& camera );
    private:
        void 弾を投げるまでのモーション( cinder::CameraPersp const& camera );
        void 弾を投げる( cinder::CameraPersp const& camera );
        void 弾を投げた後の硬直( cinder::CameraPersp const& camera );
        void 弾をもう一度投げるかの確認( cinder::CameraPersp const& camera );
    private:
        void 出現した時のセリフ( cinder::CameraPersp const& camera );
        bool IsHalfHPSerif( );
        void HPが半分以下になった時のセリフ( cinder::CameraPersp const& camera );
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
        int 弾の上限回数 = 3;
        int 弾カウント;
    };
}