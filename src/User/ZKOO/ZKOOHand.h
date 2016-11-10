
# pragma once

# include "cinder/Vector.h"

namespace User
{
    class ZKOOHand
    {
        double posx;
        double posy;
        double rotation;
        bool isTouching;
        bool isTracking;
    public:
        cinder::Vec2d Position( );
        double Rotation( );
        bool IsTouching( );
        bool IsTracking( );
    public:
        void Position( cinder::Vec2d position );
        void Rotation( double rotation );
        void IsTouching( bool isTouching );
        void IsTracking( bool isTracking );
    };
}