# include "EnemyBulletManager.h"

namespace User
{
    using namespace cinder;

    EnemyBulletManager::EnemyBulletManager( )
    {

    }

    void EnemyBulletManager::update( )
    {
        Each( [ ] ( EnemyBulletBaseRef& bulletRef ) { bulletRef->update( ); } );
        BulletEraser( );
    }

    void EnemyBulletManager::draw( )
    {
        Each( [ ] ( EnemyBulletBaseRef& bulletRef ) { bulletRef->draw( ); } );
    }

    void EnemyBulletManager::BulletRegister( EnemyBulletList& bulletList )
    {
        this->bulletList.splice( this->bulletList.end( ), bulletList );
    }

    int EnemyBulletManager::PlayerToEnemyDamage( Line & line_, const cinder::CameraPersp & camera )
    {
        int drainMp = 0;
        Each( [ &drainMp, &line_, &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;
            drainMp += bulletRef->Hit( CheckDefLineOfCircle( line_, vec, radius ) );
        } );
        return drainMp;
    }

    int EnemyBulletManager::PlayerSpecialAttackToEnemyDamage( )
    {
        int drainMp = 0;
        Each( [ &drainMp ] ( EnemyBulletBaseRef& bulletRef )
        {
            drainMp += bulletRef->Kill( );
        } );
        return drainMp;
    }

    int EnemyBulletManager::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        Each( [ &damage, &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            if ( bulletRef->Attack( camera ) == true )
            {
                damage += bulletRef->AttackPoint( );
                bulletRef->Erase( );
            }
        } );
        return damage;
    }

    int EnemyBulletManager::EnemyToPlayerDamage( Line & line_, const cinder::CameraPersp & camera )
    {
        int totalDamage = 0;
        Each( [ &totalDamage, &line_, &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            if ( bulletRef->Attack( camera ) )
            {
                Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                float radius = Vec3f( size - vec ).length( ) / 2.0F;
                if ( CheckDefLineOfCircle( line_, vec, radius + 50 ) > 1.0f ) totalDamage += bulletRef->AttackPoint( );
                bulletRef->Erase( );
            }
        } );
        return totalDamage;
    }

    void EnemyBulletManager::DrawCollisionCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;

            gl::color( ColorA( 1, 1, 1, 0.25F ) );
            gl::drawSolidCircle( vec, radius, radius );
        } );
    }

    void EnemyBulletManager::DrawBulletCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->EndPosition( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->EndPosition( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;

            gl::color( ColorA( 1, 0, 0, 0.5F ) );
            gl::drawSolidCircle( vec, radius, radius );
            gl::color( ColorA( 1, 1, 0, 1.0F ) );
            float time = 1.8 - 0.8 * bulletRef->NormalizedMoveTime( );
            gl::drawStrokedCircle( vec, radius * time, radius * time );
        } );
    }

    void EnemyBulletManager::Each( std::function<void( EnemyBulletBaseRef& )> func )
    {
        std::for_each( bulletList.begin( ), bulletList.end( ), func );
    }

    void EnemyBulletManager::BulletEraser( )
    {
        auto eraceList = std::remove_if( bulletList.begin( ), bulletList.end( ), [ ] ( EnemyBulletBaseRef& bulletRef ) { return !bulletRef->IsActive( ); } );
        bulletList.erase( eraceList, bulletList.end( ) );
    }
}