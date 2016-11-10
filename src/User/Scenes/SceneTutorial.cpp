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
        cameraEyePosition = Vec3f( 0, 0.7, -5 );
        camera.lookAt( cameraEyePosition, Vec3f( 0, 0.7, 0 ) );
        camera.setWorldUp( Vec3f( 0, 1, 0 ) );
        camera.setPerspective( 60.0F, env.getWindowAspectRatio( ), 1.0F, 100.0F );
        fieldManager = std::make_shared<FieldManager>( "JSON/TutorialField.json" );
        enemyManager = std::make_shared<EnemyManager>( camera, fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        effectManager = std::make_shared<EffectManager>( );
        UI = std::make_shared<Interface>( );
        gameClearFrame = 60 * 3;
        timer.Off( );
        damageColor = ColorA( 1, 0, 0, 0 );
        talk = std::make_shared<Talk>( Vec2f( 0, env.getWindowHeight( ) - 300 ) );

        // 野本が管理するものを作成。
        player = Player( 100, 100 );

        // 大ちゃんが管理するものを作成。
        mainbgm.push_back( &GData::FindAudio( "BGM/mainbgm0.wav" ) );
        mainbgm[0]->Looping( true );
        mainbgm[0]->Gain( 0.4 );
        mainbgm[0]->Play( );
    }
    SceneTutorial::~SceneTutorial( )
    {
        mainbgm[0]->Stop( );
    }
    void SceneTutorial::resize( )
    {
        camera.setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneTutorial::UpdateDamage( )
    {
        int playerHP = player.NowHp( );

        // プレイヤーが何もしていないなら
        if ( player.Command( ) == CommandType::NONE )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( camera );
            damage += enemyBulletManager->EnemyToPlayerDamage( camera );
            player.TranseNowHp( -damage );
        }

        // プレイヤーが攻撃状態なら
        if ( player.Command( ) == CommandType::ATTACK )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( camera );
            damage += enemyBulletManager->EnemyToPlayerDamage( camera );
            player.TranseNowHp( -damage );
        }

        // プレイヤーがガード状態なら
        if ( player.Command( ) == CommandType::GUARD )
        {
            int damage = 0;
            damage += enemyManager->EnemyToPlayerDamage( player.GuardLine( ), camera );
            damage += enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), camera );
            player.TranseNowHp( -damage );
        }

        if ( player.IsAttack( ) )
        {
            player.TranseNowMp( enemyManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.TranseNowMp( enemyBulletManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
        }

        if ( playerHP != player.NowHp( ) )
        {
            damageColor.a = std::min( damageColor.a + 0.2, 0.6 );
            timer.On( );
            timer.Advance( 60 );
        }
    }
    void SceneTutorial::UpdatePlayerBegin( )
    {
        player.Update( );

        UpdatePlayerBehavior( );
    }
    void SceneTutorial::UpdatePlayerBehavior( )
    {
        player.AttackPhase( );
    }
    void SceneTutorial::UpdatePlayerEnd( )
    {
        if ( player.IsAttack( ) )
        {
            player.ShiftIsAttack( );
        }
    }
    void SceneTutorial::UpdateColor( )
    {
        damageColor.a = std::max( damageColor.a - 0.01F, 0.0F );
    }
    void SceneTutorial::UpdateScore( )
    {
        UI->AddScore( enemyManager->ScoreRecovery( ) );
        UI->AddScore( enemyBulletManager->ScoreRecovery( ) );
    }
    void SceneTutorial::UpdateDamageExpression( )
    {
        timer.Update( );

        if ( timer.IsCount( ) )
        {
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }

        if ( timer.IsAction( ) )
        {
            timer.Off( );
            camera.lookAt( cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }
    }
    void SceneTutorial::UpdateNextStage( )
    {
        // エネミーが全滅したら、次のステージを準備。
        if ( enemyManager->IsEmpty( ) && TRData::enemyKill.IsComplete( ) )
        {
            fieldManager->End( );
        }

        // ステージの移動が終了したら次のステージへ。
        if ( !fieldManager->IsLastField( ) && fieldManager->IsChange( ) )
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
    void SceneTutorial::UpdateAllInstans( )
    {
        UI->update( );

        // スペシャル選択時から、エフェクトが終わるまで全ての動作を止めます。
        if ( !special.getIsSpecial( ) )
        {
            fieldManager->Update( );

            if ( !TRData::IsStopUpdate( ) )
            {
                enemyManager->update( camera );
                enemyBulletManager->BulletRegister( enemyManager->BulletsRecovery( ) );
                enemyBulletManager->update( );

                UpdateDamage( );
                UpdateColor( );
                UpdateScore( );
            }

            effectManager->EffectRegister( enemyManager->EffectRecovery( ) );
            effectManager->Update( );
        }
    }
    void SceneTutorial::UpdateSpecial( )
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
            const float damagevalue = 10.0F;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( ) * damagevalue ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( ) );
        }
    }
    void SceneTutorial::UpdateDebugTutorialClear( )
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
    void SceneTutorial::UpdateTutorialClear( )
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
            static bool isSerifEnd = false;

            if ( !isSerifEnd )
            {
                if ( talk->IsAnyPushTouch( ) )
                {
                    talk->Update( );
                    if ( TRData::IsSerifTalked( ) )
                    {
                        TRData::talkString.clear( );
                        TRData::talkString.push_front( u8"両手を握り続けてみて！ そうするとガードになるわよ。" );
                        isSerifEnd = true;
                    }
                }
                return;
            }

            if ( TRData::guard.IsSuccessFull( ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"うまいじゃない！！" );

                TRData::guard.TutorialEnd( );
                isSerifEnd = false;
            }
            else if ( player.Command( ) == CommandType::GUARD && enemyManager->EnemyToPlayerGuardCheck( player.GuardLine( ), camera ) )
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
                TRData::talkString.push_front( u8"両手を握り続けてみて！ そうするとガードになるわよ。" );

                TRData::guard.Update( false );
            }
        }

        if ( TRData::playerAttack.IsStopUpdate( ) )
        {
            static bool isSerifEnd = false;

            if ( !isSerifEnd )
            {
                if ( talk->IsAnyPushTouch( ) )
                {
                    talk->Update( );
                    if ( TRData::IsSerifTalked( ) )
                    {
                        TRData::talkString.clear( );
                        TRData::talkString.push_front( u8"手を握り続けて動かして！！" );
                        isSerifEnd = true;
                    }
                }
                return;
            }

            // チュートリアル中でもダメージ計算を行います。
            UpdateDamage( );
            enemyManager->update( camera );
            UpdateScore( );

            if ( enemyManager->IsEmpty( ) )
            {
                TRData::playerAttack.TutorialEnd( );
                isSerifEnd = false;
            }
            else if ( player.IsAttack( ) )
            {
                if ( 0 != enemyManager->PlayerToEnemyAttackCheck( player.MakeLine( ), camera ) )
                {
                    TRData::talkString.clear( );
                    TRData::talkString.push_front( u8"そうそうそんな感じよ！" );
                }
            }
            else if ( talk->IsAnyPushTouch( ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"そのまま手を動かして離すのよ！" );
            }
            else if ( talk->IsAnyPullTouch( ) )
            {
                TRData::talkString.clear( );
                TRData::talkString.push_front( u8"手を握り続けて動かして！！" );
            }
        }

        if ( TRData::enemyKill.IsStopUpdate( ) )
        {
            talk->Update( );
            if ( TRData::IsSerifTalked( ) ) TRData::enemyKill.TutorialEnd( );
        }

        if ( TRData::special.IsStopUpdate( ) )
        {
            talk->Update( );
            if ( TRData::IsSerifTalked( ) ) TRData::special.TutorialEnd( );
        }
    }
    void SceneTutorial::update( )
    {
        UpdatePlayerBegin( );

        UpdateDebugTutorialClear( );

        UpdateTutorialClear( );

        UpdateSpecial( );

        UpdateDamageExpression( );

        UpdateAllInstans( );

        UpdateNextStage( );

        UpdatePlayerEnd( );
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
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_e ) )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }

        if ( player.NowHp( ) <= 0.0F )
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
    void SceneTutorial::beginDrawMain( )
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
        gl::setMatrices( camera );
    }
    void SceneTutorial::drawMain( )
    {
        fieldManager->Draw( camera );
        enemyManager->draw( camera );
        enemyBulletManager->draw( camera );

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

        enemyManager->drawUI( );

        enemyManager->DrawAttackCircle( camera );

        enemyBulletManager->DrawBulletCircle( camera );

        effectManager->Draw( camera );

        UI->draw( player.NormalizedMp( ), player.NormalizedHp( ) );

        talk->Draw( );

        special.draw( );

        TRData::guard.Draw( );

        gl::color( damageColor );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );
    }
    void SceneTutorial::endDrawUI( )
    {
        gl::popMatrices( );
    }
}