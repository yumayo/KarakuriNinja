# pragma once

# include "KarakuriActor.h"

namespace User
{
    class KarakuriRectObj : public KarakuriActor
    {
    public:
        KarakuriRectObj( cinder::JsonTree const& params );
    public:
        virtual void Update( int count ) override;
        virtual void Draw( ) override;
    };
}