# pragma once

# include "cinder/app/AppNative.h"

# include "User/SceneSystems/SceneTask.h"

using namespace cinder;

class Cinder : public cinder::app::AppNative
{
public:
    void setup( ) override;
    void prepareSettings( cinder::app::AppNative::Settings *settings ) override;
    void shutdown( ) override;
public:
    void resize( ) override;
    void update( ) override;
    void draw( ) override;
public:
    void mouseDown( cinder::app::MouseEvent event ) override;
    void mouseUp( cinder::app::MouseEvent event ) override;
    void mouseWheel( cinder::app::MouseEvent event ) override;
    void mouseMove( cinder::app::MouseEvent event ) override;
    void mouseDrag( cinder::app::MouseEvent event ) override;
    void keyDown( cinder::app::KeyEvent event ) override;
    void keyUp( cinder::app::KeyEvent event ) override;
    void touchesBegan( cinder::app::TouchEvent event ) override;
    void touchesMoved( cinder::app::TouchEvent event ) override;
    void touchesEnded( cinder::app::TouchEvent event ) override;
private:
    User::SceneTask scene;
};
