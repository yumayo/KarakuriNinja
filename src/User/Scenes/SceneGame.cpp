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
        カメラを揺らす = false;
        damageColor = ColorA( 1, 0, 0, 0 );

        player = Player( 100, 100 );
        mainbgm.push_back( Audio( "BGM/mainbgm0.wav" ) );
        mainbgm[0].Looping( true );
        mainbgm[0].Gain( 0.4 );
        mainbgm[0].Play( );
        handtex[0] = loadImage( app::loadAsset( "ZKOO/Left_OpenHand.png" ) );
        handtex[1] = loadImage( app::loadAsset( "ZKOO/Left_CloseHand.png" ) );
        handtex[2] = loadImage( app::loadAsset( "ZKOO/Right_OpenHand.png" ) );
        handtex[3] = loadImage( app::loadAsset( "ZKOO/Right_CloseHand.png" ) );
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
        player.Update( );
        player.AttackPhase( );

        int playerHP = player.NowHp( );

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

        if ( playerHP != player.NowHp( ) )
        {
            カメラを揺らす = true;
            damageColor.a += 0.2;
            timer.On( );
            timer.Advance( 60 );
        }

        if ( player.IsAttack( ) == true )
        {
            player.TranseNowMp( enemyManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.TranseNowMp( enemyBulletManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.ShiftIsAttack( );
        }
    }
    void SceneGame::UpdateColor( )
    {
        damageColor.a = std::max( damageColor.a - 0.01F, 0.0F );
    }
    void SceneGame::UpdateScore( )
    {
        UI->AddScore( enemyManager->ScoreRecovery( ) );
        UI->AddScore( enemyBulletManager->ScoreRecovery( ) );
    }
    void SceneGame::update( )
    {
        inputzkoo.Resumption( );
        if ( !inputzkoo.IsActive( ) ) return;

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

        timer.Update( );

        if ( カメラを揺らす )
        {
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }

        if ( timer.IsAction( ) )
        {
            timer.Off( );
            camera.lookAt( cameraEyePosition, camera.getCenterOfInterestPoint( ) );
            カメラを揺らす = false;
        }

        // エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
        if ( special.getEffectEnd( ) )
        {
            const float damagevalue = 10.0F;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( ) * damagevalue ) );
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
            UpdateColor( );
            UpdateScore( );
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
        if ( player.NowHp( ) <= 0.0F )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }

        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_e ) )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }

        if ( gameClearFrame == 0 )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }
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

    #ifdef _DEBUG
        gl::drawCoordinateFrame( );
    #endif // _DEBUG

        fieldManager->Draw( );

        enemyManager->draw( camera );
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

        gl::color( damageColor );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        // 画面を薄い黒で塗りつぶす。
        if ( !inputzkoo.IsActive( ) )
        {
            gl::color( ColorA( 0, 0, 0, 0.5 ) );
            gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );
            gl::color( Color( 1, 1, 1 ) );
            gl::drawSolidRect( Rectf( Vec2f( 200, 400 ), env.getWindowSize( ) ) );
        }

        // ZKOOの表示
        auto hand = inputzkoo.hand( );
        for ( auto& i : inputzkoo.GetHandleIDs( ) )
        {
            if ( i == 1 )
            {
                if ( inputzkoo.isPress( i, hand ) )
                {
                    gl::color( Color( 1, 1, 1 ) );
                    gl::pushModelView( );
                    gl::translate( hand.Position( ) + Vec2f( -handtex[1].getWidth( ) / 2.0F, -handtex[1].getHeight( ) / 2.0F ) );
                    gl::draw( handtex[1] );
                    gl::popModelView( );
                    //gl::color( Color( 1, 1, 1 ) );
                    //gl::drawSolidCircle( hand.Position( ), 50, 50 );
                }
                else
                if ( inputzkoo.isRecognition( i, hand ) )
                {
                    gl::color( Color( 1, 1, 1 ) );
                    gl::pushModelView( );
                    gl::translate( hand.Position( ) );
                    gl::scale( handtex[0].getSize( ) );
                    handtex[0].enableAndBind( );
                    gl::drawSolidRect( Rectf( Vec2f( -0.5, -0.5 ), Vec2f( 0.5, 0.5 ) ) );
                    handtex[0].disable( );
                    gl::popModelView( );
                    //gl::color( Color( 1, 1, 0 ) );
                    //gl::drawSolidCircle( hand.Position( ), 100, 50 );
                }
                if ( inputzkoo.isPush( i, hand ) )
                {
                    if ( !inputzkoo.IsActive( ) )
                    {
                        if ( inputzkoo.IsHandsActive( ) ) inputzkoo.Resumption( );
                    }
                }
            }
            else
            {
                if ( inputzkoo.isPress( i, hand ) )
                {
                    gl::color( Color( 1, 1, 1 ) );
                    gl::pushModelView( );
                    gl::translate( hand.Position( ) + Vec2f( -handtex[3].getWidth( ) / 2.0F, -handtex[3].getHeight( ) / 2.0F ) );
                    gl::draw( handtex[3] );
                    gl::popModelView( );
                    //gl::color( Color( 1, 1, 1 ) );
                    //gl::drawSolidCircle( hand.Position( ), 50, 50 );
                }
                else
                if ( inputzkoo.isRecognition( i, hand ) )
                {
                    gl::color( Color( 1, 1, 1 ) );
                    gl::pushModelView( );
                    gl::translate( hand.Position( ) + Vec2f( -handtex[2].getWidth( ) / 2.0F, -handtex[2].getHeight( ) / 2.0F ) );
                    gl::draw( handtex[2] );
                    gl::popModelView( );
                    //gl::color( Color( 1, 1, 0 ) );
                    //gl::drawSolidCircle( hand.Position( ), 100, 50 );
                }
                if ( inputzkoo.isPush( i, hand ) )
                {
                    if ( !inputzkoo.IsActive( ) )
                    {
                        if ( inputzkoo.IsHandsActive( ) ) inputzkoo.Resumption( );
                    }
                }
            }
        }
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }
}