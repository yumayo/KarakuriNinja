#pragma once

#include "EnemyBase.h"

# include "../Utilitys/Yumayo.h"
# include"../Utilitys/Audio.h"
namespace User
{
    class EnemyBulletTutorial : public EnemyBase
    {
        Timer timer;
        std::function<void( cinder::CameraPersp const& camera )> behavior;
    private:
        cinder::gl::Texture* ‘Ò‹@;
        cinder::gl::Texture* UŒ‚ƒ‚[ƒVƒ‡ƒ“‰æ‘œ;
        cinder::gl::Texture* UŒ‚‰æ‘œ;
    public:
        EnemyBulletTutorial( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        void update( cinder::CameraPersp const& camera ) override;
        void draw( ) override;
        bool Attack( const cinder::CameraPersp& camera ) override;
    private:
        void ƒ^ƒCƒ}[‚ª–Â‚é‚Ü‚Å‘Ò‹@( cinder::CameraPersp const& camera );
        void UŒ‚ƒ‚[ƒVƒ‡ƒ“( cinder::CameraPersp const& camera );
        void UŒ‚( cinder::CameraPersp const& camera );
        void UŒ‚Œãd’¼( cinder::CameraPersp const& camera );
    private:
        void SetFunction( void ( EnemyBulletTutorial::* function ) ( cinder::CameraPersp const& camera ) );
        std::vector<Audio*> se;
    };
}