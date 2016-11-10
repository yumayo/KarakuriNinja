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
        // �e���������Ă���Ȃ� true ���Ԃ��ė��܂��B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // �Q�b�^�[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        cinder::Vec3f EndPosition( ) { return end; }
        float NormalizedMoveTime( ) { return static_cast<float>( moveTime ) / static_cast<float>( maxMoveTime ); }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( float length );
        // �����I�ɏ��ł����܂��B
        int Kill( );
        // �e�������Ă������̊m�F
        bool IsActive( );
        // �e����������
        void Erase( );
    protected:
        void MoveCount( );
        void Move( float t );
        
    };
}