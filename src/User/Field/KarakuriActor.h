# pragma once

# include "KarakuriObject.h"

namespace User
{
    class KarakuriActor
    {
        KarakuriObject object;
    public:
        KarakuriActor( cinder::JsonTree const& params );
        virtual void Update( int count );
        virtual void Draw( ) = 0;
    public:
        void bind( ) { object.bind( ); }
        void unbind( ) { object.unbind( ); }
        cinder::Vec3f Scale( ) { return object.Scale( ); }
        cinder::Vec3f Rotate( ) { return object.Rotate( ); }
        cinder::Vec3f Position( ) { return object.Position( ); }
    };
}