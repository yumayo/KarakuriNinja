#pragma once

#include "cinder/gl/Texture.h"
#include "EnemyBulletBase.h"

namespace User
{
    class EnemyBulletTexture : public EnemyBulletBase
    {
    protected:
        cinder::gl::Texture texture;
    public:
        EnemyBulletTexture( cinder::Vec3f begin, cinder::Vec3f end, std::string const& path );
        EnemyBulletTexture( cinder::Vec3f begin, cinder::Vec3f end, std::string const& path, int attackPoint );
        virtual void draw( ) override;
    };
}