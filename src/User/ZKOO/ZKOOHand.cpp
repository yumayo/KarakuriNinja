# include "ZKOOHand.h"

namespace User
{
    cinder::Vec2d ZKOOHand::Position( )
    {
        return cinder::Vec2d( posx, posy );
    }

    double ZKOOHand::Rotation( )
    {
        return rotation;
    }

    bool ZKOOHand::IsTouching( )
    {
        return isTouching;
    }

    bool ZKOOHand::IsTracking( )
    {
        return isTracking;
    }

    void ZKOOHand::Position( cinder::Vec2d position )
    {
        this->posx = position.x;
        this->posy = position.y;
    }

    void ZKOOHand::Rotation( double rotation )
    {
        this->rotation = rotation;
    }

    void ZKOOHand::IsTouching( bool isTouching )
    {
        this->isTouching = isTouching;
    }

    void ZKOOHand::IsTracking( bool isTracking )
    {
        this->isTracking = isTracking;
    }
}