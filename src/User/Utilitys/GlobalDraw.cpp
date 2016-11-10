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
        // �����x�̂Ȃ����̂��ɕ`��B
        for ( auto & obj : drawArray ) obj( );

        // �����x�̂�����́AmultiMap�ɂ���Ď����\�[�g����`�悳��܂��B
        for ( auto& obj : drawMapAlphaObject ) obj.second( );

        // �o�^���������܂��B
        drawArray.clear( );
        drawMapAlphaObject.clear( );
    }
}