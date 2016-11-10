# pragma once

# include "cinder/Vector.h"
# include "cinder/gl/Texture.h"
# include "cinder/Json.h"

namespace User
{
    class KarakuriObject
    {
        int beginCount; // 開始のフレーム
        int moveCount; // 何フレームで移動を終えるのか
        cinder::Vec3f position;
        cinder::Vec3f endScale;
        cinder::Vec3f scale;
        cinder::Vec3f rotate;
        cinder::gl::Texture* texture;
    public:
        KarakuriObject( cinder::JsonTree const& params );
    public:
        void Move( int count );
    public:
        void bind( ) { texture->bind( ); }
        void unbind( ) { texture->unbind( ); }
        cinder::Vec3f Scale( ) { return scale; }
        cinder::Vec3f Rotate( ) { return rotate; }
        cinder::Vec3f Position( ) { return position; }
    private:
        bool IsMoveKarakuri( int count );
        void EaseScale( float t );
    };
}