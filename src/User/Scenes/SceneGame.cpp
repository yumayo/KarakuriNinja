# include "SceneGame.h"
# include "SceneResult.h"

# include "ZKOO.hpp"
# include "cinder/Rand.h"

namespace User
{
    using namespace cinder;

    SceneGame::SceneGame( )
    {
        cameraEyePosition = Vec3f( 0, 0.7, -5 );
        camera.lookAt( cameraEyePosition, Vec3f( 0, 0.7, 0 ) );
        camera.setWorldUp( Vec3f( 0, 1, 0 ) );
        camera.setPerspective( 60.0F, env.getWindowAspectRatio( ), 1.0F, 100.0F );

        // ユーマヨが管理するものを作成。
        fieldManager = std::make_shared<FieldManager>( );
        enemyManager = std::make_shared<EnemyManager>( camera, fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        UI = std::make_shared<Interface>( );
        gameClearFrame = 60 * 3;

        player = Player( 100, 100 );
    }
    SceneGame::~SceneGame( )
    {

    }
    void SceneGame::resize( )
    {
        camera.setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneGame::UpdatePlayer( )
    {
        player.AttackPhase( );

        if ( player.Command( ) != CommandType::GUARD )
        {
            player.TranseNowHp( -enemyManager->EnemyToPlayerDamage( camera ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( camera ) );
        }

        if ( player.Command( ) == CommandType::GUARD )
        {
            player.TranseNowHp( -enemyManager->EnemyToPlayerDamage( player.GuardLine( ), camera ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), camera ) );
        }

        if ( player.IsAttack( ) == true )
        {
            player.TranseNowMp( enemyManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.TranseNowMp( enemyBulletManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.ShiftIsAttack( );
        }
    }
    void SceneGame::update( )
    {
        // スペシャルは最初にアップデートします。
        // エネミーがいない場合はスペシャルを発動できないようにします。
        if ( !enemyManager->IsEmpty( ) )
        {
            special.update( player.NowMp( ) == player.MaxMp( ) );
        }

        // プレイヤーのスペシャルエフェクト時にカメラを揺らします。
        if ( special.isEffecting( ) )
        {
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
            player.TranseNowMp( -1 );
        }

        // エフェクト終了時、カメラを元通りにします。
        if ( special.getEffectEnd( ) )
        {
            camera.lookAt( cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }

        // エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
        if ( special.getEffectEnd( ) )
        {
            const float damagevalue = 5.f;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( )*damagevalue ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( ) );
        }

        // スペシャル選択時から、エフェクトが終わるまで全ての動作を止めます。
        if ( !special.getIsSpecial( ) )
        {
            fieldManager->Update( );
            enemyManager->update( camera );
            enemyBulletManager->BulletRegister( enemyManager->BulletsRecovery( ) );
            enemyBulletManager->update( );
            UpdatePlayer( );
        }

        // エネミーが全滅したら、次のステージを準備。
        if ( enemyManager->IsEmpty( ) )
        {
            fieldManager->End( );
        }

        // ステージの移動が終了したら次のステージへ。
        if ( fieldManager->IsChange( ) )
        {
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( camera, fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );
            UI = std::make_shared<Interface>( );
        }

        // ラスボスを倒し終わったら、何らかのアクション後、次のシーンへ。
        if ( enemyManager->IsEmpty( ) && fieldManager->IsLastField( ) )
        {
            gameClearFrame = std::max( gameClearFrame - 1, 0 );
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
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
        if ( gameClearFrame == 0 ) 
            create( new SceneResult( ) );
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
        gl::setMatrices( camera );

        gl::drawCoordinateFrame( );

        fieldManager->Draw( );

        enemyManager->draw( );
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
        // 敵や、バレットの当たり判定領域を描画します。
    #ifdef _DEBUG
        enemyManager->DrawCollisionCircle( camera );
        enemyBulletManager->DrawCollisionCircle( camera );
    #endif

        // 描画順は プレイヤー エネミー UI スペシャルの順にします。
        player.Draw( );

        enemyManager->drawUI( );

        enemyBulletManager->DrawBulletCircle( camera );

        UI->draw( player.NormalizedMp( ), player.NormalizedHp( ) );

        special.draw( );

        // 旧バージョンのZKOOの使い方。
        if ( zkoo.IsHandUsing( ) )
        {
            gl::color( Color( 1, 1, 0 ) );
            if ( zkoo.Left( )->IsTracking( ) )  gl::drawSolidCircle( zkoo.Left( )->Position( ), 100, 50 );
            if ( zkoo.Right( )->IsTracking( ) )  gl::drawSolidCircle( zkoo.Right( )->Position( ), 100, 50 );
        }

        // 新しいZKOOの使い方。
        auto hand = inputzkoo.hand( );
        for ( auto& i : inputzkoo.GetHandleIDs( ) )
        {
            if ( inputzkoo.isPress( i, hand ) )
            {
                gl::color( Color( 1, 1, 1 ) );
                gl::drawSolidCircle( hand.Position( ), 50, 50 );
            }
        }
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }
}