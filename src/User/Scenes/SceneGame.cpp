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

    SceneGame::SceneGame( )
        : mainBGMfadeOut( 100 )
    {
        TRData::Reset( );

        // ユーマヨが管理するものを作成。
        fieldCamera = CameraData::Create( "Camera/FieldWorld.json" );
        enemyCamera = CameraData::Create( "Camera/EnemyWorld.json" );
        fieldManager = std::make_shared<FieldManager>( "JSON/GameMainField.json" );
        enemyManager = std::make_shared<EnemyManager>( enemyCamera->GetCamera( ), fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        effectManager = std::make_shared<EffectManager>( );
        UI = std::make_shared<Interface>( );
        timer.Off( );
        damageColor = ColorA( 1, 0, 0, 0 );
        talk = std::make_shared<Talk>( );
        mojiManager.Setup( "JSON/GameStart.json" );
        production = &GData::FindTexture( "Production.png" );

        // 野本が管理するものを作成。
        player = Player( 100, 100 );

        // 大ちゃんが管理するものを作成。
        mainbgm = &GData::FindAudio( "BGM/mainbgm0.wav" );
        // mainbgmはtutorialSceneで設定してあるので変更はありません。

        bossbgm = &GData::FindAudio( "BGM/lastboss.wav" );
        bossbgm->Looping( true );
        bossbgm->Gain( bgmGain );
        watercount = 0;
        fusuma = std::make_shared<Fusuma>( );
        time = 0;
        ismpmax = false;
        mpmax = &GData::FindAudio( "SE/mpmax.wav" );
        horagai = &GData::FindAudio( "SE/gamestart.wav" );
        horagai->Play( );
        gameClearSE = &GData::FindAudio( "SE/j_26.wav" );
        hagurumas.push_back( Haguruma( Vec2f( env.getWindowWidth( )*0.1f, env.getWindowHeight( )*0.15f ),
                                       Vec2f( 350, 350 ), 1.5f, 0.3f, 5.f, HagurumaType::LEFTHAGURUMA ) );
        hagurumas.push_back( Haguruma( Vec2f( env.getWindowWidth( )*0.9f, env.getWindowHeight( )*0.15f ),
                                       Vec2f( 350, 350 ), 1.5f, 0.3f, 5.f, HagurumaType::RIGHTHAGURUMA ) );

        for ( auto& obj : hagurumas ) obj.set_anglescale_t( 1 );

    }
    SceneGame::~SceneGame( )
    {
        mainbgm->Stop( );
        bossbgm->Stop( );
    }
    void SceneGame::resize( )
    {
        enemyCamera->GetCamera( ).setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneGame::UpdateGameStart( )
    {
        if ( !horagai ) return;
        if ( !horagai->IsPlaying( ) )
        {
            mojiManager.End( );
            horagai = nullptr;
        }
    }

    void SceneGame::UpdateDamage( )
    {
        if ( special.getSpecialType( ) == SpecialType::WATER ) {
            player.TranseNowHp( ( watercount % 60 ) == 0 );
            watercount++;
        }
        else { watercount = 0; }

        int playerHP = player.NowHp( );

        // プレイヤーが何もしていないなら
        if ( player.Command( ) == CommandType::NONE )
        {
            bool 無敵じゃない = ( !( special.getSpecialType( ) == SpecialType::TREE ) );
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( enemyCamera->GetCamera( ) ) * 無敵じゃない;
            damage += enemyBulletManager->EnemyToPlayerDamage( enemyCamera->GetCamera( ) ) * 無敵じゃない;
            player.TranseNowHp( -damage );
        }

        if ( playerHP != player.NowHp( ) )
        {
            damageColor.a = std::min( damageColor.a + 0.2, 0.6 );
            timer.On( );
            timer.Advance( 60 );
            UI->ResetCombo( );
        }

        for ( auto& obj : moveInput.Lines( ) )
        {
            int AP = 0;
            float damagerate = ( special.getSpecialType( ) == SpecialType::FIRE ) ? 1.3f : 1.0f;
            AP += enemyManager->PlayerToEnemyDamage( obj, enemyCamera->GetCamera( ), damagerate, UI->ComboNumber( ) );
            AP += enemyBulletManager->PlayerToEnemyDamage( obj, enemyCamera->GetCamera( ) );
            player.TranseNowMp( AP );
        }
    }
    void SceneGame::UpdateColor( )
    {
        if ( player.NowHp( ) != 0 ) damageColor.a = std::max( damageColor.a - 0.01F, 0.0F );
    }
    void SceneGame::UpdateScore( )
    {
        UI->AddScore( enemyManager->ScoreRecovery( ) );
        UI->AddScore( enemyBulletManager->ScoreRecovery( ) );
    }
    void SceneGame::UpdateCombo( )
    {
        UI->update( player.NowHp( ) );

        if ( moveInput.Lines( ).empty( ) ) return;

        int hitNum = 0;
        for ( auto& obj : moveInput.Lines( ) )
        {
            if ( 0 != enemyManager->PlayerToEnemyAttackCheck( obj, enemyCamera->GetCamera( ) ) )
            {
                hitNum += 1;
            }
        }

        UI->PlusCombo( hitNum );

        if ( hitNum == 0 )
        {
            UI->ResetCombo( );
        }
    }
    void SceneGame::UpdateDamageExpression( )
    {
        timer.Update( );

        if ( timer.IsCount( ) )
        {
            CameraShake( );
        }

        if ( timer.IsAction( ) )
        {
            timer.Off( );
        }
    }
    void SceneGame::UpdateNextStage( )
    {
        // エネミーが全滅したら、次のステージを準備。
        if ( enemyManager->IsEmpty( ) && !fieldManager->IsLastField( ) && !fieldManager->IsMoveing( ) )
        {
            fieldManager->End( );
        }

        // ステージの移動が終了したら次のステージへ。
        if ( !fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
        {
            mojiManager.End( );
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( enemyCamera->GetCamera( ), fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );

            if ( fieldManager->IsLastField( ) )
            {
                演出中 = true;
                player.Reset( );
            }
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
    }
    void SceneGame::UpdateAllInstans( )
    {
        // スペシャル選択時から、エフェクトが終わるまで全ての動作を止めます。
        if ( !special.getIsSpecial( ) )
        {
            fieldManager->Update( );
        }

        if ( !special.getIsSpecial( ) &&
             !TRData::bossSpawn.IsStopUpdate( ) &&
             !TRData::bossSerif.IsStopUpdate( ) &&
             !TRData::special.IsStopUpdate( ) )
        {
            player.Update( );
        }

        if ( !special.getIsSpecial( ) &&
             !TRData::special.IsStopUpdate( ) )
        {
            enemyManager->update( enemyCamera->GetCamera( ) );
        }

        if ( !special.getIsSpecial( ) &&
             !TRData::bossSpawn.IsStopUpdate( ) &&
             !TRData::special.IsStopUpdate( ) )
        {
            enemyBulletManager->BulletRegister( enemyManager->BulletRecovery( ) );
            enemyBulletManager->update( );
        }

        if ( !special.getIsSpecial( ) &&
             !TRData::bossSpawn.IsStopUpdate( ) &&
             !TRData::bossSerif.IsStopUpdate( ) &&
             !TRData::special.IsStopUpdate( ) )
        {
            UpdateDamage( );
            UpdateColor( );
            UpdateScore( );
            UpdateCombo( );
        }

        if ( !special.getIsSpecial( ) )
        {
            effectManager->EffectRegister( enemyManager->EffectRecovery( ) );
            effectManager->EffectRegister( enemyBulletManager->EffectRecovery( ) );
            effectManager->Update( );

            mojiManager.Update( );
            UpdateHaguruma( );
        }

        if ( !fieldManager->IsMoveing( ) ) time += 1;
    }
    void SceneGame::UpdateSpecial( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_s ) )
        {
            special.goSpecialMode( );
        }

        if ( TRData::special.IsStopUpdate( ) )
        {
            if ( !isTalk ) talkTime = time;
            isTalk = true;
            if ( ( ( time - talkTime ) % 120 ) == ( 120 - 1 ) )
            {
                TRData::PopFrontSerif( );
            }
            if ( TRData::IsSerifTalked( ) )
            {
                TRData::bossSpawn.TutorialEnd( );
                TRData::special.TutorialEnd( );
                mpmax->Gain( 1.f );
                mpmax->Play( );
                special.goSpecialMode( );
                演出中 = false;
                isTalk = false;
                talkTime = 0;
            }
        }

        // スペシャルは最初にアップデートします。
        // エネミーがいない場合はスペシャルを発動できないようにします。
        if ( !enemyManager->IsEmpty( ) )
        {
            special.update( player.NowMp( ) == player.MaxMp( ), fieldManager->IsMoveing( ) );///////////////////////////ここ！！
        }

        // プレイヤーのスペシャルエフェクト時にカメラを揺らします。
        if ( special.isEffecting( ) )
        {
            CameraShake( );
            player.TranseNowMp( -1 );
        }

        // エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
        if ( special.getEffectEnd( ) )
        {
            const float damagevalue = 10.0F;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( ) * damagevalue, enemyCamera->GetCamera( ), special.getSpecialType( ), UI->ComboNumber( ) ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( enemyCamera->GetCamera( ) ) );
        }
    }
    void SceneGame::UpdateGameEnd( )
    {
        // ラスボスを倒し終わったら、何らかのアクション後、次のシーンへ。
        if ( enemyManager->IsEmpty( ) && fieldManager->IsLastField( ) && sceneChangeFrame != 0 )
        {
            if ( !isGameClear ) gameClearSE->Play( );
            isGameClear = true;


            float gain = ( static_cast<float>( sceneChangeFrame ) / maxSceneChangeFrame ) * bgmGain;
            bossbgm->Gain( gain );
            mainbgm->Gain( gain );
            sceneChangeFrame = std::max( sceneChangeFrame - 1, 0 );

            CameraShake( );

            mojiManager.ReCall( "JSON/GameClear.json" );
        }
        // 死亡したらゲームオーバーの文字を表示して襖を閉める。
        else if ( player.NowHp( ) <= 0.0F && sceneChangeFrame != 0 )
        {
            isGameClear = false;

            float gain = ( static_cast<float>( sceneChangeFrame ) / maxSceneChangeFrame ) * bgmGain;
            bossbgm->Gain( gain );
            mainbgm->Gain( gain );
            sceneChangeFrame = std::max( sceneChangeFrame - 1, 0 );

            damageColor.a = std::min( damageColor.a + 0.005, 0.6 );

            mojiManager.ReCall( "JSON/GameOver.json" );
        }
    }
    void SceneGame::UpdateBGM( )
    {
        if ( enemyManager->IsMainBGMGainDown( ) )
        {
            float gain = ( 1.0F - mainBGMfadeOut.NormalizedRectSizeFrame( ) ) * bgmGain;
            mainbgm->Gain( gain );
            mainBGMfadeOut.Update( );
        }

        if ( mainBGMfadeOut.IsMax( ) )
        {
            mainbgm->Stop( );
        }

        if ( enemyManager->IsBossBGMStart( ) && !bossbgm->IsPlaying( ) )
        {
            bossbgm->Play( );
        }
    }
    void SceneGame::UpdateHaguruma( )
    {
        for ( int i = 0; i < hagurumas.size( ); i++ ) {
            hagurumas[i].update( );
        }
    }
    void SceneGame::CameraShake( )
    {
        fieldCamera->Shake( 0.02F );
        enemyCamera->Shake( 0.02F );
    }
    void SceneGame::update( )
    {
        frame += 1;

        enemyCamera->Update( );

        if ( !special.getIsSpecial( ) &&
             !TRData::bossSpawn.IsStopUpdate( ) &&
             !TRData::bossSerif.IsStopUpdate( ) &&
             !TRData::special.IsStopUpdate( ) )
        {
            moveInput.Begin( UI->ComboNumber( ) );
            player.Reset( );
        }
        if ( TRData::bossSpawn.IsStopUpdate( ) )
        {
            moveInput.Clear( );
        }

        UpdateGameStart( );

        UpdateSpecial( );

        UpdateDamageExpression( );

        UpdateAllInstans( );

        UpdateNextStage( );

        UpdateGameEnd( );

        UpdateBGM( );

        if ( !special.getIsSpecial( ) ) moveInput.End( );
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
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_r ) )
        {
            create( new SceneResult( UI->Score( ), UI->MaxComboNumber( ), player.NowHp( ), time / 60, false ) );///////ここ！！
            return;
        }

        if ( sceneChangeFrame == 0 )
        {
            mojiManager.End( );

            if ( mojiManager.IsEnded( ) )
            {
                fusuma->closeFusuma( );

                if ( fusuma->IsMoveFinished( ) )
                {
                    create( new SceneResult( UI->Score( ), UI->MaxComboNumber( ), player.NowHp( ), time / 60, isGameClear ) );
                    return;
                }
            }
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
        if ( special.isMinigame( ) ) return;

        gl::setMatrices( fieldCamera->GetCamera( ) );

        fieldManager->Draw( fieldCamera->GetCamera( ) );

        GlobalDraw::Draw( );

        gl::setMatrices( enemyCamera->GetCamera( ) );

        enemyManager->draw( enemyCamera->GetCamera( ) );
        enemyBulletManager->draw( enemyCamera->GetCamera( ) );

        GlobalDraw::Draw( );
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
        if ( !special.isMinigame( ) )
        {
            player.Draw( );

            moveInput.Draw( );

            enemyManager->drawUI( enemyCamera->GetCamera( ) );

            enemyManager->DrawAttackCircle( enemyCamera->GetCamera( ) );

            enemyBulletManager->DrawBulletCircle( enemyCamera->GetCamera( ) );

            effectManager->Draw( );

            enemyManager->drawProduction( );

            //enemyManager->DrawEnemyHitPoint( );

            UI->draw( player.NormalizedMp( ),
                      player.NormalizedHp( ),
                      ( player.NowMp( ) == player.MaxMp( ) ) && ( special.getSpecialType( ) == SpecialType::NOTSELECTED ),
                      int( special.getSpecialType( ) ) );

            talk->Draw( Vec2f( env.getWindowWidth( ) / 2, env.getWindowHeight( ) / 2 ) );
            for ( int i = 0; i < hagurumas.size( ); i++ ) {
                hagurumas[i].draw( );
            }
        }

        special.draw( );

        if ( 演出中 )
        {
            gl::color( ColorA( 1, 1, 1, 0.75F + 0.25F * math<float>::sin( static_cast<float>( frame ) / 15.0F ) ) );
            gl::draw( *production, Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );
            gl::color( Color::white( ) );
        }

        gl::color( damageColor );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        mojiManager.Draw( env.getWindowCenter( ), 220 );

        fusuma->drawFusuma( );
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }

}