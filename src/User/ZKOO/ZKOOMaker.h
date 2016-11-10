
# pragma once

# include <iostream>

# include <vector>
# include <string>

# include "ZKOOHand.h"

//# include "../Networks/Network.h"

namespace User
{
    class ZKOOMaker
    {
        ZKOOMaker( );
        std::vector<ZKOOHand> hands;
        /*Network::UDP::P2P::Recv recv;*/
    public:
        static ZKOOMaker& Geti( );
    public:
        void update( );
        bool IsHandUsing( );
        ZKOOHand* Left( );
        ZKOOHand* Right( );
    private:
        std::vector<ZKOOHand> handMake( std::vector<std::string>& splited );
    private:
        friend class InputZKOO;
    };
}