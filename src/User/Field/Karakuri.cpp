#include "Karakuri.h"

#include "cinder/gl/gl.h"

#include "../Utilitys/Hirasawa.h"

#include "../Utilitys/Yumayo.h"

# include "cinder/ImageIo.h"

# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    Karakuri::Karakuri( cinder::JsonTree const & params )
        : begin( getVec3f( params["begin"] ) )
        , end( getVec3f( params["end"] ) )
        , position( begin )
        , beginCount( params.getValueForKey<float>( "beginCount" ) )
        , moveCount( params.getValueForKey<float>( "moveCount" ) )
        , size( getVec3f( params["size"] ) )
        , texture( loadImage( app::loadAsset( params.getValueForKey<std::string>( "path" ) ) ) )
    {

    }
    void Karakuri::Move( int count )
    {
        if ( IsMoveKarakuri( count ) )
        {
            // カラクリ動作中なら、位置を動かします。
            EasePosition( static_cast<float>( count - beginCount ) / moveCount );
        }
    }
    bool Karakuri::IsMoveKarakuri( int count )
    {
        return beginCount <= count && count <= beginCount + moveCount;
    }
    void Karakuri::EasePosition( float t )
    {
        position.x = EasingQuadInOut( t, begin.x, end.x );
        position.y = EasingQuadInOut( t, begin.y, end.y );
        position.z = EasingQuadInOut( t, begin.z, end.z );
    }
}
