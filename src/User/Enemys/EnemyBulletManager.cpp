# include "EnemyBulletManager.h"

namespace User
{
    using namespace cinder;

    EnemyBulletManager::EnemyBulletManager( )
    {

    }

    void EnemyBulletManager::update( )
    {
        std::for_each( bulletList.begin( ), bulletList.end( ), [ ] ( EnemyBulletBaseRef& bulletRef ) { bulletRef->update( ); } );
        BulletEraser( );
    }

    void EnemyBulletManager::draw( )
    {
        std::for_each( bulletList.begin( ), bulletList.end( ), [ ] ( EnemyBulletBaseRef& bulletRef ) { bulletRef->draw( ); } );
    }

    void EnemyBulletManager::BulletRegister( EnemyBulletList& bulletList )
    {
        this->bulletList.splice( this->bulletList.end( ), bulletList );
    }

    int EnemyBulletManager::PlayerToEnemyDamage( Line & line_, const cinder::CameraPersp & camera )
    {
        int drainMp = 0;
        std::for_each( bulletList.begin( ), bulletList.end( ), [ &drainMp, &line_, &camera ] ( EnemyBulletBaseRef& bulletRef )
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
        std::for_each( bulletList.begin( ), bulletList.end( ), [ &drainMp ] ( EnemyBulletBaseRef& bulletRef )
        {
            drainMp += bulletRef->Kill( );
        } );
        return drainMp;
    }

    int EnemyBulletManager::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        std::for_each( bulletList.begin( ), bulletList.end( ), [ &damage, &camera ] ( EnemyBulletBaseRef& bulletRef )
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
        std::for_each( bulletList.begin( ), bulletList.end( ), [ &totalDamage, &line_, &camera ] ( EnemyBulletBaseRef& bulletRef )
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
        std::for_each( bulletList.begin( ), bulletList.end( ), [ &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;

            gl::color( ColorA(1, 1, 1, 0.25F) );
            gl::drawSolidCircle( vec, radius, radius );
        } );
    }

    void EnemyBulletManager::BulletEraser( )
    {
        auto eraceList = std::remove_if( bulletList.begin( ), bulletList.end( ), [ ] ( EnemyBulletBaseRef& bulletRef ) { return !bulletRef->IsActive( ); } );
        bulletList.erase( eraceList, bulletList.end( ) );
    }
}