# pragma once

# include "EffectActor.hpp"

# include <memory>

namespace User
{
    class EffectBase : public EffectActor
    {
    public:
        EffectBase( );
    public:
        virtual void Update( ) override;
        virtual void Draw( ) override;
    };

    using EffectBaseRef = std::shared_ptr<EffectBase>;
}