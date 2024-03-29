#pragma once

#include "EnemyObject.h"
#include "cinder/Camera.h"

namespace User
{
    class EnemyBulletBase
    {
    protected:
        EnemyObject object;
        cinder::Vec3f begin;
        cinder::Vec3f end;
        int attackPoint;
        int maxMoveTime;
        int moveTime;
        bool isActive;
        int frame = 0;
    public:
        EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end );
        EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end, int attackPoint );
        virtual void update( );
        virtual void draw( );
        // 弾があたっているなら true が返って来ます。
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // ゲッター
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        cinder::Vec3f EndPosition( ) { return end; }
        float NormalizedMoveTime( ) { return static_cast<float>( moveTime ) / static_cast<float>( maxMoveTime ); }
    public:
        // 引数 : あたった時の中心からの距離(正規化済み) : 0.0 ~ 1.0(半径)
        int Hit( float length );
        // 強制的に消滅させます。
        int Kill( );
        // 弾を消していいかの確認
        bool IsActive( );
        // 弾を消す命令
        void Erase( );
    protected:
        void MoveCount( );
        void Move( float t );
        
    };
}