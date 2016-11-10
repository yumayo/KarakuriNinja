# pragma once

# include "Karakuri.h"

namespace User
{
    class KarakuriBase
    {
        Karakuri object;

        // カラクリの方向も用意。
        cinder::Vec3f direction;
        cinder::Quatf quaternion;
    public:
        KarakuriBase( cinder::JsonTree const& params );
    public:
        void update( int count );
        void draw( );
    public:
        cinder::Vec3f Direction( ) { return direction; }
        cinder::Quatf Quaternion( ) { return quaternion; }
        cinder::Matrix44f Rotate( ) { return quaternion.toMatrix44( ); }
    public:
        void Direction( cinder::Vec3f direction ) { this->direction = direction.normalized( ); quaternion = cinder::Quatf( cinder::Vec3f::zAxis( ), this->direction ); }
        void DirectionRotate( cinder::Vec3f axis, float radian ) { direction.rotate( axis, radian ); Direction( direction ); }
    };
}