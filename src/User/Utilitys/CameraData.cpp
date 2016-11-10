# include "CameraData.h"

# include "cinder/app/App.h"

# include "Yumayo.h"

# include "Framework.hpp"

namespace User
{
    using namespace cinder;

    CameraDataEditorRef CameraDataEditor::Create( std::string const & jsonPath )
    {
        return std::make_shared<CameraDataEditor>( jsonPath );
    }

    CameraDataEditor::CameraDataEditor( std::string const & jsonPath )
    {
        this->jsonPath = jsonPath;
        jsonData = JsonTree( app::loadAsset( "JSON/" + jsonPath ) );
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

        jsonData.write( env.getAssetPath( "JSON/" + jsonPath ) );
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

    CameraDataRef CameraData::Create( std::string const & jsonPath )
    {
        return std::make_shared<CameraData>( jsonPath );
    }

    CameraData::CameraData( std::string const & jsonPath )
    {
        this->jsonPath = jsonPath;
        jsonData = JsonTree( app::loadAsset( "JSON/" + jsonPath ) );
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