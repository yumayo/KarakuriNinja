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
        EnemyBulletTexture( cinder::Vec3f position, cinder::Vec3f speed, std::string const& path );
        virtual void draw( ) override;
    };
}