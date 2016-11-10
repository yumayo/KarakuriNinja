#pragma once

#include <algorithm>

namespace User
{
    class AttackTime
    {
        bool isAttackMotion;
        int attackFrame;
        int maxAttackFrame;
    public:
        AttackTime( )
            : isAttackMotion( false )
        {
            AttackFrame( 0 );
        }
        void Update( )
        {
            attackFrame = std::min( attackFrame + 1, maxAttackFrame );
            if ( attackFrame == maxAttackFrame ) isAttackMotion = false;
        }
        void AttackFrame( int attackFrame )
        {
            if ( attackFrame <= 0 ) // 0œŽZ‰ñ”ð
            {
                isAttackMotion = false;
                this->attackFrame = 1;
                maxAttackFrame = 1;
            }
            else
            {
                isAttackMotion = true;
                this->attackFrame = 0;
                maxAttackFrame = attackFrame;
            }
        }
        bool IsAttackMotion( ) { return isAttackMotion; }
        bool IsAttackOneFramePrev( ) { return maxAttackFrame == attackFrame + 1; }
        float NormalizedAttackFrame( ) { return static_cast<float>( attackFrame ) / maxAttackFrame; }
    };
}