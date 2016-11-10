# include "SceneTask.h"

# include "SceneCreater.h"

# include "../Scenes/SceneStart.h"

namespace User
{
    void SceneTask::resize( ) { SceneCreater::GetScene( ).resize( ); }

    void SceneTask::update( ) { SceneCreater::GetScene( ).update( ); }

    void SceneTask::draw( ) { SceneCreater::GetScene( ).draw( ); }

    void SceneTask::select( ) { SceneCreater::GetScene( ).select( ); }

    void SceneTask::setup( ) { create( start( ) ); }

    void SceneTask::shutdown( ) { SceneCreater::shutdown( ); }

    void SceneTask::setScene( ) { SceneCreater::setScene( ); }
}