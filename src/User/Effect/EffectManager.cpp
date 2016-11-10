#include "EffectManager.h"

# include <algorithm>

namespace User
{
    EffectManager::EffectManager( )
    {
    }
    void EffectManager::Update( )
    {
        Each( [ ] ( EffectBaseRef& effectBase ) { effectBase->Update( ); } );
    }
    void EffectManager::Draw( )
    {
        Each( [ ] ( EffectBaseRef& effectBase ) { effectBase->Draw( ); } );
    }
    void EffectManager::Each( std::function<void( EffectBaseRef& )> function )
    {
        std::for_each( effectList.begin( ), effectList.end( ), function );
    }
}