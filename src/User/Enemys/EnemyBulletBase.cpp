# include "EnemyBulletBase.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"

# include "../Utilitys/Hirasawa.h"

namespace User
{
    using namespace cinder;

    EnemyBulletBase::EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end )
        : object( begin, Vec3f( 0.15, 0.15, 0.01 ), Vec3f::zero( ) )
        , begin( begin )
        , end(end)
        , attackPoint( 3 )
        , maxMoveTime( 60 * 3 )
        , moveTime( 0 )
        , isActive( true )
    {
        object.Direction( Vec3f( end - begin ).normalized( ) );
    }

    EnemyBulletBase::EnemyBulletBase( cinder::Vec3f begin, cinder::Vec3f end, int attackPoint )
        : object( begin, Vec3f( 0.15, 0.15, 0.01 ), Vec3f::zero( ) )
        , begin( begin )
        , end( end )
        , attackPoint( attackPoint )
        , maxMoveTime( 60 * 3 )
        , moveTime( 0 )
        , isActive( true )
    {
        object.Direction( Vec3f( end - begin ).normalized( ) );
    }

    void EnemyBulletBase::update( )
    {
        MoveCount( );
        Move( NormalizedMoveTime( ) );
    }

    void EnemyBulletBase::draw( )
    {
        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );

        gl::pushModelView( );
        gl::rotate( Vec3f( 0, 180, 180 ) );
        gl::color( Color::white( ) );
        gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
        gl::popModelView( );

    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
    #endif // _DEBUG

        gl::popModelView( );
    }

    bool EnemyBulletBase::Attack( const cinder::CameraPersp& camera )
    {
        return maxMoveTime <= moveTime;
    }

    int EnemyBulletBase::Hit( float length )
    {
        if ( length <= 1.0F )
        {
            Erase( );
            return 5;
        }
        return 0;
    }

    int EnemyBulletBase::Kill( )
    {
        Erase( );
        return 2;
    }

    bool EnemyBulletBase::IsActive( )
    {
        return isActive;
    }

    void EnemyBulletBase::MoveCount( )
    {
        moveTime = std::min( moveTime + 1, maxMoveTime );
    }

    void EnemyBulletBase::Move( float t )
    {
        Vec3f position;

        position.x = EasingLinear( t, begin.x, end.x );
        position.y = EasingLinear( t, begin.y, end.y );
        position.z = EasingLinear( t, begin.z, end.z );

        object.Position( position );
    }
    void EnemyBulletBase::Erase( )
    {
        isActive = false;
    }
}