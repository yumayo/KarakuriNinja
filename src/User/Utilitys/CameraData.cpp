# include "CameraData.h"

# include "cinder/app/App.h"

# include "Yumayo.h"

# include "Framework.hpp"

namespace User
{
    using namespace cinder;

    CameraDataEditor::CameraDataEditor( )
    {
        jsonData = JsonTree( app::loadAsset( "JSON/CameraData.json" ) );
        auto& data = jsonData["Camera"];

        eyePosition = getVec3f( data["eyePosition"] );
        targetPosition = getVec3f( data["targetPosition"] );
        fov = getFloat( data["fov"] );

        editor = params::InterfaceGl::create( "Edit Parameter", Vec2i( 400, 600 ) );
        editor->addParam( "eyePosition", &eyePosition ).group( "Camera" );
        editor->addParam( "targetPosition", &targetPosition ).group( "Camera" );
        editor->addParam( "fov", &fov ).group( "Camera" );

        Update( );
    }

    CameraDataEditor::~CameraDataEditor( )
    {
        auto& data = jsonData["Camera"];

        auto& eyePos = data["eyePosition"] = JsonTree::makeArray( "eyePosition" );
        eyePos.pushBack( JsonTree( "", eyePosition.x ) );
        eyePos.pushBack( JsonTree( "", eyePosition.y ) );
        eyePos.pushBack( JsonTree( "", eyePosition.z ) );

        auto& tarPos = data["targetPosition"] = JsonTree::makeArray( "targetPosition" );
        tarPos.pushBack( JsonTree( "", targetPosition.x ) );
        tarPos.pushBack( JsonTree( "", targetPosition.y ) );
        tarPos.pushBack( JsonTree( "", targetPosition.z ) );

        data["fov"] = ci::JsonTree( "fov", fov );

        jsonData.write( env.getAssetPath( "JSON/CameraData.json" ) );
    }

    void CameraDataEditor::Update( )
    {
        camera.lookAt( eyePosition, targetPosition );
        camera.setPerspective( fov, env.getWindowAspectRatio( ), 1.0F, 100.0F );
    }

    void CameraDataEditor::Shake( float range )
    {
        auto shakePower = Vec3f( randFloat( -range, range ), randFloat( -range, range ), 0.0F );
        camera.lookAt( eyePosition + shakePower, targetPosition );
    }

    void CameraDataEditor::DrawEditor( )
    {
        editor->draw( );
    }

    CameraData::CameraData( )
    {
        jsonData = JsonTree( app::loadAsset( "JSON/CameraData.json" ) );
        auto& data = jsonData["Camera"];

        eyePosition = getVec3f( data["eyePosition"] );
        targetPosition = getVec3f( data["targetPosition"] );
        fov = getFloat( data["fov"] );

        Update( );
    }

    CameraData::~CameraData( )
    {

    }

    void CameraData::Update( )
    {
        camera.lookAt( eyePosition, targetPosition );
        camera.setPerspective( fov, env.getWindowAspectRatio( ), 1.0F, 100.0F );
    }

    void CameraData::Shake( float range )
    {
        auto shakePower = Vec3f( randFloat( -range, range ), randFloat( -range, range ), 0.0F );
        camera.lookAt( eyePosition + shakePower, targetPosition );
    }
}