# include "SceneTutorial.h"
# include "SceneGame.h"
# include "SceneResult.h"

# include "ZKOO.hpp"
# include "cinder/Rand.h"

# include "GlobalData.hpp"
# include "TutorialData.hpp"
# include "../Utilitys/GlobalDraw.h"

namespace User
{
    using namespace cinder;

    SceneTutorial::SceneTutorial( )
    {
        // ユーマヨが管理するものを作成。
        fieldCamera = CameraData::Create( "Camera/FieldWorld.json" );
        enemyCamera = CameraData::Create( "Camera/EnemyWorld.json" );
        fieldManager = std::make_shared<FieldManager>( "JSON/TutorialField.json" );
        enemyManager = std::make_shared<EnemyManager>( enemyCamera->GetCamera( ), fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        effectManager = std::make_shared<EffectManager>( );

        // 野本が管理するものを作成。
        player = Player( 100, 100 );

        // 大ちゃんが管理するものを作成。
        mainbgm = &GData::FindAudio( "BGM/mainbgm0.wav" );
        mainbgm->Looping( true );
        mainbgm->Gain( bgmGain );
        mainbgm->Play( );
		hagurumas.push_back(Haguruma(Vec2f(env.getWindowWidth()*0.05f, env.getWindowHeight()*0.05f),
			Vec2f(300, 300), 1.5f, 0.3f, 5.f, HagurumaType::LEFTHAGURUMA));
		hagurumas.push_back(Haguruma(Vec2f(env.getWindowWidth()*0.95f, env.getWindowHeight()*0.05f),
			Vec2f(300, 300), 1.5f, 0.3f, 5.f, HagurumaType::RIGHTHAGURUMA));
    }
    SceneTutorial::~SceneTutorial( )
    {
        //mainbgm->Stop( );
    }
    void SceneTutorial::resize( )
    {
        enemyCamera->GetCamera( ).setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneTutorial::UpdateDamage( )
    {
        // プレイヤーが何もしていないなら
        if ( player.Command( ) == CommandType::NONE )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( enemyCamera->GetCamera( ) );
            damage += enemyBulletManager->EnemyToPlayerDamage( enemyCamera->GetCamera( ) );
            player.TranseNowHp( -damage );
        }

        // プレイヤーがガード状態なら
        if ( player.Command( ) == CommandType::GUARD )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( player.GuardLine( ), enemyCamera->GetCamera( ) );
            damage += enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), enemyCamera->GetCamera( ) );
            player.TranseNowHp( -damage );
        }

        for ( auto& obj : moveInput.Lines( ) )
        {
            int AP = 0;
            AP += enemyManager->PlayerToEnemyDamage( obj, enemyCamera->GetCamera( ) );
            AP += enemyBulletManager->PlayerToEnemyDamage( obj, enemyCamera->GetCamera( ) );
            player.TranseNowMp( AP );
        }
    }
    void SceneTutorial::UpdateCombo( )
    {
        if ( moveInput.Lines( ).empty( ) ) return;

        int attackSuccessNum = 0;
        for ( auto& obj : moveInput.Lines( ) )
        {
            if ( 0 != enemyManager->PlayerToEnemyAttackCheck( obj, enemyCamera->GetCamera( ) ) )
            {
                attackSuccessNum += 1;
            }
        }

        combo += attackSuccessNum;

        if ( attackSuccessNum == 0 )
        {
            combo = 0;
        }
    }
    void SceneTutorial::UpdateNextStage( )
    {
        switch ( fieldManager->GetFieldNumber( ) )
        {
        case 0:
            tutorialManager.Set( &TutorialManager::攻撃 );
            break;
        case 1:
            tutorialManager.Set( &TutorialManager::防御 );
            break;
        default:
            break;
        }

        switch ( fieldManager->GetFieldNumber( ) )
        {
        case 0:
            if ( tutorialManager.KougekiIsNextStage( ) && enemyManager->IsEmpty( ) )
            {
                tutorialManager.KougekiEnd( );
                fieldManager->End( );
            }
            break;
        case 1:
            if ( tutorialManager.BougyoIsNextStage( ) && enemyManager->IsEmpty( ) )
            {
                tutorialManager.BougyoEnd( );
                break;
            }
            if ( tutorialManager.BougyoIsNextStage( ) && !fieldManager->IsMoveing( ) )
            {
                enemyBulletManager = std::make_shared<EnemyBulletManager>( );
                fieldManager->End( );
                enemyManager->PlayerSpecialAttackToEnemyDamage( 100, enemyCamera->GetCamera( ), SpecialType::NOTSELECTED );
            }
            break;
        default:
            break;
        }

        if ( fieldManager->IsMoveing( ) )
        {
            for ( int i = 0; i < hagurumas.size( ); i++ ) {
                hagurumas[i].setUpdate( UPDATETYPE::STARTUPDATE );
            }
        }
        else
        {
            for ( int i = 0; i < hagurumas.size( ); i++ ) {
                hagurumas[i].setUpdate( UPDATETYPE::ENDUPDATE );
            }
        }

        // ステージの移動が終了したら次のステージへ。
        if ( !fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
        {
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( enemyCamera->GetCamera( ), fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        }
    }
    void SceneTutorial::UpdateCamera( )
    {
        enemyCamera->Update( );
    }
    void SceneTutorial::UpdateHaguruma( )
    {
        for ( int i = 0; i < hagurumas.size( ); i++ ) {
            hagurumas[i].update( );
        }
    }
    void SceneTutorial::UpdateEnemySpawn( )
    {
        if ( enemyManager->IsEmpty( ) )
        {
            auto path = tutorialManager.KougekiGetEnemyCreatePath( );
            if ( path != "" )
            {
                enemyManager = std::make_shared<EnemyManager>( enemyCamera->GetCamera( ), path );
            }
        }
    }
    void SceneTutorial::UpdateGuardSucceed( )
    {
        // ガードに成功したら
        if ( player.Command( ) == CommandType::GUARD )
        {
            if ( 0 < enemyBulletManager->EnemyToPlayerGuardCheck( player.GuardLine( ), enemyCamera->GetCamera( ) ) )
            {
                tutorialManager.BougyoPlayerGuardSucceed( );
            }
        }
    }
    void SceneTutorial::UpdateAllInstans( )
    {
        fieldManager->Update( );

        player.Update( );

        // これを呼ぶことでガードができる。
        if ( fieldManager->GetFieldNumber( ) == 1 ) player.UpdateDeffenceOfTouch( );

        enemyManager->update( enemyCamera->GetCamera( ) );
        enemyBulletManager->BulletRegister( enemyManager->BulletRecovery( ) );
        enemyBulletManager->update( );

        UpdateDamage( );
        UpdateCombo( );
        UpdateEnemySpawn( );
        UpdateGuardSucceed( );

        effectManager->EffectRegister( enemyManager->EffectRecovery( ) );
        effectManager->EffectRegister( enemyBulletManager->EffectRecovery( ) );
        effectManager->Update( );
    }
    void SceneTutorial::update( )
    {
        UpdateCamera( );

        moveInput.Begin( combo );

        // プレイヤーがguard状態の時はmoveInputを無効にします。
        if ( player.Command( ) == GUARD || fieldManager->GetFieldNumber( ) == 1 ) moveInput.InputInvalidation( );

        UpdateAllInstans( );

        UpdateNextStage( );

        UpdateHaguruma( );

        moveInput.End( );
    }
    void SceneTutorial::draw( )
    {
        beginDrawMain( );
        drawMain( );
        endDrawMain( );

        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneTutorial::select( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_g ) )
        {
            create( new SceneGame( ) );
            return;
        }

        if ( fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
        {
            create( new SceneGame( ) );
            return;
        }
    }
    void SceneTutorial::beginDrawMain( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::enable( GL_CULL_FACE );

        gl::enableAlphaBlending( );

        gl::enableDepthWrite( );
        gl::enableDepthRead( );

        gl::setViewport( env.getWindowBounds( ) );
    }
    void SceneTutorial::drawMain( )
    {
        gl::setMatrices( fieldCamera->GetCamera( ) );

        fieldManager->Draw( fieldCamera->GetCamera( ) );

        GlobalDraw::Draw( );

        gl::setMatrices( enemyCamera->GetCamera( ) );

        enemyManager->draw( enemyCamera->GetCamera( ) );
        enemyBulletManager->draw( enemyCamera->GetCamera( ) );

        GlobalDraw::Draw( );
    }
    void SceneTutorial::endDrawMain( )
    {
        gl::popMatrices( );
    }
    void SceneTutorial::beginDrawUI( )
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
    void SceneTutorial::drawUI( )
    {
        player.Draw( );

        moveInput.Draw( );

        enemyManager->drawUI( enemyCamera->GetCamera( ) );

        enemyManager->DrawAttackCircle( enemyCamera->GetCamera( ) );

        enemyBulletManager->DrawBulletCircle( enemyCamera->GetCamera( ) );

        effectManager->Draw( );

        // エネミーのHPバーは表示しません。
        //enemyManager->DrawEnemyHitPoint( );

        tutorialManager.Draw( );

        for ( int i = 0; i < hagurumas.size( ); i++ ) {
            hagurumas[i].draw( );
        }
    }
    void SceneTutorial::endDrawUI( )
    {
        gl::popMatrices( );
    }
}