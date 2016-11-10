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
        , maxHP( 5.0F )
        , HP( maxHP )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( 3 )
        , deadTime( 60 )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
    }
    EnemyBase::EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp & camera, float sizeScale )
        : object( pos, Vec3f( 1.7 * sizeScale, 1.7 * sizeScale, 0.01 ), Vec3f::zero( ) )
        , maxHP( 5.0F )
        , HP( maxHP )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( 3 )
        , deadTime( 60 )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
    }
    EnemyBase::EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp & camera, std::string const & path )
        : object( pos, Vec3f( 1.7, 1.7, 0.01 ), Vec3f::zero( ) )
        , texture( &GData::FindTexture( path ) )
        , maxHP( 5.0F )
        , HP( maxHP )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( 3 )
        , deadTime( 60 )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
    }
    EnemyBase::EnemyBase( cinder::Vec3f pos, float sizeScale, const cinder::CameraPersp & camera, std::string const & path )
        : object( pos, Vec3f( 1.7 * sizeScale, 1.7 * sizeScale, 0.01 ), Vec3f::zero( ) )
        , texture( &GData::FindTexture( path ) )
        , maxHP( 5.0F )
        , HP( maxHP )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( 3 )
        , deadTime( 60 )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
    }
    EnemyBase::EnemyBase( cinder::Vec3f pos, float sizeScale, float HP, int attackPoint, const cinder::CameraPersp & camera, std::string const & path )
        : object( pos, Vec3f( 1.7 * sizeScale, 1.7 * sizeScale, 0.01 ), Vec3f::zero( ) )
        , texture( &GData::FindTexture( path ) )
        , maxHP( HP )
        , HP( maxHP )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( attackPoint )
        , deadTime( 60 )
    {
        update( camera );
        initObject = object;
        SpawnEffect( camera );
    }
    void EnemyBase::update( cinder::CameraPersp const& camera )
    {
        CameraSee( camera );

        if ( IsLive( ) )
        {
            object.PositionAdd( object.Speed( ) );
            LiveCheck( );
            DamageEffect( );
            attackTime.Update( );
        }
        CollideGround( );// 死んでいても実行します。
        CollideField( );// 死んでいても実行します。
        Dying( );// 死んでいても実行します。

        // デバッグダメージ
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) )
            Kill( );
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

        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 180, 180 ) );
        gl::color( HitColor( ) );
        texture->bind( );
        gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
        texture->unbind( );
        gl::popModelView( );

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
    int EnemyBase::Hit( float length, float value )
    {
        if ( length <= 0.2F )
        {
            HP = std::max( HP - 3.0F * value, 0.0F );
            hitColor = Color( 1, 0, 0 );
            return 7;
        }
        else if ( length <= 0.5F )
        {
            HP = std::max( HP - 2.5F * value, 0.0F );
            hitColor = Color( 1, 0.3, 0.3 );
            return 5;
        }
        else if ( length <= 1.0F )
        {
            HP = std::max( HP - 1.5F * value, 0.0F );
            hitColor = Color( 1, 0.6, 0.6 );
            return 2;
        }
        return 0;
    }
    int EnemyBase::Damage( int damage )
    {
        if ( damage < 0 ) return 0;
        HP = std::max( HP - damage, 0.0F );
        hitColor = Color( 1, 0, 0 );

        return 2;
    }
    void EnemyBase::Kill( )
    {
        HP = 0.0F;
        hitColor = Color( 1, 0, 0 );
    }
    bool EnemyBase::IsActive( )
    {
        return 0 < deadTime;
    }
    bool EnemyBase::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    EnemyBulletList EnemyBase::BulletsRecovery( )
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
        if ( isLive == false ) return;
        isLive = 0.0F < HP;
    }
    void EnemyBase::Dying( )
    {
        if ( isLive == false )
        {
            deadTime = std::max( deadTime - 1, 0 );
        }
    }
    void EnemyBase::DamageEffect( )
    {
        if ( isLive )
            hitColor += Color( 0.03, 0.03, 0.03 );
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
        EffectCreate( EffectBase( "Textures/Effect/kumo2.png",
                                  vec,
                                  Vec2f( vec - size ) * 2.5F,
                                  Vec2f( 600, 300 ),
                                  EffectBase::Mode::CENTERCENTER, true
        ) );
    }
}