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
    public:
        EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end );
        EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end, int attackPoint );
        virtual void update( );
        virtual void draw( );
        // ’e‚ª‚ ‚½‚Á‚Ä‚¢‚é‚È‚ç true ‚ª•Ô‚Á‚Ä—ˆ‚Ü‚·B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // ƒQƒbƒ^[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        cinder::Vec3f EndPosition( ) { return end; }
        float NormalizedMoveTime( ) { return static_cast<float>( moveTime ) / static_cast<float>( maxMoveTime ); }
    public:
        // ˆø” : ‚ ‚½‚Á‚½‚Ì’†S‚©‚ç‚Ì‹——£(³‹K‰»Ï‚İ) : 0.0 ~ 1.0(”¼Œa)
        int Hit( float length );
        // ‹­§“I‚ÉÁ–Å‚³‚¹‚Ü‚·B
        int Kill( );
        // ’e‚ğÁ‚µ‚Ä‚¢‚¢‚©‚ÌŠm”F
        bool IsActive( );
        // ’e‚ğÁ‚·–½—ß
        void Erase( );
    protected:
        void MoveCount( );
        void Move( float t );
        
    };
}