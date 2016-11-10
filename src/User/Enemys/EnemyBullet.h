#pragma once

#include "EnemyBase.h"

# include "../Utilitys/Yumayo.h"
#include"../Utilitys/Audio.h"
namespace User
{
    class EnemyBullet : public EnemyBase
    {
        Timer timer;
        std::function<void( cinder::CameraPersp const& camera )> behavior;
    private:
        std::shared_ptr<cinder::gl::Texture> �ҋ@;
        std::shared_ptr<cinder::gl::Texture> �U�����[�V�����摜;
        std::shared_ptr<cinder::gl::Texture> �U���摜;
        std::shared_ptr<cinder::gl::Texture> ���Ɉړ�;
        std::shared_ptr<cinder::gl::Texture> �E�Ɉړ�;
    public:
        EnemyBullet( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& fieldName );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        void �^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera );
        void �U�����[�V����( cinder::CameraPersp const& camera );
        void �U��( cinder::CameraPersp const& camera );
        void �U����d��( cinder::CameraPersp const& camera );
        void ���ֈړ�( cinder::CameraPersp const& camera );
        void �E�ֈړ�( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBullet::* function ) ( cinder::CameraPersp const& camera ) );
        std::vector<Audio> se;
    };
}