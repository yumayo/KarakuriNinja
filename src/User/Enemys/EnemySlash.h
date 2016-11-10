#pragma once

#include "EnemyBase.h"

#include "../Utilitys/Yumayo.h"

#include <functional>

namespace User
{
    class EnemySlash : public EnemyBase
    {
        Timer timer;
        bool isAttack;
        cinder::Vec3f prevMovePosition;
        cinder::Vec3f targetVector;
        std::function<void( cinder::CameraPersp const& )> behavior;
    private:
        cinder::gl::Texture* �ҋ@;
        cinder::gl::Texture* �U�����[�V�����摜;
        cinder::gl::Texture* �U���摜;
        cinder::gl::Texture* ���Ɉړ�;
        cinder::gl::Texture* �E�Ɉړ�;
    private:
        Audio* guard_se;
        float prevAttackHP = 0.0F;
    public:
        EnemySlash( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        // �d�͂������܂��B
        void Gravitate( );
    private:
        void �^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera );
        void �J�����֋߂Â�( cinder::CameraPersp const& camera );
        void �U�����[�V����( cinder::CameraPersp const& camera );
        void �U��( cinder::CameraPersp const& camera );
        void �U����d��( cinder::CameraPersp const& camera );
        void �W�����v�Ŗ߂�( cinder::CameraPersp const& camera );
        void ���n( cinder::CameraPersp const& camera );
    private:
        void ���ֈړ�( cinder::CameraPersp const& camera );
        void �E�ֈړ�( cinder::CameraPersp const& camera );
    private:
        bool IsJumping( );
    private:
        void SetFunction( void ( EnemySlash::* function ) ( cinder::CameraPersp const& camera ) );
    };
}