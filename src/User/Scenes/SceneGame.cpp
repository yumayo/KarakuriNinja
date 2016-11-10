# include "ZKOO.hpp"
#include "SceneGame.h"
#include "cinder/Rand.h"

namespace User
{
    using namespace cinder;

    SceneGame::SceneGame( )
    {
        cameraEyePosition = Vec3f( 0, 0.5, -5 );
        cameraPersp.lookAt( cameraEyePosition, Vec3f( 0, 0.5, 0 ) );
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
        // プレイヤーのスペシャルエフェクト時にカメラを揺らします。
        if ( player.getisEffecting( ) )
        {
            cameraPersp.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, cameraPersp.getCenterOfInterestPoint( ) );
        }
        // それが終わったら、カメラを元通りにします。
        if ( player.getisEffetEnd( ) )
        {
            cameraPersp.lookAt( cameraEyePosition, cameraPersp.getCenterOfInterestPoint( ) );
        }

        // カラクリアップデート
        field->update( );

        // スペシャル選択時から、エフェクトが終わるまでエネミーの動作を止めます。
        if ( !player.getisSpecial( ) )
        {
            enemyController->update( );
            enemyBulletManager->BulletRegister( enemyController->BulletsRecovery( ) );
            enemyBulletManager->update( );
        }

        // エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
        if ( player.getisEffetEnd( ) )
        {
            player.TranseNowMp( enemyController->PlayerSpecialAttackToEnemyDamage( 5.0F ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( ) );
        }

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
    #ifdef _DEBUG
        enemyController->DrawCollisionCircle( cameraPersp );
        enemyBulletManager->DrawCollisionCircle( cameraPersp );
    #endif

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

        if ( !player.getisMinigame( ) )
        {
            UI.draw( player.NormalizedMp( ), player.NormalizedHp( ) );
        }
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }
}