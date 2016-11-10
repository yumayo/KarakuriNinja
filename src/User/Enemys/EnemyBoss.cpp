# include "EnemyBoss.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

#include "EnemyTutorial.h"
#include "EnemySlash.h"
#include "EnemySlashNot.h"
#include "EnemyBullet.h"
#include "EnemyBulletNot.h"
#include "EnemyBoss.h"
#include "EnemyBulletTutorial.h"
#include "EnemySlashWalk.h"
#include "EnemyBulletWalk.h"

# include "../Utilitys/Hirasawa.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBoss::EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 22.0F, 7 ) )
        , timer( )
        , isAttack( false )
        , isDeadStop( true )
        , font( u8"HG行書体", 90 )
        , serif( u8"" )
        , prevAttackHP( 0 )
    {
        TRData::bossSpawn.TutorialStart( );
        {
            int index = 1;
            messageBoxLeft = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
            messageBoxCenter = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
            messageBoxRight = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
        }

        {
            int index = 1;
            待機 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            texture = 待機;
            投げるモーション画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            投げる画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            攻撃モーション画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            攻撃画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            左に移動 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            右に移動 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            倒れかけるモーション = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            倒れるモーション = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            knockBackTexture = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );

        }
        {
            int index = 1;
            オーラ1 = &GData::FindTexture( "Enemy/Boss/オーラ_1_" + std::to_string( index++ ) + ".png" );
            オーラ2 = &GData::FindTexture( "Enemy/Boss/オーラ_1_" + std::to_string( index++ ) + ".png" );
            auraTex = nullptr;
        }

        timer.Advance( 240 );
        SetFunction( &EnemyBoss::ボス出現のエフェクトが終了するまで待機 );
        SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );

        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
            弾カウント = 0;
        }

        jsonEnemys = JsonTree( app::loadAsset( "JSON/Bosspawn.json" ) )["Enemy"];
        jsonItr = jsonEnemys.begin( );
    }
    void EnemyBoss::update( cinder::CameraPersp const& camera )
    {
        if ( IsDeadSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );
            prevPosition = object.Position( );
            timer.Advance( 120 );
            SetFunction( &EnemyBoss::死ぬ時のモーション );
        }
        if ( IsLive( ) )
        {
            // 重力処理
            Gravitate( );
            // ノックバック時は一時的に処理をしません。
            if ( !IsKnockBack( ) )
            {
                // メイン処理
                behavior( camera );

                // タイマー処理
                timer.Update( );
            }
        }
        else
        {
            //texture = knockBackTexture;
        }


        frame += 1;
        CameraSee( camera );

        if ( IsLive( ) )
        {
            LiveCheck( );
            if ( !IsKnockBack( ) )
            {
                object.PositionAdd( object.Speed( ) );
                attackTime.Update( );
            }
        }
        MutekiEffect( );
        DamageEffect( );
        CollideGround( );// 死んでいても実行します。
        //CollideField( );// 死んでいても実行します。
        Dying( );// 死んでいても実行します。

                 // デバッグダメージ
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Kill( );
    }
    void EnemyBoss::draw( )
    {
        if ( isLive )
        {
            DrawAura( );
        }

        EnemyBase::draw( );

        if ( isLive )
        {
            if ( IsKnockBack( ) )
            {
                objects.clear( );
            }
            int i = 1;
            for ( auto itr = objects.begin( ); itr != objects.end( ); ++itr, ++i )
            {
                DrawAfterimage( *itr, i );
            }
        }
    }
    void EnemyBoss::drawUI( const cinder::CameraPersp& camera )
    {
        EnemyBase::drawUI( camera );
        serifBehavior( );
    }
    bool EnemyBoss::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && isDeadStop;
    }
    bool EnemyBoss::IsLive( )
    {
        return isLive || isDeadStop;
    }
    void EnemyBoss::Gravitate( )
    {
        //　ジャンプ中なら重力をかけます。
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyBoss::DrawAfterimage( EnemyObject& object, int index )
    {
        gl::disable( GL_CULL_FACE );

        gl::pushModelView( );
        gl::translate( object.Position( ).x, 0, object.Position( ).z );
        gl::translate( 0, 0.05, 0 );
        gl::rotate( Vec3f( 90, 0, 0 ) );
        gl::color( ColorA( 0, 0, 0, 0.5 / index ) );
        auto distans = 1 - ( object.Position( ).y - object.Size( ).y / 2 ) / 2;
        if ( distans < 0 ) distans = 0;
        gl::drawSolidCircle( Vec2f::zero( ), ( object.Size( ).xy( ).length( ) / 4.0F ) * distans, 100 );
        gl::popModelView( );

        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );

        if ( IsKnockBack( ) )
        {
            gl::pushModelView( );
            gl::rotate( Vec3f( 0, 180, 180 ) );
            ColorA col = HitColor( );
            col.a = 1.0 / index;
            gl::color( col );
            knockBackTexture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            knockBackTexture->unbind( );
            gl::popModelView( );
        }
        else
        {
            gl::pushModelView( );
            gl::rotate( Vec3f( 0, 180, 180 ) );
            ColorA col = HitColor( );
            col.a = 1.0 / index;
            gl::color( col );
            texture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            texture->unbind( );
            gl::popModelView( );
        }

        gl::popModelView( );

        gl::enable( GL_CULL_FACE );
    }
    void EnemyBoss::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        if ( IsFrieldKilledSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );

            isFrieldKilledSerif = true;
            SetSerifFunction( u8"よくも私の仲間を！！", &EnemyBoss::セリフ );

            timer.Advance( 120 ); // セリフを吐くフレーム
            SetFunction( &EnemyBoss::部下を倒された時のセリフ );
            return;
        }

        // タイマーが鳴ったら次の関数へ。
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemyBoss::どっちの攻撃を出すかの確認 );
            return;
        }
    }
    void EnemyBoss::どっちの攻撃を出すかの確認( cinder::CameraPersp const & camera )
    {
        auto anyAttack = randBool( );
        if ( anyAttack && ( 直接攻撃カウント < 直接攻撃の上限回数 ) )
        {
            弾カウント = 0;

            SetFunction( &EnemyBoss::直接攻撃選択 );
            return;
        }
        else if ( 弾カウント < 弾の上限回数 )
        {
            直接攻撃カウント = 0;

            SetFunction( &EnemyBoss::間接攻撃選択 );
            return;
        }
        else // どちらとも出せない場合
        {
            if ( anyAttack )
            {
                SetFunction( &EnemyBoss::直接攻撃選択 );
                return;
            }
            else
            {
                SetFunction( &EnemyBoss::間接攻撃選択 );
                return;
            }
        }
    }
    void EnemyBoss::直接攻撃選択( cinder::CameraPersp const & camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
        cameraPos.y = 0;

        auto pos = object.Position( );
        pos.y = 0;

        moveSpeed = cameraPos - pos;
        moveSpeed /= 60.0F;

        moveLeftRightSpeed = Vec3f::xAxis( ) * 0.4F;

        timer.Advance( 60 ); // 高速移動フレーム

        直接攻撃カウント++;
        SetFunction( &EnemyBoss::左右に高速移動しながらカメラへ近づく );
        return;
    }
    void EnemyBoss::間接攻撃選択( cinder::CameraPersp const & camera )
    {
        timer.Advance( 120 ); // 弾を投げる動作
        texture = 投げるモーション画像;

        弾カウント++;
        SetFunction( &EnemyBoss::弾を投げるまでのモーション );
        return;
    }
    void EnemyBoss::左右に高速移動しながらカメラへ近づく( cinder::CameraPersp const& camera )
    {
        if ( 3 < objects.size( ) ) objects.pop_front( );

        if ( !IsInTheScreen( camera ) )
        {
            moveLeftRightSpeed *= -1;
            object.PositionAdd( moveLeftRightSpeed );
        }

        auto cameraRightVec = camera.getViewDirection( );
        cameraRightVec.rotate( camera.getWorldUp( ), -M_PI / 2.0 );
        if ( moveLeftRightSpeed.dot( cameraRightVec ) < 0 )
        {
            texture = 右に移動;
        }
        else
        {
            texture = 左に移動;
        }

        object.Speed( moveSpeed + moveLeftRightSpeed );

        objects.emplace_back( object );

        if ( timer.IsAction( ) )
        {
            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;
            object.Position( cameraPos );

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 140 ); // 攻撃モーションフレームを代入
            attackTime.AttackFrame( 140 );

            prevAttackHP = status.HP;
            objects.clear( );
            texture = 攻撃モーション画像;
            prevPosition = object.Position( );
            SetFunction( &EnemyBoss::攻撃モーション );
            return;
        }
    }
    void EnemyBoss::攻撃モーション( cinder::CameraPersp const& camera )
    {
        auraTex = ( ( frame / 15 ) % 2 == 0 ? オーラ1 : オーラ2 );
        auto yureru = Vec3f( randFloat( -0.01, 0.01 ), randFloat( -0.01, 0.01 ), 0 );
        object.Position( prevPosition + yureru );
        // 攻撃モーション後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            auraTex = nullptr;
            texture = 攻撃画像;
            object.Position( prevPosition );
            SetFunction( &EnemyBoss::攻撃 );
            return;
        }

        if ( prevAttackHP != status.HP )
        {
            auraTex = nullptr;
            texture = 待機;

            timer.Advance( 20 ); // 硬直フレーム
            SetFunction( &EnemyBoss::攻撃後硬直 );
            return;
        }
    }
    void EnemyBoss::攻撃( cinder::CameraPersp const& camera )
    {
        // 攻撃してすぐ次の関数へ。
        isAttack = true;

        timer.Advance( 20 ); // 硬直フレーム
        SetFunction( &EnemyBoss::攻撃後硬直 );
        return;
    }
    void EnemyBoss::攻撃後硬直( cinder::CameraPersp const& camera )
    {
        // 攻撃フラグをオフに、硬直時間10フレームの後、次の関数へ。
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            texture = 待機;
            SetFunction( &EnemyBoss::ジャンプで戻る );
            return;
        }
    }
    void EnemyBoss::ジャンプで戻る( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 6.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 12.0F, M_PI / 12.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // 硬直後、すぐにジャンプして次の関数へ。
        Jump( jumpPower + backDirection );

        SetFunction( &EnemyBoss::着地 );
        return;
    }
    void EnemyBoss::着地( cinder::CameraPersp const& camera )
    {
        // 地面に着いたら、次の関数へ。
        if ( isLanding )
        {
            // スピードは0にします。滑っちゃうので。
            object.Speed( Vec3f::zero( ) );
            // 次の攻撃までのフレームを代入。
            timer.Advance( randInt( 180, 280 ) );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::死ぬ時のモーション( cinder::CameraPersp const & camera )
    {
        isAttack = false;
        isDeadSerif = true;
        texture = 倒れかけるモーション;

        SetSerifFunction( u8"っく、我は滅びぬ", &EnemyBoss::セリフ );

        timer.Advance( 120 ); // セリフを吐くフレーム
        SetFunction( &EnemyBoss::死ぬ時のセリフ );
        return;
    }
    void EnemyBoss::死ぬ( cinder::CameraPersp const & camera )
    {
        isDeadStop = false;
    }
    void EnemyBoss::弾を投げるまでのモーション( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            texture = 投げる画像;
            SetFunction( &EnemyBoss::弾を投げる );
        }
    }
    void EnemyBoss::弾を投げる( cinder::CameraPersp const & camera )
    {
        //WavyBulletFiring( randFloat( 0, M_PI * 2 ), camera );
        RandomWavyBulletFiring( camera );
        for ( int i = 0; i < 3; i++ )
        {
            se[i]->Play( );
        }
        timer.Advance( 40 ); // 硬直フレーム

        SetFunction( &EnemyBoss::弾を投げた後の硬直 );
        return;
    }
    void EnemyBoss::弾を投げた後の硬直( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 340, 440 ) );

            texture = 待機;

            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::ボス出現のエフェクトが終了するまで待機( cinder::CameraPersp const & camera )
    {
        if ( TRData::bossSpawn.IsComplete( ) )
        {
            TRData::bossSerif.TutorialStart( );
            timer.Advance( 180 );

            SetSerifFunction( u8"いでよ、我が下僕よ！", &EnemyBoss::セリフ );
            SetFunction( &EnemyBoss::出現した時にエネミーをスポーンさせる );
            return;
        }
    }
    void EnemyBoss::出現した時にエネミーをスポーンさせる( cinder::CameraPersp const & camera )
    {
        if ( frame % 5 == 0 )
        {
            if ( jsonItr != jsonEnemys.end( ) )
            {
                auto type = jsonItr->getValueForKey<std::string>( "type" );
                auto position = getVec3f( ( *jsonItr )["position"] );
                if ( type == "slashNot" )
                {
                    EnemyCreate<EnemySlashNot>( position, camera );
                }
                else if ( type == "bulletNot" )
                {
                    EnemyCreate<EnemyBulletNot>( position, camera );
                }
                else if ( type == "slashWalk" )
                {
                    EnemyCreate<EnemySlashWalk>( position, camera );
                }
                else if ( type == "bulletWalk" )
                {
                    EnemyCreate<EnemyBulletWalk>( position, camera );
                }
                else if ( type == "tutorial" )
                {
                    EnemyCreate<EnemyTutorial>( position, camera );
                }
                else if ( type == "bulletTutorial" )
                {
                    EnemyCreate<EnemyBulletTutorial>( position, camera );
                }
                else if ( type == "bullet" )
                {
                    EnemyCreate<EnemyBullet>( position, camera );
                }
                else if ( type == "slash" )
                {
                    EnemyCreate<EnemySlash>( position, camera );
                }
                else if ( type == "boss" )
                {
                    EnemyCreate<EnemyBoss>( position, camera );
                }
                jsonItr++;
            }
        }

        if ( timer.IsAction( ) )
        {
            TRData::bossSerif.TutorialEnd( );
            TRData::special.TutorialStart( );
            SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::部下を倒された時のセリフ( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::死ぬ時のセリフ( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            texture = 倒れるモーション;
            timer.Advance( 60 );
            SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );
            SetFunction( &EnemyBoss::死ぬ );
        }
    }
    void EnemyBoss::ヌルセルフ( )
    {
        /*nothing*/
    }
    void EnemyBoss::セリフ( )
    {
        auto textSize = font.BoundingBox( serif ).getSize( );
        auto messageBoxHeight = messageBoxLeft->getHeight( );
        auto rectLeft = Rectf( Vec2f::zero( ), messageBoxLeft->getSize( ) );
        auto rectCenter = Rectf( Vec2f::zero( ), Vec2f( textSize.x, messageBoxHeight ) );
        auto rectRight = Rectf( Vec2f::zero( ), messageBoxRight->getSize( ) );

        int ue = 34;
        int inMessageHeight = 111;
        int fontStartHeightPosition = ue + ( inMessageHeight - textSize.y ) / 2;

        // メッセージボックスの表示
        gl::pushModelView( );

        gl::translate( -( rectLeft.getWidth( ) + rectCenter.getWidth( ) + rectRight.getWidth( ) ) / 2, 0 );

        Vec2f position = env.getWindowCenter( );
        position.y = 300;
        gl::translate( position + Vec2f( 0, -messageBoxHeight ) );
        messageBoxLeft->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectLeft );
        messageBoxLeft->unbind( );

        gl::translate( Vec2f( rectLeft.getWidth( ), 0 ) );
        messageBoxCenter->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectCenter );
        messageBoxCenter->unbind( );

        gl::pushModelView( );
        gl::translate( 0, fontStartHeightPosition );
        font.Draw( serif, Vec2f::zero( ), Color::white( ), Fonts::Mode::LEFTUP );
        gl::popModelView( );

        gl::translate( Vec2f( rectCenter.getWidth( ), 0 ) );
        messageBoxRight->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectRight );
        messageBoxRight->unbind( );

        gl::popModelView( );
    }
    bool EnemyBoss::IsFrieldKilledSerif( )
    {
        return !isFrieldKilledSerif && status.HP != status.maxHP;
    }
    bool EnemyBoss::IsDeadSerif( )
    {
        return !isDeadSerif && isLive == false;
    }
    void EnemyBoss::SetFunction( void( EnemyBoss::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
    void EnemyBoss::SetSerifFunction( std::string const& serif, void( EnemyBoss::* function )( ) )
    {
        this->serif = serif;
        serifBehavior = std::bind( function, this );
    }
    void EnemyBoss::WavyBulletFiring( float radians, cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        auto direction = Vec2f::xAxis( );
        direction.rotate( radians );

        float length = std::min( env.getWindowWidth( ), env.getWindowHeight( ) );
        length /= 2.0F;
        length -= 50;

        {
            float u = 0.5F;
            float v = 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * -length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * -length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }
    }
    void EnemyBoss::RandomWavyBulletFiring( cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        Vec2f pos1, pos2, pos3;

        pos1.x = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.4 ) / env.getWindowWidth( );
        pos1.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( pos1.x, pos1.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos2.x = randFloat( env.getWindowWidth( )*0.6, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        pos2.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        ray = camera.generateRay( pos2.x, pos2.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos3.x = ( pos1.x + pos2.x ) / 2.0F;
        pos3.y = ( pos1.y + pos2.y ) / 2.0F;
        ray = camera.generateRay( pos3.x, pos3.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
    }

    void EnemyBoss::DrawAura( )
    {
        if ( auraTex )
        {
            auto object = this->object;
            object.Position( object.Position( ) - object.Direction( ) * 0.1 );

            gl::disable( GL_CULL_FACE );

            gl::pushModelView( );
            gl::translate( object.Position( ) );
            gl::multModelView( object.Quaternion( ).toMatrix44( ) );

            if ( !IsKnockBack( ) )
            {
                gl::pushModelView( );
                float s = 2.0F + ( 0.3F * math<float>::sin( float( frame ) / 2.5F ) );
                gl::scale( s, s, s );
                gl::rotate( Vec3f( 0, 180, 180 + frame ) );
                ColorA col = ColorA::white( );
                col.a = 0.5F + ( 0.5 * math<float>::cos( float( frame ) / 2.5F ) );
                gl::color( col );
                auraTex->bind( );
                gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
                auraTex->unbind( );
                gl::popModelView( );
            }

            gl::popModelView( );

            gl::enable( GL_CULL_FACE );
        }
    }
}