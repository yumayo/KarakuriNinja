#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <vector>
#include <functional>

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
    public:
        EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        void drawUI( ) override;
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
    private:
        void 弾を投げる( cinder::CameraPersp const& camera );
        void 弾をもう一度投げるかの確認( cinder::CameraPersp const& camera );
    private:
        void 出現した時のセリフ( cinder::CameraPersp const& camera );
        void HPが半分以下になった時のセリフ( cinder::CameraPersp const& camera );
        void 死ぬ時のセリフ( cinder::CameraPersp const& camera );
    private:
        void ヌルセルフ( );
        void セリフ( );
    private:
        bool IsJumping( );
        // スクリーンの中にいる時はtrueが返ってきます。
        bool IsInTheScreen( cinder::CameraPersp const& camera );
        bool IsHalfHPSerif( );
        bool IsDeadSerif( );
    private:
        void SetFunction( void ( EnemyBoss::* function ) ( cinder::CameraPersp const& camera ) );
    private:
        // セリフ割り込み
        void SerifInterrupt( );
        void SetSerifFunction( std::string const& serif, void ( EnemyBoss::* function ) ( ) );
    private:
        // 画面を中心に回転で決める波状バレット攻撃
        void WavyBulletFiring( float angle, cinder::CameraPersp const& camera );
        // ランダムに決めた二点から中点を求める波状バレット攻撃
        void RandomWavyBulletFiring( cinder::CameraPersp const& camera );
    };
}