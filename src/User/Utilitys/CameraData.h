# pragma once

# include "cinder/Camera.h"
# include "cinder/Json.h"
# include "cinder/params/Params.h"

namespace User
{
    class CameraDataEditor;
    using CameraDataEditorRef = std::shared_ptr<CameraDataEditor>;

    class CameraData;
    using CameraDataRef = std::shared_ptr<CameraData>;

    class CameraDataEditor
    {
    private:
        cinder::Vec3f eyePosition;
        cinder::Vec3f targetPosition;
        float fov;
        std::string jsonPath;
        cinder::JsonTree jsonData;
        cinder::CameraPersp camera;
        cinder::params::InterfaceGlRef editor;

    public:
        static CameraDataEditorRef Create( std::string const& jsonPath );
        CameraDataEditor( std::string const& jsonPath );
        ~CameraDataEditor( );
        void Update( );
        void Shake( float range );
        void DrawEditor( );
        cinder::CameraPersp& GetCamera( ) { return camera; }
    };

    class CameraData
    {
    private:
        cinder::Vec3f eyePosition;
        cinder::Vec3f targetPosition;
        float fov;
        std::string jsonPath;
        cinder::JsonTree jsonData;
        cinder::CameraPersp camera;

    public:
        static CameraDataRef Create( std::string const& jsonPath );
        CameraData( std::string const& jsonPath );
        ~CameraData( );
        void Update( );
        void Shake( float range );
        cinder::CameraPersp& GetCamera( ) { return camera; }
    };


}