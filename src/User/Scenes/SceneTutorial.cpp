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
        fieldManager = std::make_shared<FieldManager>( "JSON/TutorialField.json" );
        enemyManager = std::make_shared<EnemyManager>( camera( ), fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        effectManager = std::make_shared<EffectManager>( );

        // 野本が管理するものを作成。
        player = Player( 100, 100 );

        // 大ちゃんが管理するものを作成。
        mainbgm = &GData::FindAudio( "BGM/mainbgm0.wav" );
        mainbgm->Looping( true );
        mainbgm->Gain( 0.4 );
        mainbgm->Play( );
        fusuma = std::make_shared<Fusuma>( );
    }
    SceneTutorial::~SceneTutorial( )
    {
        mainbgm->Stop( );
    }
    void SceneTutorial::resize( )
    {
        camera( ).setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneTutorial::UpdateDamage( )
    {
        // プレイヤーが何もしていないなら
        if ( player.Command( ) == CommandType::NONE )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( camera( ) );
            damage += enemyBulletManager->EnemyToPlayerDamage( camera( ) );
            player.TranseNowHp( -damage );
        }

        // プレイヤーがガード状態なら
        if ( player.Command( ) == CommandType::GUARD )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( player.GuardLine( ), camera( ) );
            damage += enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), camera( ) );
            player.TranseNowHp( -damage );
        }

        for ( auto& obj : moveInput.Lines( ) )
        {
            int AP = 0;
            AP += enemyManager->PlayerToEnemyDamage( obj, camera( ) );
            AP += enemyBulletManager->PlayerToEnemyDamage( obj, camera( ) );
            player.TranseNowMp( AP );
        }
    }
    void SceneTutorial::UpdateCombo( )
    {
        if ( moveInput.Lines( ).empty( ) ) return;

        int attackSuccessNum = 0;
        for ( auto& obj : moveInput.Lines( ) )
        {
            if ( 0 != enemyManager->PlayerToEnemyAttackCheck( obj, camera( ) ) )
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
            if ( tutorialManager.KougekiIsNextStage( ) && !fieldManager->IsLastField( ) && enemyManager->IsEmpty( ) )
            {
                fieldManager->End( );
            }
            break;
        case 1:
            if ( tutorialManager.BougyoIsNextStage( ) && !fieldManager->IsLastField( ) && enemyManager->IsEmpty( ) )
            {
                fieldManager->End( );
            }
            break;
        default:
            break;
        }

        // ステージの移動が終了したら次のステージへ。
        if ( !fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
        {
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( camera( ), fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        }
    }
    void SceneTutorial::UpdateCamera( )
    {
        camera.Update( );
    }
    void SceneTutorial::UpdateEnemySpawn( )
    {
        if ( enemyManager->IsEmpty( ) )
        {
            auto path = tutorialManager.KougekiGetEnemyCreatePath( );
            if ( path != "" )
            {
                enemyManager = std::make_shared<EnemyManager>( camera( ), path );
            }
        }
    }
    void SceneTutorial::UpdateGuardSucceed( )
    {
        // ガードに成功したら
        if ( player.Command( ) == CommandType::GUARD && enemyManager->EnemyToPlayerGuardCheck( player.GuardLine( ), camera( ) ) )
        {
            tutorialManager.BougyoPlayerGuardSucceed( );
        }
    }
    void SceneTutorial::UpdateAllInstans( )
    {
        fieldManager->Update( );

        player.Update( );

        // これを呼ぶことでガードができる。
        player.UpdateDeffenceOfTouch( );

        enemyManager->update( camera( ) );
        enemyBulletManager->BulletRegister( enemyManager->BulletRecovery( ) );
        enemyBulletManager->update( );

        UpdateDamage( );
        UpdateCombo( );
        UpdateEnemySpawn( );

        effectManager->EffectRegister( enemyManager->EffectRecovery( ) );
        effectManager->EffectRegister( enemyBulletManager->EffectRecovery( ) );
        effectManager->Update( );
    }
    void SceneTutorial::UpdateTutorialClear( )
    {
        if ( TRData::spawn.IsStopUpdate( ) )
        {
            TRData::spawn.TutorialEnd( );
        }

        if ( TRData::attackCircle.IsStopUpdate( ) )
        {
            TRData::attackCircle.TutorialEnd( );
        }

        if ( TRData::guard.IsStopUpdate( ) )
        {
            TRData::guard.TutorialEnd( );
        }

        if ( TRData::playerAttack.IsStopUpdate( ) )
        {
            TRData::playerAttack.TutorialEnd( );
        }

        if ( TRData::enemyKill.IsStopUpdate( ) )
        {
            TRData::enemyKill.TutorialEnd( );
        }
    }
    void SceneTutorial::update( )
    {
        UpdateCamera( );

        moveInput.Begin( combo );

        // プレイヤーがguard状態の時はmoveInputを無効にします。
        if ( player.Command( ) == GUARD ) moveInput.InputInvalidation( );

        UpdateTutorialClear( );

        UpdateAllInstans( );

        UpdateNextStage( );

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

        if ( /*TIPS*/false )
        {
            //float gain = ( static_cast<float>( sceneChangeFrame ) / maxSceneChangeFrame ) * 0.4F;
            //mainbgm[0]->Gain( gain );
        }

        if ( enemyManager->IsEmpty( ) && fieldManager->IsLastField( ) )
        {
            fusuma->closeFusuma( );

            if ( fusuma->IsMoveFinished( ) )
            {
                create( new SceneGame( ) );
                return;
            }
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
        gl::setMatrices( camera( ) );
    }
    void SceneTutorial::drawMain( )
    {
        fieldManager->Draw( camera( ) );
        enemyManager->draw( camera( ) );
        enemyBulletManager->draw( camera( ) );

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

        enemyManager->drawUI( camera( ) );

        enemyManager->DrawAttackCircle( camera( ) );

        enemyBulletManager->DrawBulletCircle( camera( ) );

        effectManager->Draw( );

        // エネミーのHPバーは表示しません。
        //enemyManager->DrawEnemyHitPoint( );

        tutorialManager.Draw( );

        fusuma->drawFusuma( );
    }
    void SceneTutorial::endDrawUI( )
    {
        gl::popMatrices( );
    }
}