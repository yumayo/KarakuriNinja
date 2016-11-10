#include "GlobalDraw.h"

namespace User
{
    std::vector< std::function<void( )>> GlobalDraw::drawArray;
    std::multimap<float, std::function<void( )>> GlobalDraw::drawMapAlphaObject;

    void GlobalDraw::Insert( std::function<void( )> const & function )
    {
        drawArray.emplace_back( function );
    }
    void GlobalDraw::InsertAlphaObject( std::pair<float, std::function<void( )>> const & pair )
    {
        drawMapAlphaObject.insert( pair );
    }
    void GlobalDraw::Draw( )
    {
        // 透明度のない物体を先に描画。
        for ( auto & obj : drawArray ) obj( );

        // 透明度のあるもは、multiMapによって自動ソートされ描画されます。
        for ( auto& obj : drawMapAlphaObject ) obj.second( );

        // 登録を解除します。
        drawArray.clear( );
        drawMapAlphaObject.clear( );
    }
}