#include "KarakuriObject.h"

#include "cinder/gl/gl.h"

#include "../Utilitys/Hirasawa.h"

#include "../Utilitys/Yumayo.h"

# include "cinder/ImageIo.h"

# include "cinder/app/App.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    KarakuriObject::KarakuriObject( cinder::JsonTree const & params )
        : beginCount( params.getValueForKey<int>( "beginCount" ) )
        , moveCount( params.getValueForKey<int>( "moveCount" ) == 0 ? 1 : params.getValueForKey<int>( "moveCount" ) )
        , begin( getVec3f( params["begin"] ) )
        , end( getVec3f( params["end"] ) )
        , position( begin )
        , scale( getVec3f( params["scale"] ) )
        , rotate( getVec3f( params["rotate"] ) )
        , texture( &GData::FindTexture( params.getValueForKey<std::string>( "texture" ) ) )
    {

    }
    void KarakuriObject::Move( int count )
    {
        if ( IsMoveKarakuri( count ) )
        {
            // カラクリ動作中なら、位置を動かします。
            EasePosition( static_cast<float>( count - beginCount ) / moveCount );
        }
    }
    bool KarakuriObject::IsMoveKarakuri( int count )
    {
        return beginCount <= count && count <= beginCount + moveCount;
    }
    void KarakuriObject::EasePosition( float t )
    {
        position.x = EasingQuadInOut( t, begin.x, end.x );
        position.y = EasingQuadInOut( t, begin.y, end.y );
        position.z = EasingQuadInOut( t, begin.z, end.z );
    }
}
