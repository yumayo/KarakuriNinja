# pragma once

# include "EffectBase.h"

# include <list>
# include <functional>

namespace User
{
    using EffectList = std::list<EffectBaseRef>;

    class EffectManager : public EffectActor
    {
        EffectList effectList;
    public:
        EffectManager( );
        void Update( ) override;
        void Draw( ) override;
    private:
        void Each( std::function<void( EffectBaseRef& )> function );
    public:
        // 弾を作成します。作成した弾はエネミーに一時的に保存されます。
        template <class Ty>
        void Create( Ty const& instans );
    };

    template<class Ty>
    inline void EffectManager::Create( Ty const& instans )
    {
        effectList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}