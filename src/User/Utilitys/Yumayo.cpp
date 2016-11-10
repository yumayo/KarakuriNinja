#include "Yumayo.h"

namespace User
{
    using namespace cinder;

    cinder::Vec3f getVec3f( cinder::JsonTree const & params )
    {
        Vec3f v;

        v.x = params.getValueAtIndex<float>( 0 );
        v.y = params.getValueAtIndex<float>( 1 );
        v.z = params.getValueAtIndex<float>( 2 );

        return v;
    }
    cinder::Color getColor( cinder::JsonTree const & params )
    {
        Color c;

        c.r = params.getValueAtIndex<float>( 0 );
        c.g = params.getValueAtIndex<float>( 1 );
        c.b = params.getValueAtIndex<float>( 2 );

        return c;
    }
    cinder::Color getColorA( cinder::JsonTree const & params )
    {
        ColorA c;

        c.r = params.getValueAtIndex<float>( 0 );
        c.g = params.getValueAtIndex<float>( 1 );
        c.b = params.getValueAtIndex<float>( 2 );
        c.a = params.getValueAtIndex<float>( 3 );

        return c;
    }
}