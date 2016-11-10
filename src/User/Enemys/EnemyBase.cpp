# include "EnemyBase.h"

namespace User
{
    using namespace cinder;

    EnemyBase::EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : object( pos, Vec3f( 0.5, 0.8, 0.01 ), Vec3f::zero( ) )
        //, initObject( object )
        , HP( 5.0F )
        , hitColor( Color::white( ) )
        , isLanding( true )
        , isLive( true )
        , attackPoint( 3 )
        , deadTime( 60 )
    {
        object.Direction( camera.getEyePoint( ) - object.Position( )  );
        initObject = object;
    }
    void EnemyBase::update( )
    {
        Gravitate( );
        CollideGround( );
        LiveCheck( );
        Dying( );
        DamageEffect( );
    }
    void EnemyBase::draw( )
    {
        gl::color( ColorA( 1, 1, 0, 1 ) );
        gl::drawVector( object.Position( ), object.Position( ) + object.Direction( ) * 0.1 );

        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );
        gl::color( hitColor );
        gl::drawCube( Vec3f::zero( ), object.Size( ) );
    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG
        gl::popModelView( );
    }
    int EnemyBase::Hit( float length )
    {
        if ( length <= 0.2F )
        {
            HP -= 2.0F;
            hitColor = Color( 1, 0, 0 );
            return 7;
        }
        else if ( length <= 0.5F )
        {
            HP -= 1.5F;
            hitColor = Color( 1, 0.3, 0.3 );
            return 5;
        }
        else if ( length <= 1.0F )
        {
            HP -= 1.0F;
            hitColor = Color( 1, 0.6, 0.6 );
            return 2;
        }
        return 0;
    }
    bool EnemyBase::IsActive( )
    {
        return 0 < deadTime;
    }
    bool EnemyBase::Attack( const cinder::CameraPersp& camera )
    {
        return false;
    }
    void EnemyBase::ResetPosition( )
    {
        object = initObject;
    }
    EnemyBulletList EnemyBase::BulletsRecovery( )
    {
        auto ret = bulletList;
        bulletList.clear( );
        return ret;
    }
    bool EnemyBase::IsUnderGround( )
    {
        return Position( ).y - Size( ).y / 2.0F < 0.0F;
    }
    bool EnemyBase::IsJumping( )
    {
        return isLanding == false;
    }
    void EnemyBase::Gravitate( )
    {
        //　ジャンプ中なら重力をかけます。
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
            object.PositionAdd( object.Speed( ) );
            object.PositionAdd( object.Direction( ) * 0.03 );
        }
    }
    void EnemyBase::CollideGround( )
    {
        // オブジェクトの埋まった位置を戻します。
        auto pos = object.Position( );
        auto speed = object.Speed( );
        if ( IsUnderGround( ) )
        {
            isLanding = true;

            pos.y = Size( ).y / 2.0F;
            speed.y = 0;
            object.Position( pos );
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
}