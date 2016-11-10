#include "EnemyController.h"

#include <algorithm>

#include "cinder/Rand.h"

#include "EnemyBase.h"
#include "SlashEnemy.h"
#include "BulletEnemy.h"

namespace User
{
    using namespace cinder;
    EnemyController::EnemyController( const cinder::CameraPersp& camera )
    {
        for ( int i = 0; i < 3; ++i ) Create<SlashEnemy>( Vec3f( randFloat( -1.5F, 1.5F ), 0, randFloat( -1, 1 ) ), camera );
        Create<BulletEnemy>( Vec3f( randFloat( -1.5F, 1.5F ), 0, randFloat( -1, 1 ) ), camera );
    }

    void EnemyController::update( )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), [ ] ( EnemyBaseRef& enemyRef ) { enemyRef->update( ); } );

        EnemyEraser( );

        EnemyBulletsRecovery( );
    }

    void EnemyController::draw( )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), [ ] ( EnemyBaseRef& enemyRef ) { enemyRef->draw( ); } );
    }

    bool EnemyController::IsAttack( const cinder::CameraPersp& camera )
    {
        bool isAttack = false;
        std::for_each( enemyList.begin( ), enemyList.end( ), [ &isAttack, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true )
                isAttack = true;
        } );
        return isAttack;
    }

    int EnemyController::PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera )
    {
        int drainMp = 0;
        std::for_each( enemyList.begin( ), enemyList.end( ), [ &drainMp, &line_, &camera ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;
            drainMp += enemyRef->Hit( CheckDefLineOfCircle( line_, vec, radius ) );
        } );
        return drainMp;
    }

    int EnemyController::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        std::for_each( enemyList.begin( ), enemyList.end( ), [ &damage, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true )
                damage += enemyRef->AttackPoint( );
        } );

        return damage;
    }

    int EnemyController::EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera )
    {
        int totalDamage = 0;
        std::for_each( enemyList.begin( ), enemyList.end( ), [ &totalDamage, &line_, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) )
            {
                Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                float radius = Vec3f( size - vec ).length( ) / 2.0F;
                if ( CheckDefLineOfCircle( line_, vec, radius + 50 ) > 1.0f ) totalDamage += enemyRef->AttackPoint( );
            }
        } );
        return totalDamage;
    }

    void EnemyController::ResetPosition( const cinder::CameraPersp& camera )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), [ &camera ] ( EnemyBaseRef& enemyRef ) { if ( enemyRef->Attack( camera ) ) enemyRef->ResetPosition( ); } );
    }

    EnemyBulletList EnemyController::BulletsRecovery( )
    {
        auto ret = bulletList;
        bulletList.clear( );
        return ret;
    }

    void EnemyController::EnemyEraser( )
    {
        auto eraceList = std::remove_if( enemyList.begin( ), enemyList.end( ), [ ] ( EnemyBaseRef& enemyRef ) { return !enemyRef->IsActive( ); } );
        enemyList.erase( eraceList, enemyList.end( ) );
    }

    void EnemyController::EnemyBulletsRecovery( )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), [ this ] ( EnemyBaseRef& enemyRef )
        {
            // spliceはlist同士をつなげるときに使います。
            // 以下のサイトを参考にしました。
            // http://d.hatena.ne.jp/pknight/20090814/1250227222
            bulletList.splice( bulletList.end( ), enemyRef->BulletsRecovery( ) );
        } );
    }
}