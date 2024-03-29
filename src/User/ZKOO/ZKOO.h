
# pragma once

#include "InputZKOO.h"

namespace User
{
    class ZKOO
    {
    private:
        ZKOO( );
    public:
        static ZKOO& Geti( );
    public:
        bool isRecognition( const uint32_t handle, ZKOOHand& hand );
        bool isPush( const uint32_t handle, ZKOOHand & hand );
        bool isPress( const uint32_t handle, ZKOOHand & hand );
        bool isPull( const uint32_t handle, ZKOOHand & hand );
        void flushInput( );
        std::vector<uint32_t> GetHandleIDs( );
        const ZKOOHand& hand( )const;
        void Resumption( ); // ZKOO認識を再開します。
        bool IsActive( ); // ZKOOのアップデートが止まっているかどうか。
        bool IsHandsActive( );
    public:
        void begin( );
        void end( );
    private:
        InputZKOO zkoo;
    };
}