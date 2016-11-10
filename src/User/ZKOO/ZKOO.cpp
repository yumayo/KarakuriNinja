#include "ZKOO.h"
#include "ZKOOMaker.h"

namespace User
{
    ZKOO::ZKOO( )
    {
    }
    ZKOO & ZKOO::Geti( )
    {
        static ZKOO instans;
        return instans;
    }
    bool ZKOO::isPush( const uint32_t handle, ZKOOHand & hand )
    {
        return zkoo.isPush( handle, hand );
    }

    bool ZKOO::isPress( const uint32_t handle, ZKOOHand & hand )
    {
        return zkoo.isPress( handle, hand );
    }

    bool ZKOO::isPull( const uint32_t handle, ZKOOHand & hand )
    {
        return zkoo.isPull( handle, hand );
    }

    void ZKOO::flushInput( )
    {
        zkoo.flushInput( );
    }

    void ZKOO::begin( )
    {
        zkoo.update( );
    }

    void ZKOO::end( )
    {
        zkoo.end( );
    }

    std::vector<uint32_t> ZKOO::GetHandleIDs( )
    {
        return zkoo.GetHandleIDs( );
    }
    const ZKOOHand & ZKOO::hand( ) const
    {
        static ZKOOHand hand;
        return hand;
    }
}