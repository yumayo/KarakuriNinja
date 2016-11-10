# pragma once

# include <memory>
# include <string>

# include "cinder/Camera.h"
# include "cinder/gl/Texture.h"

namespace User
{
    class Skydome
    {
        cinder::gl::Texture* domeTexture;
        float scale;
    public:
        Skydome( std::string const& texturePath, float scale );
        void Draw( cinder::CameraPersp const& camera );
    };

    using SkydomeRef = std::shared_ptr<Skydome>;
}