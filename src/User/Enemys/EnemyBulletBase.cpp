# include "EnemyBulletBase.h"

# include "cinder/ImageIo.h"
# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    EnemyBulletBase::EnemyBulletBase( cinder::Vec3f position, cinder::Vec3f speed )
        : object( position, Vec3f( 0.1, 0.1, 0.01 ), speed )
        , attackPoint( 3 )
        , activeTime( 60 * 10 )
        , isActive( true )
    {
        object.Direction( speed );
    }

    void EnemyBulletBase::update( )
    {
        ActiveCount( );
        Move( );
    }

    void EnemyBulletBase::draw( )
    {
        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );
        gl::drawCube( Vec3f::zero( ), object.Size( ) );
    #ifdef _DEBUG
        gl::color( Color::white( ) );
        gl::drawStrokedCube( Vec3f::zero( ), object.Size( ) );
    #endif // _DEBUG
        gl::popModelView( );
    }

    bool EnemyBulletBase::Attack( const cinder::CameraPersp& camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( );
        // エネミの場所がカメラよりも手前側だったら攻撃します。
        if ( cameraPos.dot( cameraPos - object.Position( ) ) < 0 ) return true;
        else return false;
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
        return 5;
    }

    bool EnemyBulletBase::IsActive( )
    {
        return activeTime <= 0 || isActive;
    }

    void EnemyBulletBase::ActiveCount( )
    {
        activeTime = std::max( activeTime - 1, 0 );
    }

    void EnemyBulletBase::Move( )
    {
        object.PositionAdd( object.Speed( ) );
    }
    void EnemyBulletBase::Erase( )
    {
        isActive = false;
    }
}