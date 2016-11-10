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
        , position( getVec3f( params["position"] ) )
        , scale( Vec3f::zero( ) )
        , endScale( getVec3f( params["endScale"] ) )
        , rotate( getVec3f( params["rotate"] ) )
        , texture( &GData::FindTexture( params.getValueForKey<std::string>( "texture" ) ) )
    {

    }
    void KarakuriObject::Move( int count )
    {
        if ( IsMoveKarakuri( count ) )
        {
            EaseScale( static_cast<float>( count - beginCount ) / moveCount );
        }
    }
    bool KarakuriObject::IsMoveKarakuri( int count )
    {
        return beginCount <= count && count <= beginCount + moveCount;
    }
    void KarakuriObject::EaseScale( float t )
    {
        scale.x = endScale.x;
        scale.y = EasingElasticOut( t, 0, endScale.y );
        scale.z = endScale.z;
    }
}
