#pragma once

#include "EnemyObject.h"
#include "cinder/Camera.h"

namespace User
{
    class EnemyBulletBase
    {
    protected:
        EnemyObject object;
        int attackPoint;
        int activeTime;
        bool isActive;
    public:
        EnemyBulletBase( cinder::Vec3f position, cinder::Vec3f speed );
        virtual void update( );
        virtual void draw( );
        // ’e‚ª‚ ‚½‚Á‚Ä‚¢‚é‚È‚ç true ‚ª•Ô‚Á‚Ä—ˆ‚Ü‚·B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // ƒQƒbƒ^[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
    public:
        // ˆø” : ‚ ‚½‚Á‚½‚Ì’†S‚©‚ç‚Ì‹——£(³‹K‰»Ï‚İ) : 0.0 ~ 1.0(”¼Œa)
        int Hit( float length );
        // ’e‚ğÁ‚µ‚Ä‚¢‚¢‚©‚ÌŠm”F
        bool IsActive( );
        // ’e‚ğÁ‚·–½—ß
        void Erase( );
    protected:
        void ActiveCount( );
        void Move( );
    };
}