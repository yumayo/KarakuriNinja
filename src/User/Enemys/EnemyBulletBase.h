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
        // �e���������Ă���Ȃ� true ���Ԃ��ė��܂��B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // �Q�b�^�[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( float length );
        // �e�������Ă������̊m�F
        bool IsActive( );
        // �e����������
        void Erase( );
    protected:
        void ActiveCount( );
        void Move( );
    };
}