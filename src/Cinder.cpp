# include "Cinder.h"

# include "Lib/InputTask.h"
# include "Framework.hpp"

void Cinder::setup( )
{
    Lib::InputTask::Geti( ).setup( );

    Lib::Environment::Geti( ).setup( this );

    scene.setup( );

    scene.setScene( );

    setFullScreen( true );
}

void Cinder::prepareSettings( cinder::app::AppNative::Settings *settings )
{
    settings->enableMultiTouch( );
}

void Cinder::shutdown( )
{
    Lib::InputTask::Geti( ).shutdown( );

    scene.shutdown( );
}

void Cinder::update( )
{
    scene.setScene( );

    // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
    Lib::InputTask::Geti( ).update( );
    Lib::Input::Geti( ).begin( );

    scene.update( );
}

void Cinder::resize( )
{
    scene.resize( );
}

void Cinder::draw( )
{
    gl::clear( );

    scene.draw( );

    scene.select( );

    // Inputのbeginとendで囲まれた部分のみ、Inputの機能を扱えます。
    Lib::Input::Geti( ).end( );
}

void Cinder::mouseDown( cinder::app::MouseEvent event )
{
    Lib::InputTask::Geti( ).mouseDown( event );
}

void Cinder::mouseUp( cinder::app::MouseEvent event )
{
    Lib::InputTask::Geti( ).mouseUp( event );
}

void Cinder::mouseWheel( cinder::app::MouseEvent event )
{
    Lib::InputTask::Geti( ).mouseWheel( event );
}

void Cinder::mouseMove( cinder::app::MouseEvent event )
{
    Lib::InputTask::Geti( ).mouseMove( event );
}

void Cinder::mouseDrag( cinder::app::MouseEvent event )
{
    Lib::InputTask::Geti( ).mouseDrag( event );
}

void Cinder::keyDown( cinder::app::KeyEvent event )
{
    Lib::InputTask::Geti( ).keyDown( event );
}

void Cinder::keyUp( cinder::app::KeyEvent event )
{
    Lib::InputTask::Geti( ).keyUp( event );
}

void Cinder::touchesBegan( cinder::app::TouchEvent event )
{
    Lib::InputTask::Geti( ).touchesBegan( event );
}

void Cinder::touchesMoved( cinder::app::TouchEvent event )
{
    Lib::InputTask::Geti( ).touchesMoved( event );
}

void Cinder::touchesEnded( cinder::app::TouchEvent event )
{
    Lib::InputTask::Geti( ).touchesEnded( event );
}
