# pragma once

# include "cinder/TriMesh.h"
# include "cinder/ObjLoader.h"
# include "cinder/Vector.h"
# include "cinder/gl/Texture.h"

namespace User
{
    class ObjRender
    {
        cinder::gl::Texture* texture;
        cinder::TriMesh* mesh;
        cinder::Vec3f position;
        cinder::Vec3f scale;
        cinder::Vec3f rotate;
    public:
        ObjRender( std::string const& texturePath, std::string const& objPath );
        void Position( cinder::Vec3f position )
        {
            this->position = position;
        }
        void Scale( cinder::Vec3f scale )
        {
            this->scale = scale;
        }
        void Rotate( cinder::Vec3f rotate )
        {
            this->rotate = rotate;
        }
        void Draw( );
    };
}