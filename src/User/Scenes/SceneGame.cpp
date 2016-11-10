# include "ZKOO.hpp"
#include "SceneGame.h"

namespace User
{
    using namespace cinder;

    SceneGame::SceneGame( )
    {
        cameraPersp.lookAt( Vec3f( 0, 0.5, -5 ), Vec3f( 0, 0.5, 0 ) );
        cameraPersp.setWorldUp( Vec3f( 0, 1, 0 ) );
        cameraPersp.setPerspective( 60.0F, env.getWindowAspectRatio( ), 1.0F, 100.0F );

        enemyController = std::make_shared<EnemyController>( cameraPersp );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );

        field = std::make_shared<Field>( "field_1.json" );

        player = Player( 100, 100 );
    }
    SceneGame::~SceneGame( )
    {

    }
    void SceneGame::resize( )
    {
        cameraPersp.setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneGame::UpdatePlayer( )
    {
        player.AttackPhase( );

        if ( player.Command( ) != CommandType::GUARD )
        {
            player.TranseNowHp( -enemyController->EnemyToPlayerDamage( cameraPersp ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( cameraPersp ) );
            enemyController->ResetPosition( cameraPersp );
        }

        if ( player.Command( ) == CommandType::GUARD )
        {
            player.TranseNowHp( -enemyController->EnemyToPlayerDamage( player.GuardLine( ), cameraPersp ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), cameraPersp ) );
            enemyController->ResetPosition( cameraPersp );
        }

        if ( player.IsAttack( ) == true )
        {
            player.TranseNowMp( enemyController->PlayerToEnemyDamage( player.MakeLine( ), cameraPersp ) );
            player.TranseNowMp( enemyBulletManager->PlayerToEnemyDamage( player.MakeLine( ), cameraPersp ) );
            player.ShiftIsAttack( );
        }
    }
    void SceneGame::update( )
    {
        field->update( );

        enemyController->update( );
        enemyBulletManager->BulletRegister( enemyController->BulletsRecovery( ) );
        enemyBulletManager->update( );

        UpdatePlayer( );
        auto touch = inputs.touch( );
        for ( auto id : inputs.GetTouchHandleIDs( ) )
        {
            if ( inputs.isPushTouch( id, touch ) )
                CONSOLE << id << std::endl;
        }

    }
    void SceneGame::draw( )
    {
        beginDrawMain( );
        drawMain( );
        endDrawMain( );

        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneGame::select( )
    {

    }
    void SceneGame::beginDrawMain( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::enable( GL_CULL_FACE );

        gl::enableAlphaBlending( );

        gl::enableDepthWrite( );
        gl::enableDepthRead( );

        //gl::enable( GL_NORMALIZE );
        //gl::enable( GL_LIGHTING );
        //glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Color::black( ) );
        //glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

        gl::setViewport( env.getWindowBounds( ) );
    }
    void SceneGame::drawMain( )
    {
        gl::setMatrices( cameraPersp );

        gl::drawCoordinateFrame( );

        field->draw( );

        enemyController->draw( );
        enemyBulletManager->draw( );
    }
    void SceneGame::endDrawMain( )
    {
        gl::popMatrices( );
    }
    void SceneGame::beginDrawUI( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::disable( GL_CULL_FACE );
        gl::disable( GL_NORMALIZE );
        gl::disable( GL_LIGHTING );
        gl::enableAlphaBlending( );
        gl::disableDepthRead( );
        gl::disableDepthWrite( );

        gl::setViewport( env.getWindowBounds( ) );
        gl::setMatricesWindow( env.getWindowSize( ) );
        gl::color( Color::white( ) );
    }
    void SceneGame::drawUI( )
    {
        player.Draw( );



        if ( zkoo.IsHandUsing( ) )
        {
            gl::color( Color( 1, 1, 0 ) );
            if ( zkoo.Left( )->IsTracking( ) )  gl::drawSolidCircle( zkoo.Left( )->Position( ), 100, 50 );
            if ( zkoo.Right( )->IsTracking( ) )  gl::drawSolidCircle( zkoo.Right( )->Position( ), 100, 50 );
        }

        auto hand = inputzkoo.hand( );
        for ( auto& i : inputzkoo.GetHandleIDs( ) )
        {
            if ( inputzkoo.isPress( i, hand ) )
            {
                gl::color( Color( 1, 1, 1 ) );
                gl::drawSolidCircle( hand.Position( ), 50, 50 );
            }
        }

        UI.draw( player.NormalizedMp( ), player.NormalizedHp( ) );
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }
}