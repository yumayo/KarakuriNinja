# include "EnemyBoss.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

# include "../Utilitys/Hirasawa.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBoss::EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 20.0F, 7 ) )
        , timer( )
        , isAttack( false )
        , isDeadStop( true )
        , isHalfHPSerif( true )
        , isDeadSerif( true )
        , font( u8"メイリオ", 74 )
        , serif( u8"" )
        , serifDrawPosition( Vec2f::zero( ) )
    {
        int index = 1;
        待機 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        投げるモーション画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        投げる画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        攻撃モーション画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        攻撃画像 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        左に移動 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        右に移動 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        倒れかけるモーション = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
        倒れるモーション = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );

        texture = 待機;

        timer.Advance( 180 ); // セリフを吐くフレーム
        SetFunction( &EnemyBoss::出現した時のセリフ );
        SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );

        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
            弾カウント = 0;
        }

    }
    void EnemyBoss::update( cinder::CameraPersp const& camera )
    {
        if ( isDeadStop )
        {
            // 重力処理
            Gravitate( );
            // メイン処理
            behavior( camera );
            // タイマー処理
            timer.Update( );

        }

        // 以下 EnemyBaseUpdate
        CameraSee( camera );

        if ( isDeadStop )
        {
            object.PositionAdd( object.Speed( ) );
            DamageEffect( );
            LiveCheck( );
            attackTime.Update( );
        }
        CollideGround( ); // 死んでいても実行します。
        //CollideField( );// 死んでいても実行します。
        if ( isDeadStop == false ) // ここで、死ぬと判断されることを遅らせています。
            Dying( );

        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Kill( );
    }
    void EnemyBoss::draw( )
    {
        EnemyBase::draw( );
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
    void EnemyBoss::Gravitate( )
    {
        //　ジャンプ中なら重力をかけます。
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyBoss::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        if ( IsHalfHPSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );

            timer.Advance( 120 ); // セリフを吐くフレーム
            SetFunction( &EnemyBoss::HPが半分以下になった時のセリフ );
            return;
        }
        if ( IsDeadSerif( ) )
        {
            texture = 倒れかけるモーション;

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 120 ); // セリフを吐くフレーム
            SetFunction( &EnemyBoss::死ぬ時のセリフ );
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
        auto どちらの攻撃 = randBool( );
        if ( どちらの攻撃 && ( 直接攻撃カウント < 直接攻撃の上限回数 ) )
        {
            弾カウント = 0;

            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;

            auto pos = object.Position( );
            pos.y = 0;

            moveSpeed = cameraPos - pos;
            moveSpeed /= 60.0F;

            moveLeftRightSpeed = Vec3f::xAxis( ) * 0.5F;

            timer.Advance( 60 ); // 高速移動フレーム

            直接攻撃カウント++;
            SetFunction( &EnemyBoss::左右に高速移動しながらカメラへ近づく );
            return;
        }
        else if ( 弾カウント < 弾の上限回数 )
        {
            直接攻撃カウント = 0;

            timer.Advance( 120 ); // 弾を投げる動作
            texture = 投げるモーション画像;

            弾カウント++;
            SetFunction( &EnemyBoss::弾を投げるまでのモーション );
            return;
        }
        else // どちらとも出せない場合
        {
            弾カウント = 0;
            直接攻撃カウント = 0;
            どっちの攻撃を出すかの確認( camera );
        }
    }
    void EnemyBoss::左右に高速移動しながらカメラへ近づく( cinder::CameraPersp const& camera )
    {
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

        if ( timer.IsAction( ) )
        {
            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;
            object.Position( cameraPos );

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 80 ); // 攻撃モーションフレームを代入
            attackTime.AttackFrame( 80 );

            texture = 攻撃モーション画像;
            SetFunction( &EnemyBoss::攻撃モーション );
            return;
        }
    }
    void EnemyBoss::攻撃モーション( cinder::CameraPersp const& camera )
    {
        // 攻撃モーション後、次の関数へ。
        if ( timer.IsAction( ) )
        {
            texture = 攻撃画像;
            SetFunction( &EnemyBoss::攻撃 );
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
        auto backDirection = -object.Direction( ) * 8.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 4.0F, M_PI / 4.0F ) );
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
            timer.Advance( randInt( 60, 120 ) );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
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
        timer.Advance( 20 ); // 硬直フレーム

        弾を投げる回数++;
        SetFunction( &EnemyBoss::弾を投げた後の硬直 );
        return;
    }
    void EnemyBoss::弾を投げた後の硬直( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            timer.Advance( 120 );

            SetFunction( &EnemyBoss::弾をもう一度投げるかの確認 );
            return;
        }
    }
    void EnemyBoss::弾をもう一度投げるかの確認( cinder::CameraPersp const & camera )
    {
        if ( 弾を投げる回数 < 弾を投げる上限回数 )
        {
            texture = 投げるモーション画像;
            if ( timer.IsAction( ) )
            {
                SetFunction( &EnemyBoss::弾を投げるまでのモーション );
                return;
            }
        }
        else
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 180, 240 ) );

            texture = 待機;

            弾を投げる回数 = 0;
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::出現した時のセリフ( cinder::CameraPersp const & camera )
    {
        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"がっはっは、何用だ貴様", &EnemyBoss::セリフ );

        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::HPが半分以下になった時のセリフ( cinder::CameraPersp const & camera )
    {
        isHalfHPSerif = false;

        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"なかなかやるな貴様", &EnemyBoss::セリフ );

        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::ヌルセルフ );
            SetFunction( &EnemyBoss::タイマーが鳴るまで待機 );
            return;
        }
    }
    void EnemyBoss::死ぬ時のセリフ( cinder::CameraPersp const & camera )
    {
        isDeadSerif = false;

        serifDrawPosition = camera.worldToScreen( object.Position( ) + Vec3f( 0, object.Size( ).y, 0 ), env.getWindowWidth( ), env.getWindowHeight( ) );
        SetSerifFunction( u8"っく、我は滅びぬ", &EnemyBoss::セリフ );

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
        gl::pushModelView( );
        gl::translate( serifDrawPosition );
        gl::color( Color::white( ) );
        auto size = font.BoundingBox( serif ).getSize( );
        gl::drawSolidRect( Rectf( -size.x / 2.0F, 0, size.x / 2.0F, size.y ) );
        font.Draw( serif, Vec2f::zero( ), Color::black( ), Fonts::Mode::CENTERUP );
        gl::popModelView( );
    }
    bool EnemyBoss::IsHalfHPSerif( )
    {
        return isHalfHPSerif && NormalizedHitPoint( ) <= 0.5F;
    }
    bool EnemyBoss::IsDeadSerif( )
    {
        return isDeadSerif && IsLive( ) == false;
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
}