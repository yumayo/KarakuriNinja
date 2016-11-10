# include "EnemyBase.h"

# include "cinder/ImageIo.h"
# include "cinder/App/App.h"

# include "Utilitys.hpp"

# include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBase::EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : object( pos, Vec3f( 1.7, 1.7, 0.01 ), Vec3f::zero( ) )
        , texture( &GData::FindTexture( "null" ) )
        , knockBackTexture( &GData::FindTexture( "null" ) )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , deadTime( 60 )
        , status( )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
        GData::FindAudio( "SE/umareru.wav" ).Play( );
    }
    EnemyBase::EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp & camera, Status status, float sizeScale )
        : object( pos, Vec3f( 1.7 * sizeScale, 1.7 * sizeScale, 0.01 ), Vec3f::zero( ) )
        , texture( &GData::FindTexture( "null" ) )
        , knockBackTexture( &GData::FindTexture( "null" ) )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , deadTime( 60 )
        , status( status )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
        GData::FindAudio( "SE/umareru.wav" ).Play( );
    }
    void EnemyBase::update( cinder::CameraPersp const& camera )
    {
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
        DamageEffect( );
        CollideGround( );// 死んでいても実行します。
        CollideField( );// 死んでいても実行します。
        Dying( );// 死んでいても実行します。

        // デバッグダメージ
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Kill( );
    }
    void EnemyBase::draw( )
    {
    #ifdef _DEBUG
        gl::color( ColorA( 1, 1, 0, 1 ) );
        gl::drawVector( object.Position( ), object.Position( ) + object.Direction( ) * 0.1 );
    #endif // _DEBUG

        gl::disable( GL_CULL_FACE );

        gl::pushModelView( );
        gl::translate( object.Position( ).x, 0, object.Position( ).z );
        gl::translate( 0, 0.05, 0 );
        gl::rotate( Vec3f( 90, 0, 0 ) );
        gl::color( ColorA( 0, 0, 0, 0.5 ) );
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
            gl::color( HitColor( ) );
            knockBackTexture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            knockBackTexture->unbind( );
            gl::popModelView( );
        }
        else
        {
            gl::pushModelView( );
            gl::rotate( Vec3f( 0, 180, 180 ) );
            gl::color( HitColor( ) );
            texture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            texture->unbind( );
            gl::popModelView( );
        }


    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG

        gl::popModelView( );

        gl::enable( GL_CULL_FACE );
        }
    void EnemyBase::drawUI( cinder::CameraPersp const& camera )
    {

    }
    int EnemyBase::Hit( cinder::CameraPersp const& camera, float length, int scoreRate, float value )
    {
        int drainMP = 0;
        if ( length <= 0.2F )
        {
            status.HP = std::max( status.HP - 3.0F * value, 0.0F );
            hitColor = Color( 1, 0.1, 0.1 );
            drainMP = 7;
        }
        else if ( length <= 0.5F )
        {
            status.HP = std::max( status.HP - 2.5F * value, 0.0F );
            hitColor = Color( 1, 0.2, 0.2 );
            drainMP = 5;
        }
        else if ( length <= 1.0F )
        {
            status.HP = std::max( status.HP - 1.5F * value, 0.0F );
            hitColor = Color( 1, 0.3, 0.3 );
            drainMP = 2;
        }

        if ( drainMP != 0 )
        {
            Vec2f vec = camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );

            vec.x = randFloat( vec.x - 100, vec.x + 100 );
            vec.y = randFloat( vec.y - 100, vec.y + 100 );

            EffectCreate( EffectScore( vec, drainMP * scoreRate ) );
        }

        if ( !isLive ) hitColor = Color( 1, 0, 0 );

        return drainMP;
    }
    int EnemyBase::Damage( int damage )
    {
        if ( damage < 0 ) return 0;
        status.HP = std::max( status.HP - damage, 0.0F );
        hitColor = Color( 1, 0.1, 0.1 );

        return 2;
    }
    void EnemyBase::Kill( )
    {
        status.HP = 0.0F;
        hitColor = Color( 1, 0.1, 0.1 );
    }
    bool EnemyBase::IsActive( )
    {
        return 0 < deadTime;
    }
    bool EnemyBase::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    EnemyList EnemyBase::EnemyRecovery( )
    {
        auto ret = enemyList;
        enemyList.clear( );
        return ret;
    }
    EnemyBulletList EnemyBase::BulletRecovery( )
    {
        auto ret = bulletList;
        bulletList.clear( );
        return ret;
    }
    EffectList EnemyBase::EffectRecovery( )
    {
        auto ret = effectList;
        effectList.clear( );
        return ret;
    }

    bool EnemyBase::IsJumping( )
    {
        return isLanding == false;
    }

    bool EnemyBase::IsUnderGround( )
    {
        return Position( ).y - Size( ).y / 2.0F < 0.0F;
    }
    bool EnemyBase::IsInTheScreen( cinder::CameraPersp const & camera )
    {
        Vec2f ScreenPosition = camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
        return Utl::Colli2D::rectPoint( Vec2f::zero( ), env.getWindowSize( ), ScreenPosition );
    }
    bool EnemyBase::IsInField( )
    {
        auto x = object.Position( ).x;
        return -1 <= x && x <= 1;
    }
    bool EnemyBase::IsKnockBack( )
    {
        // 真っ赤でない時かつ白色でない時。
        // つまり、中途半端な色の時はノックバックであると仮定します。
        return ( hitColor != Color( 1, 0, 0 ) ) && ( hitColor != Color::white( ) );
    }
    void EnemyBase::CollideField( )
    {
        if ( !IsInField( ) )
        {
            auto pos = object.Position( );
            if ( pos.x < -1 )
            {
                pos.x = -1;
                object.Position( pos );
            }
            else if ( 1 < pos.x )
            {
                pos.x = 1;
                object.Position( pos );
            }
        }
    }
    void EnemyBase::CollideGround( )
    {
        // オブジェクトの埋まった位置を戻します。
        if ( IsUnderGround( ) )
        {
            auto pos = object.Position( );
            auto speed = object.Speed( );

            isLanding = true;

            pos.y = Size( ).y / 2.0F;
            object.Position( pos );

            speed.y = 0;
            object.Speed( speed );
        }
    }
    void EnemyBase::LiveCheck( )
    {
        if ( !isLive ) return;
        isLive = 0.0F < status.HP;
    }
    void EnemyBase::Dying( )
    {
        if ( !IsLive( ) )
        {
            deadTime = std::max( deadTime - 1, 0 );
        }
    }
    void EnemyBase::DamageEffect( )
    {
        if ( isLive )
        {
            hitColor += Color( 0.03, 0.03, 0.03 );
            if ( 1 < hitColor.r )
                if ( 1 < hitColor.g )
                    if ( 1 < hitColor.b ) hitColor = Color::white( );
        }
        else
            hitColor = Color( 1, 0, 0 );

    }
    void EnemyBase::CameraSee( cinder::CameraPersp const & camera )
    {
        auto cameraEyePos = camera.getEyePoint( );
        cameraEyePos.y = object.Position( ).y;
        object.Direction( cameraEyePos - object.Position( ) );
    }
    void EnemyBase::Jump( cinder::Vec3f jumpPower )
    {
        object.Speed( jumpPower );
        isLanding = false;
    }
    void EnemyBase::SpawnEffect( cinder::CameraPersp const& camera )
    {
        Vec2f vec = camera.worldToScreen( object.Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
        Vec2f size = camera.worldToScreen( object.Position( ) + object.Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
        EffectCreate( EffectAlpha( "Textures/Effect/kumo2.png",
                                   vec,
                                   camera.worldToEyeDepth( object.Position( ) ),
                                   Vec2f( vec - size ) * 2.5F,
                                   Vec2f( 600, 300 ),
                                   EffectBase::Mode::CENTERCENTER
        ) );
    }
}