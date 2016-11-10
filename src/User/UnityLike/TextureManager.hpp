# pragma once

# include <map>
# include <string>
# include <vector>

# include "cinder/gl/Texture.h"

namespace User
{
    class TextureManager
    {
    private:
        static TextureManager& Geti( );
    private:
        TextureManager( );
    public:
        static const cinder::gl::Texture& find( const std::string& asssetPath );
    private:
        std::map<std::string, std::vector<std::string>> csvData;
        std::map<std::string, cinder::gl::Texture> textureData;
    };
}