#include "EffectManager.h"

# include <algorithm>

# include "Framework.hpp"

namespace User
{
    using namespace cinder;

    EffectManager::EffectManager( )
    {

    }
    void EffectManager::Update( )
    {
        Each( [ ] ( EffectBaseRef& effectBase ) { effectBase->Update( ); } );

        EffectEraser( );
    }
    void EffectManager::Draw( )
    {
        std::multimap<float, std::function<void( )>> draw;
        Each( [ & ] ( EffectBaseRef& effectBase )
        {
            draw.insert( std::make_pair( effectBase->GetDepth( ), std::bind( &EffectBase::Draw, effectBase ) ) );
        } );

        for ( auto& obj : draw ) obj.second( );
    }
    void EffectManager::EffectRegister( EffectList & effectList )
    {
        this->effectList.splice( this->effectList.end( ), effectList );
    }
    void EffectManager::EffectEraser( )
    {
        auto eraseList = std::remove_if( effectList.begin( ), effectList.end( ), [ ] ( EffectBaseRef& effectBase ) { return !effectBase->IsActive( ); } );
        effectList.erase( eraseList, effectList.end( ) );
    }
    void EffectManager::Each( std::function<void( EffectBaseRef& )> function )
    {
        std::for_each( effectList.begin( ), effectList.end( ), function );
    }
}