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
        std::function<void( cinder::CameraPersp const& )> behavior;
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
        bool IsJumping( );
    private:
        void SetFunction( void ( EnemySlash::* function ) ( cinder::CameraPersp const& camera ) );
    };
}