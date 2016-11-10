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
    {
        // ユーマヨが管理するものを作成。
        fieldManager = std::make_shared<FieldManager>( "JSON/GameMainField.json" );
        enemyManager = std::make_shared<EnemyManager>( camera( ), fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        effectManager = std::make_shared<EffectManager>( );
        UI = std::make_shared<Interface>( );
        timer.Off( );
        damageColor = ColorA( 1, 0, 0, 0 );
        talk = std::make_shared<Talk>( );
        description = std::make_shared<Description>( );
        kougeki = &GData::FindTexture( "Textures/kougeki.png" );
        bougyo = &GData::FindTexture( "Textures/bougyo.png" );

        // 野本が管理するものを作成。
        player = Player( 100, 100 );

        // 大ちゃんが管理するものを作成。
        mainbgm.push_back( &GData::FindAudio( "BGM/mainbgm0.wav" ) );
        mainbgm[0]->Looping( true );
        mainbgm[0]->Gain( 0.4 );
        mainbgm[0]->Play( );
        watercount = 0;
        fusuma = std::make_shared<Fusuma>( );
        time = 0;/////////////////////////////////////////////////////////ここ！！
        ismpmax = false;
        mpmax = &GData::FindAudio( "SE/mpmax.wav" );
        horagai = &GData::FindAudio( "SE/gamestart.wav" );
    }
    SceneGame::~SceneGame( )
    {
        mainbgm[0]->Stop( );
    }
    void SceneGame::resize( )
    {
        camera( ).setAspectRatio( env.getWindowAspectRatio( ) );
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
            damage += enemyManager->EnemyToPlayerDamage( camera( ) ) * 無敵じゃない;
            damage += enemyBulletManager->EnemyToPlayerDamage( camera( ) ) * 無敵じゃない;
            player.TranseNowHp( -damage );
        }

        // プレイヤーがガード状態なら
        if ( player.Command( ) == CommandType::GUARD )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( player.GuardLine( ), camera( ) );
            damage += enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), camera( ) );
            player.TranseNowHp( -damage * !( special.getSpecialType( ) == SpecialType::TREE ) );
        }

        if ( playerHP != player.NowHp( ) )
        {
            damageColor.a = std::min( damageColor.a + 0.2, 0.6 );
            timer.On( );
            timer.Advance( 60 );
        }

        for ( auto& obj : moveInput.Lines( ) )
        {
            int AP = 0;
            float damagerate = ( special.getSpecialType( ) == SpecialType::FIRE ) ? 1.3f : 1.0f;
            AP += enemyManager->PlayerToEnemyDamage( obj, camera( ), damagerate, UI->ComboNumber( ) );
            AP += enemyBulletManager->PlayerToEnemyDamage( obj, camera( ) );
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
        UI->update( );

        if ( moveInput.Lines( ).empty( ) ) return;

        bool isHit = false;
        for ( auto& obj : moveInput.Lines( ) )
        {
            if ( 0 != enemyManager->PlayerToEnemyAttackCheck( obj, camera( ) ) )
            {
                isHit = true;
                break;
            }
        }

        UI->PlusCombo( isHit );
    }
    void SceneGame::UpdateDamageExpression( )
    {
        timer.Update( );

        if ( timer.IsCount( ) )
        {
            camera.Shake( 0.02F );
        }

        if ( timer.IsAction( ) )
        {
            timer.Off( );
        }
    }
    void SceneGame::UpdateNextStage( )
    {
        // エネミーが全滅したら、次のステージを準備。
        if ( enemyManager->IsEmpty( ) && !fieldManager->IsLastField( ) && TRData::enemyKill.IsComplete( ) )
        {
            fieldManager->End( );
        }

        // ステージの移動が終了したら次のステージへ。
        if ( !fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
        {
            mojiManager.End( );
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( camera( ), fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        }
    }
    void SceneGame::UpdateAllInstans( )
    {
        // スペシャル選択時から、エフェクトが終わるまで全ての動作を止めます。
        if ( !special.getIsSpecial( ) )
        {
            fieldManager->Update( );

            if ( !TRData::IsStopUpdate( ) )
            {
                player.Update( );
                player.UpdateDeffenceOfTouch( );

                enemyManager->update( camera( ) );
                enemyBulletManager->BulletRegister( enemyManager->BulletRecovery( ) );
                enemyBulletManager->update( );

                UpdateDamage( );
                UpdateColor( );
                UpdateScore( );
                UpdateCombo( );
            }

            // エフェクトは常に動きます。
            effectManager->EffectRegister( enemyManager->EffectRecovery( ) );
            effectManager->EffectRegister( enemyBulletManager->EffectRecovery( ) );
            effectManager->Update( );
        }
        /////////////////////////////////ここ！！
        if ( ( TRData::enemyKill.IsComplete( ) ) && ( !special.getIsSpecial( ) ) && ( !fieldManager->IsMoveing( ) ) ) {
            time++;
        }
  /*      if ( ( player.NowMp( ) == player.MaxMp( ) ) && ( !ismpmax ) ) {
            mpmax->Gain( 0.5f );
            mpmax->Play( );
            ismpmax = true;
        }
        if ( ( !( player.NowMp( ) == player.MaxMp( ) ) ) && ( ismpmax ) ) {
            ismpmax = false;
        }*/
        /////////////////////////////////

        mojiManager.Update( );
    }
    void SceneGame::UpdateSpecial( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_s ) )
        {
            special.goSpecialMode( );
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
            camera.Shake( 0.05F );
            player.TranseNowMp( -1 );
        }

        // エフェクト終了時、全てのエネミー及び弾にダメージを与えます。
        if ( special.getEffectEnd( ) )
        {
            const float damagevalue = 10.0F;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( ) * damagevalue, camera( ), special.getSpecialType( ) ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( ) );
        }
    }
    void SceneGame::UpdateDebugTutorialClear( )
    {
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_1 ) )
        {
            TRData::spawn.TutorialEnd( );
        }
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_2 ) )
        {
            TRData::attackCircle.TutorialEnd( );
        }
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_3 ) )
        {
            TRData::guard.TutorialEnd( );
        }
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_4 ) )
        {
            TRData::playerAttack.TutorialEnd( );
        }
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_5 ) )
        {
            TRData::special.TutorialEnd( );
        }
    }
    void SceneGame::UpdateTutorialClear( )
    {
        if ( TRData::spawn.IsStopUpdate( ) )
        {
            talk->Update( );
            if ( TRData::IsSerifTalked( ) ) TRData::spawn.TutorialEnd( );
        }
        if ( TRData::attackCircle.IsStopUpdate( ) )
        {
            talk->Update( );
            if ( TRData::IsSerifTalked( ) ) TRData::attackCircle.TutorialEnd( );
        }

        if ( TRData::guard.IsStopUpdate( ) )
        {
            static bool isTalked = false;
            if ( !isTalked )
            {
                talk->Update( );
                if ( TRData::IsSerifTalked( ) ) isTalked = true;
                return;
            }

            Vec2f left( env.getWindowWidth( ) / 2 - 256, env.getWindowHeight( ) / 2 );
            Vec2f right( env.getWindowWidth( ) / 2 + 256, env.getWindowHeight( ) / 2 );
            handAnimation.updateGuard( left, right, Vec2f( 512, 512 ) );

            description->SetMode( Description::Mode::BOUGYO );

            // ディフェンスに使う関数。
            player.UpdateDeffenceOfTouch( );

            if ( TRData::guard.IsSuccessFull( ) )
            {
                description->SetMode( Description::Mode::NORMAL );
                TRData::guard.TutorialEnd( );
                isTalked = false;
            }
            else if ( player.Command( ) == CommandType::GUARD && enemyManager->EnemyToPlayerGuardCheck( player.GuardLine( ), camera( ) ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"そうそうその調子。" );

                TRData::guard.Update( true );
            }
            else if ( player.Command( ) == CommandType::GUARD )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"そのまま剣を赤い円の中に入れてみて！" );

                TRData::guard.Update( false );
            }
            else
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"両手を握り続けてみて！ ガードになるわよ。" );

                TRData::guard.Update( false );
            }
        }

        if ( TRData::playerAttack.IsStopUpdate( ) )
        {
            static bool isTalked = false;
            if ( !isTalked )
            {
                talk->Update( );
                if ( TRData::IsSerifTalked( ) ) isTalked = true;
                return;
            }

            Vec2f end( env.getWindowWidth( ) / 2 - 256, env.getWindowHeight( ) / 2 );
            Vec2f begin( env.getWindowWidth( ) / 2 + 256, env.getWindowHeight( ) / 2 );
            handAnimation.updateAttack( begin, end, Vec2f( 512, 512 ) );

            description->SetMode( Description::Mode::KOUGEKI );

            // 攻撃時に必要な関数。
            player.UpdateDeffenceOfTouch( );
            UpdateDamage( );
            enemyManager->update( camera( ) );
            UpdateScore( );
            UpdateCombo( );

            bool isHit = false;
            for ( auto& obj : moveInput.Lines( ) )
            {
                if ( 0 != enemyManager->PlayerToEnemyAttackCheck( obj, camera( ) ) )
                {
                    isHit = true;
                    break;
                }
            }

            if ( enemyManager->IsEmpty( ) )
            {
                description->SetMode( Description::Mode::NORMAL );
                TRData::playerAttack.TutorialEnd( );
                isTalked = false;
            }
            else if ( isHit )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"そうそうそんな感じよ！" );
            }
            else if ( player.Command( ) == CommandType::GUARD )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"それはガードよ。片方の手を握って動かすの！" );
            }
            else if ( talk->IsAnyPushTouch( ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"そのまま手を動かすのよ！" );
            }
            else if ( talk->IsAnyPullTouch( ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"手を握り続けて動かしてみて。" );
            }
        }

        if ( TRData::enemyKill.IsStopUpdate( ) )
        {
            player.UpdateDeffenceOfTouch( );

            talk->Update( );
            if ( TRData::IsSerifTalked( ) )
            {
                mojiManager.Setup( "JSON/GameStart.json" );
                TRData::enemyKill.TutorialEnd( );
            }
        }

        if ( TRData::special.IsStopUpdate( ) )
        {
            talk->Update( );
            if ( TRData::IsSerifTalked( ) )
            {
                TRData::special.TutorialEnd( );
                mpmax->Gain( 1.f );
                mpmax->Play( );
                special.goSpecialMode( );

            }
        }
    }
    void SceneGame::update( )
    {
        camera.Update( );

        if ( !special.getIsSpecial( ) )
        {
            moveInput.Begin( UI->ComboNumber( ) );
            if ( player.Command( ) == GUARD ) moveInput.InputInvalidation( );
        }

        UpdateDebugTutorialClear( );

        UpdateTutorialClear( );

        UpdateSpecial( );

        UpdateDamageExpression( );

        UpdateAllInstans( );

        UpdateNextStage( );

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
            create( new SceneResult( UI->Score( ), UI->MaxComboNumber( ), player.NowHp( ) + player.NowMp( ), time / 60 ) );///////ここ！！
            return;
        }

        // ラスボスを倒し終わったら、何らかのアクション後、次のシーンへ。
        if ( enemyManager->IsEmpty( ) && fieldManager->IsLastField( ) && sceneChangeFrame != 0 )
        {
            float gain = ( static_cast<float>( sceneChangeFrame ) / maxSceneChangeFrame ) * 0.4F;
            mainbgm[0]->Gain( gain );
            sceneChangeFrame = std::max( sceneChangeFrame - 1, 0 );

            camera.Shake( 0.05F );

            mojiManager.ReCall( "JSON/GameClear.json" );
        }

        // 死亡したらゲームオーバーの文字を表示して襖を閉める。
        if ( player.NowHp( ) <= 0.0F && sceneChangeFrame != 0 )
        {
            float gain = ( static_cast<float>( sceneChangeFrame ) / maxSceneChangeFrame ) * 0.4F;
            mainbgm[0]->Gain( gain );
            sceneChangeFrame = std::max( sceneChangeFrame - 1, 0 );

            damageColor.a = std::min( damageColor.a + 0.005, 0.6 );

            mojiManager.ReCall( "JSON/GameOver.json" );
        }

        if ( sceneChangeFrame == 0 )
        {
            mojiManager.End( );

            if ( mojiManager.IsEnded( ) )
            {
                fusuma->closeFusuma( );

                if ( fusuma->IsMoveFinished( ) )
                {
                    create( new SceneResult( UI->Score( ), UI->MaxComboNumber( ), player.NowHp( ) + player.NowMp( ), time / 60 ) );
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
        gl::setMatrices( camera( ) );
    }
    void SceneGame::drawMain( )
    {
        if ( special.isMinigame( ) ) return;

        fieldManager->Draw( camera( ) );
        enemyManager->draw( camera( ) );
        enemyBulletManager->draw( camera( ) );

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

            enemyManager->drawUI( camera( ) );

            enemyManager->DrawAttackCircle( camera( ) );

            enemyBulletManager->DrawBulletCircle( camera( ) );

            effectManager->Draw( );

            enemyManager->DrawEnemyHitPoint( );

            UI->draw( player.NormalizedMp( ),
                      player.NormalizedHp( ),
                      ( player.NowMp( ) == player.MaxMp( ) ) && ( special.getSpecialType( ) == SpecialType::NOTSELECTED ),
                      int( special.getSpecialType( ) ) );

            talk->Draw( Vec2f( 0, env.getWindowHeight( ) ) + Vec2f( 0, -220 ) );

            description->Draw( Vec2f( 230, env.getWindowHeight( ) ) + Vec2f( 0, -245 ) );

            if ( TRData::guard.IsStopUpdate( ) )
            {
                gl::pushModelView( );
                gl::translate( env.getWindowWidth( ) / 2 - bougyo->getWidth( ) / 2, 50 );
                gl::draw( *bougyo );
                gl::popModelView( );
                handAnimation.drawGuard( Vec2f( 600, 50 ) );
                TRData::guard.Draw( );
            }
            else if ( TRData::playerAttack.IsStopUpdate( ) )
            {
                gl::pushModelView( );
                gl::translate( env.getWindowWidth( ) / 2 - kougeki->getWidth( ) / 2, 50 );
                gl::draw( *kougeki );
                gl::popModelView( );
                handAnimation.drawAttack( Vec2f( 600, 50 ) );
            }

            mojiManager.Draw( env.getWindowCenter( ), 135 );
        }

        special.draw( );

        gl::color( damageColor );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        fusuma->drawFusuma( );
    }
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }

}