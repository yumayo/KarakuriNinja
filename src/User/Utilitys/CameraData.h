# pragma once

# include "cinder/Camera.h"
# include "cinder/Json.h"
# include "cinder/params/Params.h"

namespace User
{
    struct CameraDataEditor
    {
        cinder::Vec3f eyePosition;
        cinder::Vec3f targetPosition;
        float fov;
        cinder::JsonTree jsonData;
        cinder::CameraPersp camera;
        cinder::params::InterfaceGlRef editor;

        CameraDataEditor( );
        ~CameraDataEditor( );
        void Update( );
        void Shake( float range );
        void DrawEditor( );
        cinder::CameraPersp& operator()( ) { return camera; }
    };

    struct CameraData
    {
        cinder::Vec3f eyePosition;
        cinder::Vec3f targetPosition;
        float fov;
        cinder::JsonTree jsonData;
        cinder::CameraPersp camera;

        CameraData( );
        ~CameraData( );
        void Update( );
        void Shake( float range );
        cinder::CameraPersp& operator()( ) { return camera; }
    };
}