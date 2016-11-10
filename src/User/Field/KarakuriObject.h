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
        cinder::Vec3f begin;
        cinder::Vec3f end;
        cinder::Vec3f position;
        cinder::Vec3f size;
        cinder::gl::Texture* texture;
    public:
        KarakuriObject( cinder::JsonTree const& params );
    public:
        void Move( int count );
    private:
        bool IsMoveKarakuri( int count );
        void EasePosition( float t );
    public:
        void bind( ) { texture->bind( ); }
        void unbind( ) { texture->unbind( ); }
        cinder::Vec3f Size( ) { return size; }
        cinder::Vec3f Position( ) { return position; }
    };
}